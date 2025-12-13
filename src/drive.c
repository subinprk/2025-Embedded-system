#include "../include/drive.h"
#include "../include/pwm.h"
#include "../include/mlxProcess.h"
#include "../include/mlx90640.h"
#include "../include/uart.h"

// Inputs: PD2, PD3, PD4 (digital sensors: 1 when within distance)
// Output: PA7 (solenoid)
// Behavior:
// - Use MLX centroid: if left of center, rotate left (left wheel stop, right forward)
//   if right of center, rotate right (right wheel stop, left forward).
// - When centroid is centered and PD3 == 1, actuate solenoid on PA7.

static inline uint8_t pd_read(uint8_t bm) {
    return (PORTD.IN & bm) ? 1 : 0;
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

void drive_update(void)
{
    // Read sensors
    uint8_t s_left = pd_read(PIN2_bm);
    uint8_t s_center = pd_read(PIN3_bm);
    uint8_t s_right = pd_read(PIN4_bm);

    // Compute centroid using existing processing helper (top 20%)
    CentroidResult c = MLX_find_hot_centroid_auto(20);

    // MLX frame is 32x24; define center window
    const uint8_t center_x = MLX_WIDTH / 2;      // 16
    const uint8_t center_y = MLX_HEIGHT / 2;     // 12
    const uint8_t deadband_x = 2;  // allow small error (±2 columns)
    const int8_t base_speed = 35;   // Base speed (slow): 35% duty cycle

    // Convert fixed-point to integer pixel coordinate
    uint8_t cx = (uint8_t)(c.x_fp / 256);
    uint8_t cy = (uint8_t)(c.y_fp / 256);

    // Calculate horizontal error (centroid offset from center)
    int16_t error_x = (int16_t)cx - (int16_t)center_x;  // Range: -16 to +16

    uint8_t obstacle_handled = 0;

    // Obstacle avoidance has priority unless we're centered on heat and firing solenoid
    if (s_center && !(cx >= (center_x - deadband_x) && cx <= (center_x + deadband_x))) {
        // Obstacle straight ahead (not centered on heat) → back up
        motorA_set_speed(-base_speed);
        motorB_set_speed(-base_speed);
        obstacle_handled = 1;
    } else if (s_left && s_right) {
        // Obstacles both sides → back up
        motorA_set_speed(-base_speed);
        motorB_set_speed(-base_speed);
        obstacle_handled = 1;
    } else if (s_left && !s_right) {
        // Obstacle on left → rotate right (motorA forward, motorB stop/slow)
        motorA_set_speed(base_speed);
        motorB_set_speed(0);
        obstacle_handled = 1;
    } else if (s_right && !s_left) {
        // Obstacle on right → rotate left (motorB forward, motorA stop/slow)
        motorA_set_speed(0);
        motorB_set_speed(base_speed);
        obstacle_handled = 1;
    }

    if (!obstacle_handled) {
        // Dynamic speed control based on centroid position
        // Calculate compensation: larger error = larger speed difference
        // compensation = (error_x * base_speed) / center_x
        // This way: if error is large, one motor gets much faster than the other
        // if error is small, both motors are similar speed (gentle turn)
        
        int16_t compensation = (error_x * base_speed) / (int16_t)center_x;
        
        // Clamp compensation to prevent excessive speed
        if (compensation > base_speed) compensation = base_speed;
        if (compensation < -base_speed) compensation = -base_speed;

        if (error_x < -deadband_x) {
            // Hotspot left of center → rotate left
            // motorB faster, motorA slower
            motorA_set_speed(base_speed - compensation);
            motorB_set_speed(base_speed + compensation);
        } else if (error_x > deadband_x) {
            // Hotspot right of center → rotate right
            // motorA faster, motorB slower
            motorA_set_speed(base_speed + compensation);
            motorB_set_speed(base_speed - compensation);
        } else {
            // Centered: both motors at same speed (move forward)
            motorA_set_speed(base_speed);
            motorB_set_speed(base_speed);
        }
    }

    // Solenoid condition: centered and PD3 sensor == 1
    uint8_t centered = (cx >= (center_x - deadband_x)) && (cx <= (center_x + deadband_x));
    if (centered && s_center) {
        PORTA.OUTSET = PIN7_bm;  // activate solenoid
    } else {
        PORTA.OUTCLR = PIN7_bm;  // deactivate solenoid
    }
}
