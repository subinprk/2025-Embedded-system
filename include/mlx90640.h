#ifndef MLX90640_H
#define MLX90640_H
#include <avr/io.h>

#define MLX_ADDR 0x33

// MLX90640 RAM addresses
#define MLX_RAM_START    0x0400  // Pixel data starts here
#define MLX_RAM_END      0x06FF  // End of pixel RAM
#define MLX_PIXELS       768     // 32x24 = 768 pixels

// MLX90640 EEPROM addresses
#define MLX_EEPROM_START 0x2400

// Status/Control registers  
#define MLX_STATUS_REG   0x8000
#define MLX_CTRL_REG1    0x800D

//======== MLX90640 function prototypes ========
uint16_t MLX_read16(uint16_t reg);
void debug_MLX_read16(uint16_t reg);
void MLX_read_frame(uint16_t *buffer);      // Read one subpage (384 pixels)
void MLX_send_frame_to_pc(void);            // Send raw frame data over UART
uint8_t MLX_wait_for_data(void);            // Wait for new frame ready

//======== MPU6050 function prototypes ========
uint8_t MPU6050_read8(uint8_t reg);
void debug_MPU6050_read8(uint8_t reg, const char *label);

#endif // MLX90640_H