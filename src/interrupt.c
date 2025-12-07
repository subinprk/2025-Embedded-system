#include "../include/interrupt.h"

// Definition of the global scheduler flags (declared extern in flags.h)
SchedulerFlags sched_flags = {0};

static struct {
    MlxState state;
    uint8_t current_row;
    uint16_t frame[MLX_TOTAL];
} mlx_ctx = { .state = MLX_STATE_WAIT_READY, .current_row = 0, .frame = {0} };

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
    PORTMUX.TCBROUTEA |= PORTMUX_TCB0_bm;
    TCB0.CTRLA = 0;  // Disable first
    TCB0.CTRLB = 0x00;  // Periodic Interrupt mode (CNTMODE = 0b000)
    TCB0.CCMP = 7999;
    TCB0.CNT = 0;  // Clear counter
    TCB0.INTFLAGS = TCB_CAPT_bm;  // Clear any pending interrupt
    *((volatile uint8_t*)&TCB0.INTCTRL) = TCB_CAPT_bm; // Enable capture/compare interrupt
    TCB0.CTRLA = (0x01 << 1) | TCB_ENABLE_bm;  // CLKSEL=01 (CLK_PER/2), ENABLE=1
    
    // Debug: confirm register values
    char buf[80];
    snprintf(buf, sizeof(buf), "TCB0: CTRLA=%02X CTRLB=%02X INTCTRL=%02X CCMP=%u CNT=%u\r\n", 
        TCB0.CTRLA, TCB0.CTRLB, TCB0.INTCTRL, TCB0.CCMP, TCB0.CNT);
    USART2_sendString(buf);
}

// ISR(TCB0_INT_vect)
// {
//     static uint16_t ms_counter = 0;
//     static uint8_t isr_announced = 0;
//     TCB0.INTFLAGS = TCB_CAPT_bm; // Clear flag

//     ms_counter++;

//     // One-time ISR confirmation
//     if (!isr_announced) {
//         isr_announced = 1;
//         PORTF.OUTTGL = PIN5_bm; // Quick LED toggle to confirm ISR runs
//     }
//     if (ms_counter > 1000) {
//         ms_counter = 0; 
//     }
//     // 50 ms: MPU poll
//     if ((ms_counter % 50) == 0) {
//         sched_flags.mpu_due = true;
//     }
//     // 200 ms: MLX frame handling (avoid swamping I2C)
//     if ((ms_counter % 200) == 0) {
//         sched_flags.mlx_due = true;
//     }
//     // 20 ms: motor pattern step
//     if ((ms_counter % 20) == 0) {
//         sched_flags.pwm_due = true;
//     }
//     // 500 ms: LED heartbeat
//     if ((ms_counter % 500) == 0) {
//         sched_flags.led_due = true;
//     }
// }

// Non-blocking task handlers (called from main loop)
static void task_mlx_frame(void)
{
    sched_flags.mlx_due = false;

    switch (mlx_ctx.state) {
        case MLX_STATE_WAIT_READY:
            // Check once; if ready, start incremental read
            if (MLX_poll_data_ready()) {
                mlx_ctx.state = MLX_STATE_READ_ROWS;
                mlx_ctx.current_row = 0;
            }
            break;

        case MLX_STATE_READ_ROWS:
            // Read one row per service call to stay non-blocking
            MLX_read_row(mlx_ctx.current_row, &mlx_ctx.frame[(uint16_t)mlx_ctx.current_row * MLX_WIDTH]);
            mlx_ctx.current_row++;

            // Reset bus periodically to avoid lockups
            if ((mlx_ctx.current_row % 4) == 0) {
                TWI0_reset_bus();
            }

            if (mlx_ctx.current_row >= MLX_HEIGHT) {
                mlx_ctx.state = MLX_STATE_PROCESS;
            }
            break;

        case MLX_STATE_PROCESS:
            // Process buffered frame and report
            MLX_process_buffer_and_report(mlx_ctx.frame);
            MLX_send_buffer_to_pc(mlx_ctx.frame);
            mlx_ctx.state = MLX_STATE_WAIT_READY;
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

            char buf[64];
        snprintf(buf, sizeof(buf),
        "CPUINT CTRLA=%02X  LVL0PRI=%02X  STATUS=%02X\r\n",
        CPUINT.CTRLA, CPUINT.LVL0PRI, CPUINT.STATUS);
        USART2_sendString(buf);


    mlx_ctx.state = MLX_STATE_WAIT_READY;
    mlx_ctx.current_row = 0;

    timer_init_1khz();
    USART2_sendString("[TIMER INIT OK]\r\n");
    sei();
    TCB0.INTFLAGS = TCB_CAPT_bm;
    USART2_sendString("[SEI DONE]\r\n");
}

void scheduler_service_tasks(void)
{
    // Debug: print flag states occasionally
    static uint16_t dbg_cnt = 0;
    dbg_cnt++;
    if (dbg_cnt >= 50000) {
        dbg_cnt = 0;
        char buf[40];
        snprintf(buf, sizeof(buf), "F:%d%d%d%d\r\n", 
            sched_flags.led_due, sched_flags.pwm_due, 
            sched_flags.mpu_due, sched_flags.mlx_due);
        USART2_sendString(buf);
    }

    if (sched_flags.led_due) {
        USART2_sendString("[LED]\r\n");
        task_led_toggle();
        USART2_sendString("~\r\n");
    }
    if (sched_flags.pwm_due) {
        USART2_sendString("[PWM]\r\n");
        task_pwm_update();
    }
    // Temporarily disabled to isolate crash source
    // if (sched_flags.mpu_due) {
    //     USART2_sendString("[MPU]\r\n");
    //     task_mpu_sample();
    // }
    // if (sched_flags.mlx_due) {
    //     USART2_sendString("[MLX]\r\n");
    //     task_mlx_frame();
    // }
    sched_flags.mpu_due = false;
    sched_flags.mlx_due = false;
}