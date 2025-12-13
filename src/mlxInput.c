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
    
    _delay_us(50);  // Reduced from 100us for faster reads

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

// Non-blocking poll: returns 1 if new frame ready, 0 otherwise
uint8_t MLX_poll_data_ready(void)
{
    uint16_t status = MLX_read16(MLX_STATUS_REG);
    return (status & 0x0008) ? 1 : 0;
}

// Set MLX90640 frame rate
// rate: 0=0.5Hz, 1=1Hz, 2=2Hz, 3=4Hz, 4=8Hz, 5=16Hz, 6=32Hz, 7=64Hz
void MLX_set_framerate(uint8_t rate)
{
    if (rate > 7) rate = 7;  // Clamp to valid range
    
    // Read current control register
    uint16_t ctrl = MLX_read16(MLX_CTRL_REG1);
    
    // Clear bits 10:7 and set new rate
    ctrl &= ~(0x7 << 7);  // Clear bits 10:7
    ctrl |= (rate << 7);  // Set new rate
    
    // Write back to control register
    uint8_t result = TWI0_start((MLX_ADDR << 1) | 0);
    if (!result) {
        TWI0_stop();
        return;
    }
    
    TWI0_write(MLX_CTRL_REG1 >> 8);
    TWI0_write(MLX_CTRL_REG1 & 0xFF);
    TWI0_write(ctrl >> 8);
    TWI0_write(ctrl & 0xFF);
    TWI0_stop();
    
    _delay_ms(10);  // Wait for setting to take effect
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
    
    _delay_us(50);  // Reduced from 100us for faster burst reads
}

// Read one MLX row (32 pixels) into dest[32]
void MLX_read_row(uint8_t row_index, uint16_t *dest)
{
    uint16_t start_reg = MLX_RAM_START + ((uint16_t)row_index * 32);
    uint8_t raw[64];

    MLX_read_burst(start_reg, raw, sizeof(raw));

    for (uint8_t i = 0; i < 32; i++) {
        dest[i] = ((uint16_t)raw[i * 2] << 8) | raw[(i * 2) + 1];
    }
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
    // After completing the MLX frame transfer, perform a light I2C recovery
    // to ensure the bus is released by any stuck slave devices.
    TWI0_clock_pulse_stop();
    
    if (error_count > 0) {
        snprintf(buffer, sizeof(buffer), "Errors: %u\r\n", error_count);
        USART2_sendString(buffer);
    }
}
