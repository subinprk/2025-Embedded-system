#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "../include/uart.h"
#include "../include/twi.h"
#include "../include/mlx90640.h"
#include "../include/mpu6050.h"
#include "../include/mlxProcess.h"
#include "../include/pwm.h"
#include "../include/interrupt.h"
#include "../include/debugging.h"
#include "../include/drive.h"

// Function to set CPU clock to maximum speed (16MHz or 20MHz depending on Vcc)
void clock_init(void)
{
    // Set prescaler to /1 (no division) for maximum speed
    // Your chip has 16MHz base oscillator (based on earlier UART testing)
    // This will give you 16MHz CPU clock instead of 2.67MHz
    
    CCP = CCP_IOREG_gc;              // Unlock protected register
    CLKCTRL.MCLKCTRLB = 0x00;        // Disable prescaler (PEN = 0)
    
    // Wait for clock to stabilize
    while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm);
}

int main(void)
{
    // Set CPU clock to 16MHz (disable /6 prescaler)
    clock_init();
    _delay_ms(100); 
    USART2_init();
    timer_init_1khz();

    // ===== Hardware sanity test (blocking) =====
    // Configure LED pin early
    PORTF.DIRSET = PIN5_bm;
    PORTF.OUTCLR = PIN5_bm; // LED off

    TWI0_init();
    motor_init();
        drive_init();
    // scheduler_init();
    _delay_ms(100);
    
    // Set MLX90640 to 16Hz frame rate for faster data acquisition
    USART2_sendString("Setting MLX90640 to 16Hz...\r\n");
    MLX_set_framerate(MLX_FRAMERATE_32HZ);
    USART2_sendString("MLX90640 configured.\r\n");
    
    // Aggressive I2C bus recovery at startup
    for (uint8_t i = 0; i < 5; i++) {
        TWI0_reset_bus();
        _delay_ms(50);
    }

    // initial_debugging();
    int loop_count = 0;
    while (1)
    {
        scheduler_service_tasks();
        // sensor_loop_debugging(loop_count);
        pwm_loop_debugging(loop_count);
        drive_update();
    }
}
