#include <avr/io.h>
#include <util/delay.h>
#include "../include/uart.h"
#include "../include/twi.h"
#include "../include/mlx90640.h"
#include "../include/mpu6050.h"

int main(void)
{
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, CLKCTRL_CLKSEL_OSC20M_gc);
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, 0);
    _delay_ms(100); // Wait for clock to stabilize

    USART2_init();

    USART2_sendString("Test Debugging Port\r\n");
    _delay_ms(100);  // 스캔 후 안정화


    while (1)
    {
    }
}