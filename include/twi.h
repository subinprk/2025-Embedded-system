#ifndef TWI_H
#define TWI_H
#include <avr/io.h>

// Debug UART selection for TWI
// Set to 1 for USART1 (PC0/PC1), 2 for USART2 (PF0/PF1)
#define TWI_DEBUG_UART  2
// Debug output enable/disable (set to 0 to disable all debug output)
#define TWI_DEBUG_ENABLE  1

void TWI0_init(void);
void TWI0_reset_bus(void);
uint8_t TWI0_start(uint8_t address);
void TWI0_stop(void);
uint8_t TWI0_write(uint8_t data);
uint8_t TWI0_read_ack(void);
uint8_t TWI0_read_nack(void);
void TWI0_debug_status(void);
void TWI0_debug_status_detailed(void);
void TWI0_scan(void);
void TWI0_clock_pulse_stop(void);

#endif // TWI_H