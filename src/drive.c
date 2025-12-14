#include "../include/drive.h"
#include "../include/pwm.h"
#include "../include/mlxProcess.h"
#include "../include/mlx90640.h"
#include "../include/uart.h"
#include <stdio.h>

// Inputs: PD2, PD3, PD4 (digital sensors: 1 when within distance)
// Output: PA7 (solenoid)
// Behavior:
// - Use MLX centroid: if left of center, rotate left (left wheel stop, right forward)
//   if right of center, rotate right (right wheel stop, left forward).
// - When centroid is centered and PD3 == 1, actuate solenoid on PA7.

// Debouncing state for sensors
#define DEBOUNCE_THRESHOLD 5  // 5 consecutive reads = ~5ms at typical update rate
static struct {
    uint8_t pd2_count;
    uint8_t pd3_count;
    uint8_t pd4_count;
    uint8_t pd2_state;
    uint8_t pd3_state;
    uint8_t pd4_state;
} sensor_state = {0, 0, 0, 1, 1, 1};  // Initialize to HIGH (no obstacle)

static inline uint8_t pd_read(uint8_t bm) {
    return (PORTD.IN & bm) ? 1 : 0;
}

// Debouncing function for individual sensor
static inline uint8_t debounce_sensor(uint8_t pin_bm, uint8_t *count, uint8_t *state) {
    uint8_t raw = pd_read(pin_bm);
    
    if (raw != *state) {
        // State changed, increment counter
        (*count)++;
        if (*count >= DEBOUNCE_THRESHOLD) {
            // State confirmed stable
            *state = raw;
            *count = 0;
        }
    } else {
        // State stable, reset counter
        *count = 0;
    }
    
    return *state;
}

// Update all sensor states with debouncing
static inline void update_debounced_sensors(uint8_t *s_left, uint8_t *s_center, uint8_t *s_right) {
    *s_left = debounce_sensor(PIN2_bm, &sensor_state.pd2_count, &sensor_state.pd2_state);
    *s_center = debounce_sensor(PIN3_bm, &sensor_state.pd3_count, &sensor_state.pd3_state);
    *s_right = debounce_sensor(PIN4_bm, &sensor_state.pd4_count, &sensor_state.pd4_state);
}

void drive_init(void)
{
    // Configure PD2, PD3, PD4 as inputs with pull-ups
    PORTD.DIRCLR = PIN2_bm | PIN3_bm | PIN4_bm;
    PORTD.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTD.PIN3CTRL = PORT_PULLUPEN_bm;
    PORTD.PIN4CTRL = PORT_PULLUPEN_bm;

    // Configure PA7 as output (solenoid), default off
    PORTA.DIRSET = PIN7_bm;
    PORTA.OUTCLR = PIN7_bm;
}

