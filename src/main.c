#include <avr/io.h>
#include <util/delay.h>
#include "../include/uart.h"
#include "../include/twi.h"
#include "../include/mlx90640.h"
#include "../include/mpu6050.h"

// Function to set CPU clock to maximum speed (16MHz or 20MHz depending on fuse)
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
    
    _delay_ms(100);  // Power-up stabilization

    USART2_init();

    _delay_ms(100);  // UART stabilization
    
    // LED toggle test (PF5 as LED)
    PORTF.DIRSET = PIN5_bm;
    
    // Send startup message
    USART2_sendString("\r\n");
    USART2_sendString("ATmega4809 running at 16MHz!\r\n");
    
    uint8_t counter = 0;
    
    while (1)
    {
        PORTF.OUTTGL = PIN5_bm;  // LED toggle
        
        // Send readable debug message
        USART2_sendString("Hello ");
        USART2_sendChar('0' + (counter % 10));  // Send counter 0-9
        USART2_sendString("\r\n");
        
        counter++;
        _delay_ms(1000);  // 1 second delay for easy reading
    }
}