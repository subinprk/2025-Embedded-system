#ifndef MPU6050_H
#define MPU6050_H

#include <avr/io.h>

#define MPU6050_ADDR 0x68

// MPU6050 Registers
#define MPU6050_WHO_AM_I   0x75
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_GYRO_XOUT_H  0x43

// Function prototypes
void MPU6050_init(void);
uint8_t MPU6050_test_connection(void);
void MPU6050_read_accel(int16_t *ax, int16_t *ay, int16_t *az);
void MPU6050_read_gyro(int16_t *gx, int16_t *gy, int16_t *gz);
void MPU6050_debug_test(void);
uint8_t MPU6050_read8(uint8_t reg);
void debug_MPU6050_read8(uint8_t reg, const char *label);

#endif // MPU6050_H
