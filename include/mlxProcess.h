#ifndef MLX_PROCESS_H
#define MLX_PROCESS_H

#include <avr/io.h>
#include <stdint.h>

// MLX90640 image dimensions
#define MLX_WIDTH   32
#define MLX_HEIGHT  24
#define MLX_TOTAL   768  // 32 * 24

// Result structure for hotspot detection
typedef struct {
    uint8_t x;              // X coordinate (0-31)
    uint8_t y;              // Y coordinate (0-23)
    uint16_t max_value;     // Maximum pixel value found
    uint16_t avg_value;     // Average value of hot region
    uint8_t hot_pixel_count; // Number of pixels above threshold
} HotspotResult;

// Centroid result with sub-pixel precision (fixed-point: value/256)
typedef struct {
    uint16_t x_fp;          // X centroid * 256 (for sub-pixel precision)
    uint16_t y_fp;          // Y centroid * 256
    uint16_t max_value;     // Maximum pixel value
    uint16_t threshold;     // Threshold used
} CentroidResult;

//======== Processing Functions ========

// Find the single hottest pixel location
void MLX_find_max_pixel(uint8_t *max_x, uint8_t *max_y, uint16_t *max_value);

// Find weighted centroid of hot pixels (above threshold)
// threshold: minimum value to consider as "hot"
// Returns centroid coordinates with sub-pixel precision
CentroidResult MLX_find_hot_centroid(uint16_t threshold);

// Find weighted centroid using automatic threshold (top N% of pixels)
// percent: top percentage to consider (e.g., 10 = top 10% hottest)
CentroidResult MLX_find_hot_centroid_auto(uint8_t percent);

// Process frame and send hotspot info over UART
void MLX_process_and_report(void);

// Get simple statistics from current frame
void MLX_get_stats(uint16_t *min_val, uint16_t *max_val, uint16_t *avg_val);

#endif // MLX_PROCESS_H
