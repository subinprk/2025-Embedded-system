/*
 * MLX90640 Thermal Image Processing
 * 
 * Implements weighted centroid algorithm to find the hottest region
 * in the thermal image without requiring full KPCA (which would need
 * too much RAM for ATmega4809).
 * 
 * Memory usage: ~100 bytes (no large matrices needed)
 * Complexity: O(n) where n = 768 pixels
 */

#include "../include/mlxProcess.h"
#include "../include/mlx90640.h"
#include "../include/uart.h"
#include "../include/twi.h"
#include <stdio.h>
#include <util/delay.h>

// External function from mlxInput.c (silent read)
extern uint16_t MLX_read16(uint16_t reg);

/*
 * Find the single hottest pixel in the frame
 * Reads directly from MLX90640 RAM
 */
void MLX_find_max_pixel(uint8_t *max_x, uint8_t *max_y, uint16_t *max_value)
{
    uint16_t max_val = 0;
    uint8_t mx = 0, my = 0;
    
    TWI0_reset_bus();
    _delay_ms(5);
    
    for (uint16_t i = 0; i < MLX_TOTAL; i++) {
        uint16_t pixel = MLX_read16(MLX_RAM_START + i);
        
        // Skip error values
        if (pixel == 0xFFFF) continue;
        
        if (pixel > max_val) {
            max_val = pixel;
            mx = i % MLX_WIDTH;   // X = column (0-31)
            my = i / MLX_WIDTH;   // Y = row (0-23)
        }
        
        // Reset bus periodically to prevent lockups
        if ((i + 1) % 128 == 0) {
            TWI0_reset_bus();
            _delay_ms(1);
        }
    }
    
    *max_x = mx;
    *max_y = my;
    *max_value = max_val;
}

/*
 * Get basic statistics from the frame
 */
void MLX_get_stats(uint16_t *min_val, uint16_t *max_val, uint16_t *avg_val)
{
    uint16_t min_v = 0xFFFF;
    uint16_t max_v = 0;
    uint32_t sum = 0;
    uint16_t count = 0;
    
    TWI0_reset_bus();
    _delay_ms(5);
    
    for (uint16_t i = 0; i < MLX_TOTAL; i++) {
        uint16_t pixel = MLX_read16(MLX_RAM_START + i);
        
        // Skip error values
        if (pixel == 0xFFFF) continue;
        
        if (pixel < min_v) min_v = pixel;
        if (pixel > max_v) max_v = pixel;
        sum += pixel;
        count++;
        
        if ((i + 1) % 128 == 0) {
            TWI0_reset_bus();
            _delay_ms(1);
        }
    }
    
    *min_val = min_v;
    *max_val = max_v;
    *avg_val = (count > 0) ? (uint16_t)(sum / count) : 0;
}

/*
 * Find weighted centroid of hot pixels
 * 
 * Algorithm:
 *   centroid_x = sum(x_i * weight_i) / sum(weight_i)
 *   centroid_y = sum(y_i * weight_i) / sum(weight_i)
 * 
 * where weight_i = (pixel_value - threshold) for pixels above threshold
 * 
 * Uses fixed-point arithmetic (8.8 format) for sub-pixel precision
 * Result: x_fp/256 and y_fp/256 give actual coordinates
 */
