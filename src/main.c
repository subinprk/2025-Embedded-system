#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "../include/uart.h"
#include "../include/twi.h"
#include "../include/mlx90640.h"
#include "../include/mpu6050.h"
#include "../include/mlxProcess.h"
#include "../include/pwm.h"
#include "../include/interrupt.h"
#include "../include/debugging.h"

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

    // ===== Hardware sanity test (blocking) =====
    // Configure LED pin early
    PORTF.DIRSET = PIN5_bm;

    // Blink LED 3 times + send UART message to confirm clock/UART/GPIO work
    USART2_sendString("\r\n### STARTUP SANITY ###\r\n");
    for (uint8_t i = 0; i < 3; i++) {
        PORTF.OUTSET = PIN5_bm;   // LED on
        _delay_ms(200);
        PORTF.OUTCLR = PIN5_bm;   // LED off
        _delay_ms(200);
    }
    USART2_sendString("LED blink done, proceeding...\r\n");
    // ============================================

    TWI0_init();
    motor_init();
    scheduler_init();
    _delay_ms(100);
    
    // Aggressive I2C bus recovery at startup
    for (uint8_t i = 0; i < 3; i++) {
        TWI0_reset_bus();
        _delay_ms(50);
    }
    
    // initial_debugging();
    // Initialize motor driver pins
   
    // Main cooperative loop: service tasks when flags are set
    // uint16_t loop_cnt = 0;
    // char dbg[32];
    
    // Re-enable interrupts in case something disabled them
    // sei();
    // TCB0.INTFLAGS = TCB_CAPT_bm;
    // USART2_sendString("[MAIN LOOP START - sei() called]\r\n");

    initial_debugging();
    while (1)
    {
        //         char buf[64];
        //     snprintf(buf, sizeof(buf),
        //         "PORTMUX_TCBROUTEA = %02X\r\n",
        //         PORTMUX.TCBROUTEA);
        //     USART2_sendString(buf);
        scheduler_service_tasks();

        // static uint16_t last_cnt = 0;
        // static uint16_t dbg_print = 0;

        // dbg_print++;
        // if (dbg_print >= 10000) {
        //     dbg_print = 0;
        //     char b[64];
        //     uint16_t cnt = TCB0.CNT;
        //     uint8_t flags = TCB0.INTFLAGS;
        //     snprintf(b, sizeof(b), "CNT=%u FLAGS=%02X\r\n", cnt, flags);
        //     USART2_sendString(b);
        //     last_cnt = cnt;
        // }

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

ISR(TCB0_INT_vect)
{
    TCB0.INTFLAGS = TCB_CAPT_bm;
    PORTF.OUTTGL = PIN5_bm;   // toggle LED
}
