#ifndef UART_H
#define UART_H

#include <avr/io.h>

// UART function prototypes
void USART1_init(void);
void USART1_sendChar(char c);
void USART1_sendString(const char *s);
char USART1_readChar(void);

#endif // UART_H