CentroidResult MLX_find_hot_centroid(uint16_t threshold)
{
    CentroidResult result = {0, 0, 0, threshold};
    
    uint32_t sum_wx = 0;    // Sum of (x * weight)
    uint32_t sum_wy = 0;    // Sum of (y * weight)
    uint32_t sum_w = 0;     // Sum of weights
    uint16_t max_val = 0;
    
    TWI0_reset_bus();
    _delay_ms(5);
    
    for (uint16_t i = 0; i < MLX_TOTAL; i++) {
        uint16_t pixel = MLX_read16(MLX_RAM_START + i);
        
        // Skip error values
        if (pixel == 0xFFFF) continue;
        
        // Track maximum
        if (pixel > max_val) {
            max_val = pixel;
        }
        
        // Only consider pixels above threshold
        if (pixel > threshold) {
            uint8_t x = i % MLX_WIDTH;
            uint8_t y = i / MLX_WIDTH;
            
            // Weight = how much above threshold (normalized)
            uint16_t weight = pixel - threshold;
            
            // Accumulate weighted positions
            sum_wx += (uint32_t)x * weight;
            sum_wy += (uint32_t)y * weight;
            sum_w += weight;
        }
        
        if ((i + 1) % 128 == 0) {
            TWI0_reset_bus();
            _delay_ms(1);
        }
    }
    
    result.max_value = max_val;
    
    // Calculate centroid with fixed-point precision (multiply by 256 before divide)
    if (sum_w > 0) {
        result.x_fp = (uint16_t)((sum_wx * 256UL) / sum_w);
        result.y_fp = (uint16_t)((sum_wy * 256UL) / sum_w);
    } else {
        // No hot pixels found - return center of frame
        result.x_fp = (MLX_WIDTH / 2) * 256;
        result.y_fp = (MLX_HEIGHT / 2) * 256;
    }
    
    return result;
}

/*
 * Find weighted centroid using automatic threshold
 * First pass: find min/max to calculate threshold
 * Second pass: calculate centroid
 * 
 * percent: consider top X% of pixel values (e.g., 10 = top 10%)
 */
CentroidResult MLX_find_hot_centroid_auto(uint8_t percent)
{
    uint16_t min_val, max_val, avg_val;
    
    // First pass: get statistics
    MLX_get_stats(&min_val, &max_val, &avg_val);
    
    // Calculate threshold: top X% of the range
    // threshold = max - (range * percent / 100)
    uint16_t range = max_val - min_val;
    uint16_t threshold = max_val - ((uint32_t)range * percent / 100);
    
    // Second pass: calculate centroid with this threshold
    return MLX_find_hot_centroid(threshold);
}

/*
 * Process a frame and report hotspot information over UART
 */
void MLX_process_and_report(void)
{
    char buffer[80];
    
    USART2_sendString("\r\n=== Hotspot Detection ===\r\n");
    
    // Method 1: Simple max pixel
    uint8_t max_x, max_y;
    uint16_t max_val;
    MLX_find_max_pixel(&max_x, &max_y, &max_val);
    
    snprintf(buffer, sizeof(buffer), "Max pixel: (%d, %d) = %u\r\n", 
             max_x, max_y, max_val);
    USART2_sendString(buffer);
    
    // Method 2: Weighted centroid (top 20% hottest pixels)
    CentroidResult centroid = MLX_find_hot_centroid_auto(20);
    
    // Convert fixed-point to decimal for display
    // x_fp/256 gives integer part, (x_fp%256)*100/256 gives 2 decimal places
    uint8_t cx_int = centroid.x_fp / 256;
    uint8_t cx_dec = (uint8_t)(((uint16_t)(centroid.x_fp % 256) * 100) / 256);
    uint8_t cy_int = centroid.y_fp / 256;
    uint8_t cy_dec = (uint8_t)(((uint16_t)(centroid.y_fp % 256) * 100) / 256);
    
    snprintf(buffer, sizeof(buffer), "Hot centroid: (%d.%02d, %d.%02d)\r\n",
             cx_int, cx_dec, cy_int, cy_dec);
    USART2_sendString(buffer);
    
    snprintf(buffer, sizeof(buffer), "Threshold: %u, Max: %u\r\n",
             centroid.threshold, centroid.max_value);
    USART2_sendString(buffer);
    
    // Also output as raw fixed-point for Python parsing
    snprintf(buffer, sizeof(buffer), "HOTSPOT:%u,%u,%u\r\n",
             centroid.x_fp, centroid.y_fp, centroid.max_value);
    USART2_sendString(buffer);
}
