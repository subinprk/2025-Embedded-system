#ifndef MLX90640_H
#define MLX90640_H
#include <avr/io.h>

#define MLX_ADDR 0x33

//======== MLX90640 function prototypes ========
uint16_t MLX_read16(uint16_t reg);
void debug_MLX_read16(uint16_t reg);

//======== MPU6050 function prototypes ========
uint8_t MPU6050_read8(uint8_t reg);
void debug_MPU6050_read8(uint8_t reg, const char *label);

#endif // MLX90640_H