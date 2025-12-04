// BAUD_RATE is defined in Makefile via -DBAUD_RATE=xxxx
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
    // Set PF0 as output (TX), PF1 as input (RX) FIRST
    PORTF.DIRSET = PIN0_bm;
    PORTF.DIRCLR = PIN1_bm;
    
    // Route USART2 to PF0 (TX) and PF1 (RX) - DEFAULT mapping
    PORTMUX.USARTROUTEA = (PORTMUX.USARTROUTEA & ~PORTMUX_USART2_gm) | PORTMUX_USART2_DEFAULT_gc;

    // Calculate baud rate from F_CPU (defined in Makefile)
    // BAUD = (64 * F_CPU) / (16 * BAUD_RATE)
    USART2.BAUD = (uint16_t)((F_CPU * 64UL) / (16UL * BAUD_RATE));
    
    // 8-bit data, no parity, 1 stop bit
    USART2.CTRLC = USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc;
    
    // Enable TX and RX
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