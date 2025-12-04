#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "../include/uart.h"
#include "../include/twi.h"
#include "../include/mlx90640.h"
#include "../include/mpu6050.h"
#include "../include/mlxProcess.h"

// Function to set CPU clock to maximum speed (16MHz or 20MHz depending on fuse)
void clock_init(void)
{
    // To change protected registers, we need to write to CCP first
    // Then we have 4 clock cycles to write the actual register
    
    // Set prescaler to /1 (no division) for maximum speed
    // Your chip has 16MHz base oscillator (based on earlier UART testing)
    // This will give you 16MHz CPU clock instead of 2.67MHz
    
    CCP = CCP_IOREG_gc;              // Unlock protected register
    CLKCTRL.MCLKCTRLB = 0x00;        // Disable prescaler (PEN = 0)
    
    // Wait for clock to stabilize
    while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm);
}

int main(void)
{
    // Set CPU clock to 16MHz (disable /6 prescaler)
    clock_init();
    _delay_ms(100); 
    USART2_init();
    TWI0_init();
    _delay_ms(100);
    
    // LED toggle test (PF5 as LED)
    PORTF.DIRSET = PIN5_bm;
    
    // Send startup message
    USART2_sendString("\r\n");
    USART2_sendString("================================\r\n");
    USART2_sendString("ATmega4809 @ 16MHz - I2C Test\r\n");
    USART2_sendString("================================\r\n");
    
    // I2C Bus scan
    USART2_sendString("\r\nScanning I2C bus...\r\n");
    TWI0_scan();
    
    // Try to read MLX90640 ID
    USART2_sendString("\r\nMLX90640 Device ID test:\r\n");
    debug_MLX_read16(0x2407);
    
    // Initialize MPU6050
    USART2_sendString("\r\nInitializing MPU6050...\r\n");
    MPU6050_init();
    _delay_ms(100);
    
    // Test read MPU6050
    USART2_sendString("MPU6050 WHO_AM_I:\r\n");
    debug_MPU6050_read8(0x75, "WHO_AM_I");
    
    USART2_sendString("\r\n--- Starting main loop ---\r\n");
    
    uint8_t loop_count = 0;
    
    while (1)
    {
        loop_count++;
        char buf[40];
        snprintf(buf, sizeof(buf), "\r\n--- Loop %d ---\r\n", loop_count);
        USART2_sendString(buf);
        
        // Reset bus before each cycle
        TWI0_reset_bus();
        _delay_ms(50);
        
        // Process thermal image and find hotspot
        MLX_process_and_report();
        
        // MPU6050 quick check
        USART2_sendString("\r\nMPU6050: ");
        uint8_t who = MPU6050_read8(0x75);
        snprintf(buf, sizeof(buf), "WHO_AM_I=0x%02X\r\n", who);
        USART2_sendString(buf);

        PORTF.OUTTGL = PIN5_bm;  // LED toggle
        _delay_ms(3000);  // 3 second delay between readings
    }
}