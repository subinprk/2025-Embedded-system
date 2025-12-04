#include "../include/mlx90640.h"
#include "../include/uart.h"
#include "../include/twi.h"
#include <stdio.h>
#include <util/delay.h>

// Silent read - no debug output (used during frame capture)
static uint16_t MLX_read16_silent(uint16_t reg)
{
    uint8_t hi, lo;
    uint8_t result;

    // Write register address
    result = TWI0_start((MLX_ADDR << 1) | 0);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        return 0xFFFF;
    }
    
    result = TWI0_write(reg >> 8);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        return 0xFFFF;
    }
    
    result = TWI0_write(reg & 0xFF);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        return 0xFFFF;
    }

    // NO STOP - use Repeated START for read
    result = TWI0_start((MLX_ADDR << 1) | 1);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        return 0xFFFF;
    }
    
    hi = TWI0_read_ack();
    lo = TWI0_read_nack();
    
    _delay_us(50);

    return (hi << 8) | lo;
}

// Normal read with debug output
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

//======== MLX90640 Frame Functions ========

// Wait for new data to be ready (check status register)
uint8_t MLX_wait_for_data(void)
{
    uint16_t status;
    uint16_t timeout = 1000;
    
    while (timeout--) {
        status = MLX_read16(MLX_STATUS_REG);
        if (status & 0x0008) {  // Bit 3 = new data available
            // Clear the flag by writing 1 to bit 3
            // (For now, just return success)
            return 1;
        }
        _delay_ms(1);
    }
    return 0;  // Timeout
}

// Read multiple bytes from MLX90640 (burst read)
void MLX_read_burst(uint16_t reg, uint8_t *buffer, uint16_t count)
{
    uint8_t result;
    
    // Write register address
    result = TWI0_start((MLX_ADDR << 1) | 0);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        return;
    }
    
    TWI0_write(reg >> 8);
    TWI0_write(reg & 0xFF);
    
    // Repeated START for read
    result = TWI0_start((MLX_ADDR << 1) | 1);
    if (!result) {
        TWI0_stop();
        TWI0_reset_bus();
        return;
    }
    
    // Read all bytes
    for (uint16_t i = 0; i < count - 1; i++) {
        buffer[i] = TWI0_read_ack();
    }
    buffer[count - 1] = TWI0_read_nack();
    
    _delay_us(100);
}

// Send raw frame data to PC in a format Python can parse
// Format: "FRAME_START\n" + 768 hex values + "FRAME_END\n"
void MLX_send_frame_to_pc(void)
{
    char buffer[20];
    uint16_t pixel_data;
    uint8_t error_count = 0;
    
    // Reset bus before starting frame read
    TWI0_reset_bus();
    _delay_ms(10);
    
    USART2_sendString("FRAME_START\r\n");
    
    // Read 768 pixels from RAM (32x24)
    // MLX90640 stores data as 16-bit values starting at 0x0400
    for (uint16_t i = 0; i < 768; i++) {
        pixel_data = MLX_read16_silent(MLX_RAM_START + i);
        
        // Check for read error (0xFFFF typically means failure)
        if (pixel_data == 0xFFFF) {
            error_count++;
            // Try to recover bus
            TWI0_reset_bus();
            _delay_ms(1);
        }
        
        // Send as hex value with comma separator
        snprintf(buffer, sizeof(buffer), "%04X", pixel_data);
        USART2_sendString(buffer);
        
        // Add comma except for last value
        if (i < 767) {
            USART2_sendChar(',');
        }
        
        // Newline every 32 values (one row)
        if ((i + 1) % 32 == 0) {
            USART2_sendString("\r\n");
            // Small delay after each row to let bus stabilize
            _delay_us(500);
        }
        
        // Reset bus periodically to prevent lockups
        if ((i + 1) % 128 == 0) {
            TWI0_reset_bus();
            _delay_ms(2);
        }
    }
    
    USART2_sendString("FRAME_END\r\n");
    
    // Report error count
    if (error_count > 0) {
        snprintf(buffer, sizeof(buffer), "Errors: %d\r\n", error_count);
        USART2_sendString(buffer);
    }
}