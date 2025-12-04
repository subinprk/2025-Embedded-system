#include "../include/mlx90640.h"
#include "../include/uart.h"
#include "../include/twi.h"
#include <stdio.h>
#include <util/delay.h>

uint16_t MLX_read16(uint16_t reg)
{
    uint8_t hi, lo;
    uint8_t result;

    // Write register address
    result = TWI0_start((MLX_ADDR << 1) | 0);
    if (!result) {
        USART2_sendString("START WRITE FAIL\r\n");
        TWI0_stop();
        TWI0_reset_bus();
        return 0xFFFF;
    }
    
    result = TWI0_write(reg >> 8);
    if (!result) {
        USART2_sendString("WRITE HIGH FAIL\r\n");
        TWI0_stop();
        TWI0_reset_bus();
        return 0xFFFF;
    }
    
    result = TWI0_write(reg & 0xFF);
    if (!result) {
        USART2_sendString("WRITE LOW FAIL\r\n");
        TWI0_stop();
        TWI0_reset_bus();
        return 0xFFFF;
    }

    // NO STOP - use Repeated START for read
    result = TWI0_start((MLX_ADDR << 1) | 1);
    if (!result) {
        USART2_sendString("START READ FAIL\r\n");
        TWI0_stop();
        TWI0_reset_bus();
        return 0xFFFF;
    }
    
    hi = TWI0_read_ack();
    lo = TWI0_read_nack();
    // TWI0_stop();  // read_nack already sends STOP
    
    _delay_us(100);  // Small delay between transactions

    return (hi << 8) | lo;
}

void debug_MLX_read16(uint16_t reg)
{
    uint16_t value = MLX_read16(reg);
    char buffer[30];
    snprintf(buffer, sizeof(buffer), "Reg 0x%04X: 0x%04X\r\n", reg, value);
    USART2_sendString(buffer);
}

// MPU6050 functions (I2C address 0x68)
#define MPU6050_ADDR 0x68

uint8_t MPU6050_read8(uint8_t reg)
{
    uint8_t result;
    
    // Write register address
    result = TWI0_start((MPU6050_ADDR << 1) | 0);
    if (!result) {
        TWI0_stop();
        return 0xFF;
    }
    
    result = TWI0_write(reg);
    if (!result) {
        TWI0_stop();
        return 0xFF;
    }

    // Repeated START and read
    result = TWI0_start((MPU6050_ADDR << 1) | 1);
    if (!result) {
        TWI0_stop();
        return 0xFF;
    }
    
    uint8_t data = TWI0_read_nack();
    return data;
}

void debug_MPU6050_read8(uint8_t reg, const char *label)
{
    uint8_t value = MPU6050_read8(reg);
    char buffer[40];
    snprintf(buffer, sizeof(buffer), "%s: 0x%02X\r\n", label, value);
    USART2_sendString(buffer);
}