#include "../include/mpu6050.h"
#include "../include/twi.h"
#include "../include/uart.h"
#include <stdio.h>
#include <util/delay.h>

// Dynamic MPU address; default 0x68, fallback to 0x69 if AD0=HIGH
static uint8_t mpu_addr = MPU6050_ADDR;
static uint8_t mpu_try_ack(uint8_t addr)
{
    uint8_t ok = TWI0_start((addr << 1) | 0);
    TWI0_stop();
    return ok;
}

void MPU6050_init(void)
{
    char buf[60];
    USART2_sendString("[MPU] Initializing MPU6050...\r\n");
    
    // Ensure bus is clear before init
    TWI0_clock_pulse_stop();
    _delay_ms(10);
    
    // Determine address (handles AD0 floating/high)
    if (mpu_try_ack(0x68)) {
        mpu_addr = 0x68;
    } else if (mpu_try_ack(0x69)) {
        mpu_addr = 0x69;
    }

    for (uint8_t attempt = 0; attempt < 5; attempt++) {
        USART2_sendString("[MPU] Attempting wake-up...\r\n");
        
        // Wake up MPU6050 (exit sleep mode)
        uint8_t result = TWI0_start((mpu_addr << 1) | 0);
        if (!result) {
            snprintf(buf, sizeof(buf), "[MPU] Init START failed (att=%d), recovering...\r\n", attempt);
            USART2_sendString(buf);
            TWI0_stop();
            TWI0_clock_pulse_stop();
            _delay_ms(100);
            continue;
        }
        
        result = TWI0_write(MPU6050_PWR_MGMT_1);
        if (!result) {
            snprintf(buf, sizeof(buf), "[MPU] Init WRITE reg failed (att=%d)\r\n", attempt);
            USART2_sendString(buf);
            TWI0_stop();
            TWI0_clock_pulse_stop();
            _delay_ms(50);
            continue;
        }
        
        result = TWI0_write(0x00);  // Clear sleep bit
        if (!result) {
            snprintf(buf, sizeof(buf), "[MPU] Init WRITE data failed (att=%d)\r\n", attempt);
            USART2_sendString(buf);
            TWI0_stop();
            TWI0_clock_pulse_stop();
            _delay_ms(50);
            continue;
        }
        
        TWI0_stop();
        _delay_ms(100);
        
        // Additional configuration to prevent auto-sleep
        // Set sample rate divider (optional, keeps device active)
        result = TWI0_start((mpu_addr << 1) | 0);
        if (result) {
            TWI0_write(0x19);  // SMPLRT_DIV register
            TWI0_write(0x07);  // Sample rate = 1kHz / (1 + 7) = 125Hz
            TWI0_stop();
            _delay_ms(10);
        }
        
        // Configure accelerometer (keeps it awake)
        result = TWI0_start((mpu_addr << 1) | 0);
        if (result) {
            TWI0_write(0x1C);  // ACCEL_CONFIG register
            TWI0_write(0x00);  // ±2g range
            TWI0_stop();
            _delay_ms(10);
        }
        
        // Configure gyroscope (keeps it awake)
        result = TWI0_start((mpu_addr << 1) | 0);
        if (result) {
            TWI0_write(0x1B);  // GYRO_CONFIG register
            TWI0_write(0x00);  // ±250°/s range
            TWI0_stop();
            _delay_ms(10);
        }
        
        // Verify initialization by reading WHO_AM_I
        uint8_t who = MPU6050_read8(MPU6050_WHO_AM_I);
        snprintf(buf, sizeof(buf), "[MPU] Init verify: WHO_AM_I=0x%02X\r\n", who);
        USART2_sendString(buf);
        
        if (who == 0x68) {
            USART2_sendString("[MPU] Initialization SUCCESS!\r\n");
            return;
        }
        
        snprintf(buf, sizeof(buf), "[MPU] Init failed (att=%d), retrying...\r\n", attempt);
        USART2_sendString(buf);
        TWI0_clock_pulse_stop();
        _delay_ms(100);
    }
    
    USART2_sendString("[MPU] Initialization FAILED after 5 attempts!\r\n");
}

uint8_t MPU6050_test_connection(void)
{
    // Reconfirm address if current one doesn't ACK
    if (!mpu_try_ack(mpu_addr)) {
        if (mpu_try_ack(0x68)) mpu_addr = 0x68;
        else if (mpu_try_ack(0x69)) mpu_addr = 0x69;
    }
    uint8_t who_am_i = MPU6050_read8(MPU6050_WHO_AM_I);
    return (who_am_i == 0x68);
}

