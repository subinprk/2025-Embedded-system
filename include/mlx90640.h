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

// Frame rate options (bits 10:7 in CTRL_REG1)
#define MLX_FRAMERATE_0_5HZ   0  // 0.5 Hz
#define MLX_FRAMERATE_1HZ     1  // 1 Hz
#define MLX_FRAMERATE_2HZ     2  // 2 Hz (default)
#define MLX_FRAMERATE_4HZ     3  // 4 Hz
#define MLX_FRAMERATE_8HZ     4  // 8 Hz
#define MLX_FRAMERATE_16HZ    5  // 16 Hz
#define MLX_FRAMERATE_32HZ    6  // 32 Hz
#define MLX_FRAMERATE_64HZ    7  // 64 Hz

//======== MLX90640 function prototypes ========
uint16_t MLX_read16(uint16_t reg);
void debug_MLX_read16(uint16_t reg);
void MLX_set_framerate(uint8_t rate);       // Set frame rate (use MLX_FRAMERATE_* constants)
void MLX_read_frame(uint16_t *buffer);      // Read one subpage (384 pixels)
void MLX_send_frame_to_pc(void);            // Send raw frame data over UART
uint8_t MLX_wait_for_data(void);            // Wait for new frame ready
uint8_t MLX_poll_data_ready(void);          // Non-blocking ready check
void MLX_read_row(uint8_t row_index, uint16_t *dest); // Read 32-pixel row into dest

#endif // MLX90640_H