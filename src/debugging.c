#include "../include/debugging.h"

void initial_debugging(void){
 // Send startup message
    USART2_sendString("\r\n");
    USART2_sendString("================================\r\n");
    USART2_sendString("ATmega4809 @ 16MHz - I2C Test\r\n");
    USART2_sendString("================================\r\n");
    
    // I2C Bus scan
    USART2_sendString("\r\nScanning I2C bus...\r\n");
    TWI0_scan();
    
    // Try to read MLX90640 ID
    USART2_sendString("\r\nMLX90640 Device ID test:\r\n");
    debug_MLX_read16(0x2407);
    
    // Initialize MPU6050 (with built-in verification)
    USART2_sendString("\r\n");
    MPU6050_init();
    
    USART2_sendString("\r\n--- Starting main loop ---\r\n");
}

void sensor_loop_debugging(int loop_count){
    char buf[40];
    snprintf(buf, sizeof(buf), "\r\n--- Loop %d ---\r\n", loop_count);
    USART2_sendString(buf);
    
    // Process thermal image (MLX functions now handle bus cleanup internally)
    MLX_process_and_report();
    
    // Extra safety: brief delay for bus to stabilize
    _delay_ms(20);
    
    // MPU6050 quick check with bus status logging
    USART2_sendString("\r\nMPU6050: ");
    TWI0_debug_status_detailed();  // Log bus state before read

    uint8_t who = MPU6050_read8(0x75);
    snprintf(buf, sizeof(buf), "WHO_AM_I=0x%02X\r\n", who);
    USART2_sendString(buf);
    
    if (who == 0x68) {
        MPU6050_debug_test();
    } else {
        // MPU not responding - try re-initialization
        USART2_sendString("[MPU] Lost connection, re-initializing...\r\n");
        TWI0_clock_pulse_stop();
        _delay_ms(50);
        MPU6050_init();
    }
}

void pwm_loop_debugging(int loop_count){
    // int static motor_phase = 0;
    // if ((loop_count % 20) == 0) {
        // switch (motor_phase) {
        //     case 0:
                // both forward
                // motorB_backward();
                motorA_forward();
                motorB_forward();
                USART2_sendString("Motors: BOTH FORWARD\r\n");
                // break;
    //         case 1:
    //             // both stop
    //             motorA_stop();
    //             motorB_stop();
    //             USART2_sendString("Motors: BOTH STOP\r\n");
    //             break;
    //         case 2:
    //             // both backward
    //             motorA_backward();
    //             motorB_backward();
    //             USART2_sendString("Motors: BOTH BACKWARD\r\n");
    //             break;
    //         case 3:
    //             // alternating directions
    //             motorA_forward();
    //             motorB_backward();
    //             USART2_sendString("Motors: A FORWARD, B BACKWARD\r\n");
    //             break;
    //     }
    //     motor_phase = (motor_phase + 1) & 0x03;
    // }
}