#ifndef TWI_H
#define TWI_H
#include <avr/io.h>

void TWI0_init(void);
void TWI0_reset_bus(void);
uint8_t TWI0_start(uint8_t address);
void TWI0_stop(void);
uint8_t TWI0_write(uint8_t data);
uint8_t TWI0_read_ack(void);
uint8_t TWI0_read_nack(void);
void TWI0_debug_status(void);
void TWI0_scan(void);

#endif // TWI_H