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

    // Convert fixed-point to integer pixel coordinate
    uint8_t cx = (uint8_t)(c.x_fp / 256);
    uint8_t cy = (uint8_t)(c.y_fp / 256);

    uint8_t obstacle_handled = 0;

    // Obstacle avoidance has priority unless we're centered on heat and firing solenoid
    if (s_center && !(cx >= (center_x - deadband_x) && cx <= (center_x + deadband_x))) {
        // Obstacle straight ahead (not centered on heat) → back up
        motorA_backward();
        motorB_backward();
        obstacle_handled = 1;
        USART2_sendString("[DRIVE] Obstacle CENTER → BACK\r\n");
    } else if (s_left && s_right) {
        // Obstacles both sides → back up
        motorA_backward();
        motorB_backward();
        obstacle_handled = 1;
        USART2_sendString("[DRIVE] Obstacles BOTH → BACK\r\n");
    } else if (s_left && !s_right) {
        // Obstacle on left → rotate right
        motorB_stop();
        motorA_forward();
        obstacle_handled = 1;
        USART2_sendString("[DRIVE] Obstacle LEFT → RIGHT TURN\r\n");
    } else if (s_right && !s_left) {
        // Obstacle on right → rotate left
        motorA_stop();
        motorB_forward();
        obstacle_handled = 1;
        USART2_sendString("[DRIVE] Obstacle RIGHT → LEFT TURN\r\n");
    }

    if (!obstacle_handled) {
        // Decide rotation based on horizontal error (heat seeking)
        if (cx + deadband_x < center_x) {
            // Hotspot left of center → rotate left (left wheel stop, right forward)
            motorA_stop();
            motorB_forward();
            USART2_sendString("[DRIVE] Rotate LEFT\r\n");
        } else if (cx > center_x + deadband_x) {
            // Hotspot right of center → rotate right (right wheel stop, left forward)
            motorB_stop();
            motorA_forward();
            USART2_sendString("[DRIVE] Rotate RIGHT\r\n");
        } else {
            // Centered: both stop (await action)
            motorA_stop();
            motorB_stop();
            USART2_sendString("[DRIVE] Centered\r\n");
        }
    }

    // Solenoid condition: centered and PD3 sensor == 1
    uint8_t centered = (cx >= (center_x - deadband_x)) && (cx <= (center_x + deadband_x));
    if (centered && s_center) {
        PORTA.OUTSET = PIN7_bm;  // activate solenoid
        USART2_sendString("[DRIVE] Solenoid ON (PA7)\r\n");
    } else {
        PORTA.OUTCLR = PIN7_bm;  // deactivate solenoid
    }
}
