#define UART_F_CPU 16000000UL 
#define BAUD_RATE 9600

#include <avr/io.h>
#include "../include/uart.h"

void USART1_init(void)
{
    // Route USART1 to PC0 (TX) and PC1 (RX) - this is the DEFAULT mapping
    PORTMUX.USARTROUTEA = PORTMUX_USART1_DEFAULT_gc;

    // Set PC0 as output (TX), PC1 as input (RX)
    PORTC.DIRSET = PIN0_bm;
    PORTC.DIRCLR = PIN1_bm;

    USART1.BAUD = (uint16_t)((UART_F_CPU * 64UL) / (16UL * BAUD_RATE));
    USART1.CTRLC = USART_CHSIZE_8BIT_gc;
    USART1.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
}

void USART1_sendChar(char c)
{
    while (!(USART1.STATUS & USART_DREIF_bm));
    USART1.TXDATAL = c;
}

void USART1_sendString(const char *s)
{
    while (*s)
    {
        USART1_sendChar(*s++);
    }
}

char USART1_readChar(void)
{
    while (!(USART1.STATUS & USART_RXCIF_bm));
    return USART1.RXDATAL;
}
