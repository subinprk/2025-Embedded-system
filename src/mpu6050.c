#include "../include/mpu6050.h"
#include "../include/twi.h"
#include "../include/uart.h"
#include <stdio.h>
#include <util/delay.h>

void MPU6050_init(void)
{
    // Wake up MPU6050 (exit sleep mode)
    TWI0_start((MPU6050_ADDR << 1) | 0);
    TWI0_write(MPU6050_PWR_MGMT_1);
    TWI0_write(0x00);  // Clear sleep bit
    TWI0_stop();
    _delay_ms(100);
}

uint8_t MPU6050_test_connection(void)
{
    uint8_t who_am_i;
    
    // Read WHO_AM_I register
    TWI0_start((MPU6050_ADDR << 1) | 0);
    TWI0_write(MPU6050_WHO_AM_I);
    TWI0_start((MPU6050_ADDR << 1) | 1);
    who_am_i = TWI0_read_nack();
    
    return (who_am_i == 0x68);
}

void MPU6050_read_accel(int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t data[6];
    
    // Read 6 bytes starting from ACCEL_XOUT_H
    TWI0_start((MPU6050_ADDR << 1) | 0);
    TWI0_write(MPU6050_ACCEL_XOUT_H);
    TWI0_start((MPU6050_ADDR << 1) | 1);
    
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
    TWI0_start((MPU6050_ADDR << 1) | 0);
    TWI0_write(MPU6050_GYRO_XOUT_H);
    TWI0_start((MPU6050_ADDR << 1) | 1);
    
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