const uint8_t deadband_x = 2;  // allow small error (±1 column)
const uint8_t base_speed = 50;   // Base speed (moderate): 50% duty cycle
void drive_update(void)
{
    // Read debounced sensors
    // Sensors are pull-up inputs; active when LOW
    uint8_t s_left_raw, s_center_raw, s_right_raw;
    update_debounced_sensors(&s_left_raw, &s_center_raw, &s_right_raw);
    uint8_t left_hit = !s_left_raw;
    uint8_t center_hit = !s_center_raw;
    uint8_t right_hit = !s_right_raw;
    
    // Debug: Log sensor states
    static uint32_t log_count = 0;
    if (log_count++ % 20 == 0) {  // Log every 20 updates to avoid spam
        char buf[40];
        snprintf(buf, sizeof(buf), "[SENSOR] L=%u C=%u R=%u\r\n", left_hit, center_hit, right_hit);
        USART2_sendString(buf);
    }

    
    // Compute centroid using existing processing helper (top 20%)
    CentroidResult c = MLX_find_hot_centroid_auto(20);

    // MLX frame is 32x24; define center window
    const uint8_t center_x = MLX_WIDTH / 2;      // 16
    const uint8_t center_y = MLX_HEIGHT / 2;     // 12

    // Convert fixed-point to integer pixel coordinate
    uint8_t cx = (uint8_t)(c.x_fp / 256);
    uint8_t cy = (uint8_t)(c.y_fp / 256);

    // Calculate horizontal error (centroid offset from center)
    int16_t error_x = (int16_t)cx - (int16_t)center_x;  // Range: -16 to +16
    uint8_t obstacle_handled = 0;

    // CRITICAL: Check for ANY obstacle detection first - safety override
    if (left_hit || center_hit || right_hit) {
        USART2_sendString("[OBSTACLE DETECTED] Handling obstacle avoidance...\r\n");
    }

    // Obstacle avoidance: forward-only, so use stop instead of backward
    if (center_hit && !(cx >= (center_x - deadband_x) && cx <= (center_x + deadband_x))) {
        // Obstacle straight ahead (not centered on heat) → stop
        motorA_set_speed(0);
        motorB_set_speed(0);
        char buf[60];
        snprintf(buf, sizeof(buf), "[STOP] Obstacle ahead (C_HIT). cx=%u\r\n", cx);
        USART2_sendString(buf);
        obstacle_handled = 1;
    } else if (left_hit && right_hit) {
        // Obstacles both sides → stop
        motorA_set_speed(0);
        motorB_set_speed(0);
        USART2_sendString("[STOP] Obstacles on BOTH sides (L_HIT + R_HIT)\r\n");
        obstacle_handled = 1;
    } else if (left_hit && !right_hit) {
        // Obstacle on left → rotate right (motorA forward, motorB stop/slow)
        motorA_set_speed(0);
        motorB_set_speed(0);
        USART2_sendString("[TURN-RIGHT] Left obstacle detected (L_HIT)\r\n");
        obstacle_handled = 1;
    } else if (right_hit && !left_hit) {
        // Obstacle on right → rotate left (motorB forward, motorA stop/slow)
        motorA_set_speed(0);
        motorB_set_speed(0);
        USART2_sendString("[TURN-LEFT] Right obstacle detected (R_HIT)\r\n");
        obstacle_handled = 1;
    }

    if (!obstacle_handled) {
        // Dynamic speed control based on centroid position (forward only)
        // Stronger turn when error is large, gentle when small
        int16_t abs_err = (error_x < 0) ? -error_x : error_x;
        int16_t compensation = ((abs_err /deadband_x) * base_speed) / ((int16_t)center_x);
        USART2_sendString("[DRIVE] No obstacles, adjusting motors based on centroid\r\n");
        if (compensation > base_speed) compensation = base_speed;

        if (error_x < -deadband_x) {
            // Hotspot left → motorB faster, motorA slower (never negative)
            int16_t speedA = base_speed - compensation;
            int16_t speedB = base_speed + compensation;
            if (speedA < 0) speedA = 0;
            if (speedB > 100) speedB = 100;
            motorA_set_speed((uint8_t)speedA);
            motorB_set_speed((uint8_t)speedB);
            if (log_count % 50 == 0) {
                char buf[50];
                snprintf(buf, sizeof(buf), "[DRIVE] Turn left: A=%d B=%d\r\n", speedA, speedB);
                USART2_sendString(buf);
            }
        } else if (error_x > deadband_x) {
            // Hotspot right → motorA faster, motorB slower
            int16_t speedA = base_speed + compensation;
            int16_t speedB = base_speed - compensation;
            if (speedA > 100) speedA = 100;
            if (speedB < 0) speedB = 0;
            motorA_set_speed((uint8_t)speedA);
            motorB_set_speed((uint8_t)speedB);
            if (log_count % 50 == 0) {
                char buf[50];
                snprintf(buf, sizeof(buf), "[DRIVE] Turn right: A=%d B=%d\r\n", speedA, speedB);
                USART2_sendString(buf);
            }
        } else {
            // Centered
            motorA_set_speed(50);
            motorB_set_speed(50);
            if (log_count % 50 == 0) {
                char buf[50];
                snprintf(buf, sizeof(buf), "[DRIVE] Forward: both=%u\r\n", 50);
                USART2_sendString(buf);
            }
        }
    }
}

void solenoid_update(void)
{
    // Read debounced sensors
    // Sensors are pulled up; assume "active" when pulled LOW by obstacle
    uint8_t s_left, s_center, s_right;
    update_debounced_sensors(&s_left, &s_center, &s_right);

    // Active when any sensor reads low (0)
    bool obstacle = (!s_left) || (!s_center) || (!s_right);

    if (obstacle) {
        PORTA.OUTCLR = PIN7_bm;  // activate solenoid
        char buf[60];
        snprintf(buf, sizeof(buf), "[SOLENOID] ACTIVATED. L=%u C=%u R=%u\r\n", s_left, s_center, s_right);
        USART2_sendString(buf);
        motorA_set_speed(0);
        motorB_set_speed(0);
    } else {
        PORTA.OUTSET = PIN7_bm;  // deactivate solenoid
        // Motor control is ONLY handled by drive_update() - do NOT touch motors here!
    }
}
