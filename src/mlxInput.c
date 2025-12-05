/*
 * MLX90640 and MPU6050 I2C Communication
 * All reads are silent (no debug output) for clean processing
 */

#include "../include/mlx90640.h"
#include "../include/uart.h"
#include "../include/twi.h"
#include <stdio.h>
#include <util/delay.h>

//======== MLX90640 Functions ========

// Read 16-bit register from MLX90640 (silent - no error messages)
uint16_t MLX_read16(uint16_t reg)
{
    uint8_t hi, lo;
    uint8_t result;

    // Write register address
    result = TWI0_start((MLX_ADDR << 1) | 0);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        _delay_us(100);
        return 0xFFFF;
    }
    
    result = TWI0_write(reg >> 8);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        _delay_us(100);
        return 0xFFFF;
    }
    
    result = TWI0_write(reg & 0xFF);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        _delay_us(100);
        return 0xFFFF;
    }

    // Repeated START for read
    result = TWI0_start((MLX_ADDR << 1) | 1);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        _delay_us(100);
        return 0xFFFF;
    }
    
    hi = TWI0_read_ack();
    lo = TWI0_read_nack();
    
    _delay_us(100);

    return (hi << 8) | lo;
}

// Debug version - prints the register value
void debug_MLX_read16(uint16_t reg)
{
    uint16_t value = MLX_read16(reg);
    char buffer[30];
    snprintf(buffer, sizeof(buffer), "Reg 0x%04X: 0x%04X\r\n", reg, value);
    USART2_sendString(buffer);
}

//======== MPU6050 Functions ========

#define MPU6050_ADDR 0x68

uint8_t MPU6050_read8(uint8_t reg)
{
    uint8_t result;
    
    result = TWI0_start((MPU6050_ADDR << 1) | 0);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        return 0xFF;
    }
    
    result = TWI0_write(reg);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        return 0xFF;
    }

    result = TWI0_start((MPU6050_ADDR << 1) | 1);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        return 0xFF;
    }
    
    uint8_t data = TWI0_read_nack();
    _delay_us(100);
    return data;
}

void debug_MPU6050_read8(uint8_t reg, const char *label)
{
    uint8_t value = MPU6050_read8(reg);
    char buffer[40];
    snprintf(buffer, sizeof(buffer), "%s: 0x%02X\r\n", label, value);
    USART2_sendString(buffer);
}

//======== MLX90640 Frame Functions ========

uint8_t MLX_wait_for_data(void)
{
    uint16_t status;
    uint16_t timeout = 1000;
    
    while (timeout--) {
        status = MLX_read16(MLX_STATUS_REG);
        if (status & 0x0008) {
            return 1;
        }
        _delay_ms(1);
    }
    return 0;
}

void MLX_read_burst(uint16_t reg, uint8_t *buffer, uint16_t count)
{
    uint8_t result;
    
    result = TWI0_start((MLX_ADDR << 1) | 0);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        return;
    }
    
    TWI0_write(reg >> 8);
    TWI0_write(reg & 0xFF);
    
    result = TWI0_start((MLX_ADDR << 1) | 1);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        return;
    }
    
    for (uint16_t i = 0; i < count - 1; i++) {
        buffer[i] = TWI0_read_ack();
    }
    buffer[count - 1] = TWI0_read_nack();
    
    _delay_us(100);
}

void MLX_send_frame_to_pc(void)
{
    char buffer[20];
    uint16_t pixel_data;
    uint16_t error_count = 0;
    
    TWI0_reset_bus();
    _delay_ms(5);
    
    USART2_sendString("FRAME_START\r\n");
    
    // Read in chunks of 32 pixels (one row) for better I2C efficiency
    for (uint16_t row = 0; row < 24; row++) {
        uint16_t row_start = MLX_RAM_START + (row * 32);
        
        for (uint16_t col = 0; col < 32; col++) {
            pixel_data = MLX_read16(row_start + col);
            
            if (pixel_data == 0xFFFF) {
                error_count++;
            }
            
            snprintf(buffer, sizeof(buffer), "%04X", pixel_data);
            USART2_sendString(buffer);
            
            if (col < 31) {
                USART2_sendChar(',');
            }
        }
        USART2_sendString("\r\n");
        
        // Reset bus every 4 rows instead of every 128 pixels
        if ((row + 1) % 4 == 0) {
            TWI0_reset_bus();
            _delay_us(500);
        }
    }
    
    USART2_sendString("FRAME_END\r\n");
    
    if (error_count > 0) {
        snprintf(buffer, sizeof(buffer), "Errors: %u\r\n", error_count);
        USART2_sendString(buffer);
    }
}
