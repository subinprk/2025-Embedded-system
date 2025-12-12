#ifndef FLAGS_H
#define FLAGS_H

// Cooperative scheduler flags (set in timer ISR, serviced in main loop)
typedef struct {
    volatile bool mlx_due;   // Read/process MLX90640 frame
    volatile bool mpu_due;   // Read MPU6050 accel/gyro
    volatile bool pwm_due;   // Update motor drive pattern
    volatile bool led_due;   // Heartbeat LED toggle
} SchedulerFlags;

extern SchedulerFlags sched_flags;  // Declared here, defined in interrupt.c

// MLX incremental acquisition state
typedef enum {
    MLX_STATE_IDLE = 0,
    MLX_STATE_WAIT_READY,
    MLX_STATE_READ_ROWS,
    MLX_STATE_PROCESS
} MlxState;

#endif // FLAGS_H