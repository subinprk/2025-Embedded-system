#include "../include/interrupt.h"
#include <util/delay.h>
#include <stdbool.h>

// Definition of the global scheduler flags (declared extern in flags.h)
SchedulerFlags sched_flags = {0};

static struct {
    MlxState state;
    uint8_t current_row;
    uint8_t active_idx;   // buffer being filled
    uint8_t ready_idx;    // buffer ready for processing
    bool frame_ready;     // true when a full frame is ready to process
    uint16_t frame[2][MLX_TOTAL]; // double buffer: ping-pong
} mlx_ctx = { .state = MLX_STATE_WAIT_READY, .current_row = 0, .active_idx = 0, .ready_idx = 0, .frame_ready = false, .frame = {{0}} };

// Send buffered frame to PC (no I2C)
static void MLX_send_buffer_to_pc(uint16_t *frame)
{
    char buffer[20];
    USART2_sendString("FRAME_START\r\n");
    for (uint16_t row = 0; row < MLX_HEIGHT; row++) {
        for (uint16_t col = 0; col < MLX_WIDTH; col++) {
            uint16_t px = frame[(row * MLX_WIDTH) + col];
            snprintf(buffer, sizeof(buffer), "%04X", px);
            USART2_sendString(buffer);
            if (col < (MLX_WIDTH - 1)) {
                USART2_sendChar(',');
            }
        }
        USART2_sendString("\r\n");
    }
    USART2_sendString("FRAME_END\r\n");
}

// Use TCB0 for 1 kHz tick (simpler than TCA0)
void timer_init_1khz(void)
{
    // TCB0 in Periodic Interrupt mode
    // For ATmega4809: CNTMODE[2:0] in CTRLB
    // 0b000 = INT mode (Periodic Interrupt) - this IS the default and correct mode
    // 
    // CLK_PER/2 = 8 MHz
    // CCMP = 7999 gives 1 kHz (8000000 / 8000 = 1000 Hz)
    // Enable routing of TCB0 interrupt to CPU
    // PORTMUX.TCBROUTEA |= PORTMUX_TCB0_bm;
    TCB0.CTRLA = 0;  // Disable first
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;  // Periodic Interrupt mode (CNTMODE = 0b000)
    TCB0.CCMP = 7999;
    TCB0.CNT = 0;  // Clear counter
    TCB0.INTFLAGS = TCB_CAPT_bm;  // Clear any pending interrupt
    *((volatile uint8_t*)&TCB0.INTCTRL) = TCB_CAPT_bm; // Enable capture/compare interrupt
    TCB0.CTRLA = (0x01 << 1) | TCB_ENABLE_bm;  // CLKSEL=01 (CLK_PER/2), ENABLE=1

}

ISR(TCB0_INT_vect)
{
    static uint16_t ms_counter = 0;
    static uint8_t isr_announced = 0;
    TCB0.INTFLAGS = TCB_CAPT_bm; // Clear flag

    ms_counter++;

    // One-time ISR confirmation
    if (!isr_announced) {
        isr_announced = 1;
        PORTF.OUTTGL = PIN5_bm; // Quick LED toggle to confirm ISR runs
    }
    if (ms_counter > 1000) {
        ms_counter = 0; 
    }
    // 50 ms: MPU poll
    if ((ms_counter % 50) == 0) {
        sched_flags.mpu_due = true;
    }
    // 20 ms: MLX service (tighter loop for faster capture)
    if ((ms_counter % 20) == 0) {
        sched_flags.mlx_due = true;
    }
    // 20 ms: motor pattern step
    if ((ms_counter % 20) == 0) {
        sched_flags.pwm_due = true;
    }
    // 500 ms: LED heartbeat
    if ((ms_counter % 500) == 0) {
        sched_flags.led_due = true;
    }
}

