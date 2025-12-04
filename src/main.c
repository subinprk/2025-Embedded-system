#include <avr/io.h>
#include <util/delay.h>
#include "../include/uart.h"
#include "../include/twi.h"
#include "../include/mlx90640.h"
#include "../include/mpu6050.h"

int main(void)
{
    // Use default clock - don't touch it
    // Factory default: 20MHz / 6 = 3.33MHz
    
    _delay_ms(100);  // Power-up stabilization

    USART2_init();

    _delay_ms(100);  // UART stabilization
    
    // LED toggle test (PF5 as LED)
    PORTF.DIRSET = PIN5_bm;
    
    // Send startup message
    USART2_sendString("\r\n");
    USART2_sendString("ATmega4809 UART2 Started!\r\n");
    
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