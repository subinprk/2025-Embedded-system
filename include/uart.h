#ifndef UART_H
#define UART_H

#include <avr/io.h>

// UART function prototypes
void USART1_init(void);
void USART1_sendChar(char c);
void USART1_sendString(const char *s);
char USART1_readChar(void);

//To use PF as a debugging UART
void USART2_init(void);
void USART2_sendChar(char c);
void USART2_sendString(const char *s);
char USART2_readChar(void);

#endif // UART_H
