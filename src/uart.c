// #define UART_F_CPU 16000000UL 
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

    USART1.BAUD = (uint16_t)((F_CPU * 64UL) / (16UL * BAUD_RATE));
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

//To use PF as a debugging UART
void USART2_init(void)
{
    // Route USART2 to PF0 (TX) and PF1 (RX)
    PORTMUX.USARTROUTEA |= PORTMUX_USART2_ALT1_gc;

    // Set PF0 as output (TX), PF1 as input (RX)
    PORTF.DIRSET = PIN0_bm;
    PORTF.DIRCLR = PIN1_bm;

    USART2.BAUD = (uint16_t)((F_CPU * 64UL) / (16UL * BAUD_RATE));
    USART2.CTRLC = USART_CHSIZE_8BIT_gc;
    USART2.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
}

void USART2_sendChar(char c)
{
    while (!(USART2.STATUS & USART_DREIF_bm));
    USART2.TXDATAL = c;
}

void USART2_sendString(const char *s)
{
    while (*s)
    {
        USART2_sendChar(*s++);
    }
}

char USART2_readChar(void)
{
    while (!(USART2.STATUS & USART_RXCIF_bm));
    return USART2.RXDATAL;
}