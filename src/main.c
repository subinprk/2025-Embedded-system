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

// runtime ISR counter removed; scheduler-driven tasks used instead


// Function to set CPU clock to maximum speed (16MHz or 20MHz depending on Vcc)
void clock_init(void)
{
    // To change protected registers, we need to write to CCP first
    // Then we have 4 clock cycles to write the actual register
    
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
    scheduler_init();
    _delay_ms(100);
    
    // Aggressive I2C bus recovery at startup
    for (uint8_t i = 0; i < 5; i++) {
        TWI0_reset_bus();
        _delay_ms(50);
    }

    initial_debugging();
    while (1)
    {
        scheduler_service_tasks();

        // Clean main loop: scheduler services tasks. Debug prints removed.

            //         char b[64];
            // snprintf(b, sizeof(b),
            // "INTCTRL=%02X CTRLA=%02X CNT=%u FLAGS=%02X\r\n",
            // TCB0.INTCTRL, TCB0.CTRLA, TCB0.CNT, TCB0.INTFLAGS);
            // USART2_sendString(b);



        // Every 50000 loops, print a heartbeat to confirm loop is running
        // loop_cnt++;
        // if (loop_cnt == 0) {  // wraps every 65536
        //     // Check SREG I-bit
        //     if (SREG & 0x80) {
        //         USART2_sendString("I=1\r\n");
        //     } else {
        //         USART2_sendString("I=0\r\n");
        //     }
        // }
    }
}

// ISR handled in interrupt.c; scheduler will service tasks