// Non-blocking task handlers (called from main loop)
static void task_mlx_frame(void)
{
    sched_flags.mlx_due = false;

    // If a completed frame exists and bus is idle, process it while next frame is being prepared
    if (mlx_ctx.frame_ready && mlx_ctx.state != MLX_STATE_READ_ROWS) {
        MLX_process_buffer_and_report(mlx_ctx.frame[mlx_ctx.ready_idx]);
        // MLX_send_buffer_to_pc(mlx_ctx.frame[mlx_ctx.ready_idx]);
        mlx_ctx.frame_ready = false;
    }

    switch (mlx_ctx.state) {
        case MLX_STATE_WAIT_READY:
            // Check once; if ready, start incremental read
            if (MLX_poll_data_ready()) {
                mlx_ctx.state = MLX_STATE_READ_ROWS;
                mlx_ctx.current_row = 0;
                // Toggle to the other buffer for ping-pong
                mlx_ctx.active_idx ^= 1;
            }
            break;

        case MLX_STATE_READ_ROWS:
            // Read multiple rows per service call for faster capture
            for (uint8_t i = 0; i < 12 && mlx_ctx.current_row < MLX_HEIGHT; i++) {
                MLX_read_row(mlx_ctx.current_row,
                    &mlx_ctx.frame[mlx_ctx.active_idx][(uint16_t)mlx_ctx.current_row * MLX_WIDTH]);
                mlx_ctx.current_row++;
            }

            // Reset bus every 12 rows to avoid lockups (short wait only)
            if ((mlx_ctx.current_row % 12) == 0) {
                TWI0_reset_bus();
                _delay_us(100);
            }

            if (mlx_ctx.current_row >= MLX_HEIGHT) {
                // Mark buffer ready and return to WAIT_READY to pipeline next capture
                mlx_ctx.ready_idx = mlx_ctx.active_idx;
                mlx_ctx.frame_ready = true;
                mlx_ctx.state = MLX_STATE_WAIT_READY;
            }
            break;

        case MLX_STATE_IDLE:
        default:
            mlx_ctx.state = MLX_STATE_WAIT_READY;
            break;
    }

    // Keep rescheduling until we return to WAIT_READY and no pending work
    if (mlx_ctx.state != MLX_STATE_WAIT_READY) {
        sched_flags.mlx_due = true;
    }
}

static void task_mpu_sample(void)
{
    sched_flags.mpu_due = false;

    int16_t ax, ay, az, gx, gy, gz;
    char buf[80];

    MPU6050_read_accel(&ax, &ay, &az);
    MPU6050_read_gyro(&gx, &gy, &gz);

    // Minimal debug print; adjust as needed for bandwidth
    snprintf(buf, sizeof(buf), "MPU A:%d,%d,%d G:%d,%d,%d\r\n", ax, ay, az, gx, gy, gz);
    USART2_sendString(buf);
}


static void task_led_toggle(void)
{
    sched_flags.led_due = false;
    PORTF.OUTTGL = PIN5_bm;
}

void scheduler_init(void)
{
    sched_flags.mlx_due = false;
    sched_flags.mpu_due = false;
    sched_flags.pwm_due = false;
    sched_flags.led_due = false;

    mlx_ctx.state = MLX_STATE_WAIT_READY;
    mlx_ctx.current_row = 0;
    /*
     * Configure CPU Interrupt Controller:
     * - Clear CTRLA to use application vector table (IVSEL=0)
     * - Allow level-0 interrupts by setting LVL0PRI (enable all)
     * - Enable optional round-robin scheduling for level-0
     */
    CPUINT.CTRLA = 0x00;            // ensure IVSEL/CVT cleared
    CPUINT.LVL0PRI = 0xFF;          // allow level-0 servicing for all groups
    CPUINT.CTRLA |= CPUINT_LVL0RR_bm; // enable round-robin for level-0

    // timer_init_1khz();
    sei();
}

void scheduler_service_tasks(void)
{
    if (sched_flags.led_due) {
        USART2_sendString("[LED]\r\n");
        task_led_toggle();
        USART2_sendString("~\r\n");
    }
    if (sched_flags.pwm_due) {
        USART2_sendString("[PWM]\r\n");
        task_pwm_update();
    }
    // Re-enable MPU sampling only (keep MLX disabled for now)
    if (sched_flags.mpu_due) {
        USART2_sendString("[MPU]\r\n");
        task_mpu_sample();
    }
    // Keep MLX disabled for isolation
    if (sched_flags.mlx_due) {
        USART2_sendString("[MLX]\r\n");
        task_mlx_frame();
    }
    sched_flags.mpu_due = false;
    sched_flags.mlx_due = false;
}