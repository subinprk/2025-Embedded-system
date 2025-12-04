#include <avr/io.h>
#include <util/delay.h>
#include "../include/uart.h"
#include "../include/twi.h"
#include "../include/mlx90640.h"
#include "../include/mpu6050.h"

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
    USART2_sendString("ATmega4809 running at 16MHz!\r\n");
    
    USART2_sendString("I2C Bus Status:\r\n");
    TWI0_debug_status();
    TWI0_scan();
    
    // Initialize MPU6050 (wake it up from sleep mode)
    USART2_sendString("\r\nInitializing MPU6050...\r\n");
    MPU6050_init();
    _delay_ms(100);
    
    while (1)
    {
        // Read MLX90640 Device ID
        USART2_sendString("\r\n=== MLX90640 ===\r\n");
        USART2_sendString("Device ID: ");
        debug_MLX_read16(0x2407);

        //========MPU6050 Read Test========//
        USART2_sendString("\r\n=== MPU6050 ===\r\n");
        debug_MPU6050_read8(0x75, "WHO_AM_I");   // Should be 0x68
        debug_MPU6050_read8(0x6B, "PWR_MGMT_1"); // Should be 0x00 after init
        debug_MPU6050_read8(0x3B, "ACCEL_X_H");  // Now should show real data!

        PORTF.OUTTGL = PIN5_bm;  // LED toggle
        _delay_ms(1000);
    }
}