void MPU6050_read_accel(int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t data[6];
    
    // Read 6 bytes starting from ACCEL_XOUT_H
    TWI0_start((mpu_addr << 1) | 0);
    TWI0_write(MPU6050_ACCEL_XOUT_H);
    TWI0_start((mpu_addr << 1) | 1);
    
    for (uint8_t i = 0; i < 5; i++) {
        data[i] = TWI0_read_ack();
    }
    data[5] = TWI0_read_nack();
    
    *ax = (int16_t)((data[0] << 8) | data[1]);
    *ay = (int16_t)((data[2] << 8) | data[3]);
    *az = (int16_t)((data[4] << 8) | data[5]);
}

void MPU6050_read_gyro(int16_t *gx, int16_t *gy, int16_t *gz)
{
    uint8_t data[6];
    
    // Read 6 bytes starting from GYRO_XOUT_H
    TWI0_start((mpu_addr << 1) | 0);
    TWI0_write(MPU6050_GYRO_XOUT_H);
    TWI0_start((mpu_addr << 1) | 1);
    
    for (uint8_t i = 0; i < 5; i++) {
        data[i] = TWI0_read_ack();
    }
    data[5] = TWI0_read_nack();
    
    *gx = (int16_t)((data[0] << 8) | data[1]);
    *gy = (int16_t)((data[2] << 8) | data[3]);
    *gz = (int16_t)((data[4] << 8) | data[5]);
}

void MPU6050_debug_test(void)
{
    char buffer[60];
    int16_t ax, ay, az, gx, gy, gz;
    
    USART2_sendString("\r\n=== MPU6050 Test ===\r\n");
    
    if (MPU6050_test_connection()) {
        USART2_sendString("MPU6050 connected! (WHO_AM_I = 0x68)\r\n");
        
        MPU6050_read_accel(&ax, &ay, &az);
        snprintf(buffer, sizeof(buffer), "Accel: X=%d Y=%d Z=%d\r\n", ax, ay, az);
        USART2_sendString(buffer);
        
        MPU6050_read_gyro(&gx, &gy, &gz);
        snprintf(buffer, sizeof(buffer), "Gyro:  X=%d Y=%d Z=%d\r\n", gx, gy, gz);
        USART2_sendString(buffer);
    } else {
        USART2_sendString("MPU6050 NOT FOUND!\r\n");
    }
}

//======== MPU6050 Functions ========

#define MPU6050_ADDR 0x68

uint8_t MPU6050_read8(uint8_t reg)
{
    for (uint8_t attempt = 0; attempt < 3; attempt++) {
        uint8_t result;
        
        // Check for bus errors that indicate stuck slave
        uint8_t busstate = TWI0.MSTATUS & TWI_BUSSTATE_gm;
        if (busstate == TWI_BUSSTATE_BUSY_gc || busstate == TWI_BUSSTATE_UNKNOWN_gc) {
            char buf[50];
            snprintf(buf, sizeof(buf), "[MPU] BUS STUCK (0x%02X), recovering...\r\n", TWI0.MSTATUS);
            USART2_sendString(buf);
            TWI0_clock_pulse_stop();
            _delay_ms(2);
        }

        result = TWI0_start((mpu_addr << 1) | 0);
        if (!result) {
            char buf[60];
            snprintf(buf, sizeof(buf), "[MPU] START failed (att=%d, MSTATUS=0x%02X)\r\n", attempt, TWI0.MSTATUS);
            USART2_sendString(buf);
            TWI0_stop();
            if (attempt < 2) {
                TWI0_clock_pulse_stop();
                _delay_ms(2);
            }
            continue;
        }
        
        result = TWI0_write(reg);
        if (!result) {
            char buf[60];
            snprintf(buf, sizeof(buf), "[MPU] WRITE reg failed (att=%d, MSTATUS=0x%02X)\r\n", attempt, TWI0.MSTATUS);
            USART2_sendString(buf);
            TWI0_stop();
            if (attempt < 2) {
                TWI0_clock_pulse_stop();
                _delay_ms(2);
            }
            continue;
        }

        result = TWI0_start((mpu_addr << 1) | 1);
        if (!result) {
            char buf[60];
            snprintf(buf, sizeof(buf), "[MPU] START (read) failed (att=%d, MSTATUS=0x%02X)\r\n", attempt, TWI0.MSTATUS);
            USART2_sendString(buf);
            TWI0_stop();
            if (attempt < 2) {
                TWI0_clock_pulse_stop();
                _delay_ms(2);
            }
            continue;
        }
        
        uint8_t data = TWI0_read_nack();
        _delay_us(100);
        char buf[40];
        snprintf(buf, sizeof(buf), "[MPU] READ OK (att=%d, data=0x%02X)\r\n", attempt, data);
        USART2_sendString(buf);
        return data;
    }

    USART2_sendString("[MPU] READ8 FAILED after 3 attempts\r\n");
    return 0xFF;
}

void debug_MPU6050_read8(uint8_t reg, const char *label)
{
    uint8_t value = MPU6050_read8(reg);
    char buffer[40];
    snprintf(buffer, sizeof(buffer), "%s: 0x%02X\r\n", label, value);
    USART2_sendString(buffer);
}
