# 2025-Embedded-system

ATmega4809-based autonomous thermal-tracking robot with MLX90640 thermal camera, MPU6050 IMU, obstacle avoidance sensors, and differential drive motor control.

## Table of Contents
- [Overview](#overview)
- [Hardware Setup](#hardware-setup)
- [System Architecture](#system-architecture)
- [Timer & Interrupt System](#timer--interrupt-system)
- [I2C/TWI Bus Configuration](#i2ctwi-bus-configuration)
- [MLX90640 Thermal Camera](#mlx90640-thermal-camera)
- [Motor Control & PWM](#motor-control--pwm)
- [Drive Algorithm](#drive-algorithm)
- [Building & Flashing](#building--flashing)
- [Debugging & Visualization](#debugging--visualization)
- [Project Structure](#project-structure)
- [Configuration Options](#configuration-options)
- [Troubleshooting](#troubleshooting)

---

## Overview

This project implements an autonomous robot that:
1. **Detects heat sources** using an MLX90640 32×24 thermal camera
2. **Tracks the hottest region** by computing a weighted centroid of hot pixels
3. **Navigates toward the heat source** using differential drive motors
4. **Avoids obstacles** using three digital proximity sensors
5. **Actuates a solenoid** when centered on the target

---

## Hardware Setup

### Components
| Component | Description |
|-----------|-------------|
| **ATmega4809** | Main MCU running at 16 MHz, 3.3V |
| **MLX90640** | 32×24 thermal camera (I2C address: 0x33) |
| **MPU6050** | 6-axis IMU (I2C address: 0x68) |
| **L9100S (×2)** | Dual H-bridge motor drivers |
| **Arduino UNO** | JTAG2UPDI programmer |
| **Arduino Due** | USB-Serial bridge for debugging |

### Pin Connections

#### I2C Bus (TWI0)
| ATmega4809 | Device |
|------------|--------|
| PA2 (SDA) | MLX90640 SDA, MPU6050 SDA |
| PA3 (SCL) | MLX90640 SCL, MPU6050 SCL |
| GND | Common ground |
| VCC (3.3V) | Sensor power |

#### Motor Driver (L9100S)
| ATmega4809 | Motor A | Motor B |
|------------|---------|---------|
| PC0 | IN1 | — |
| PC1 | IN2 | — |
| PC5 | — | IN1 |
| PD0 | — | IN2 |

#### Obstacle Sensors & Solenoid
| ATmega4809 | Function |
|------------|----------|
| PD2 | Left proximity sensor (input, pull-up) |
| PD3 | Center proximity sensor (input, pull-up) |
| PD4 | Right proximity sensor (input, pull-up) |
| PA7 | Solenoid output |

#### Debug UART (USART2)
| ATmega4809 | Arduino Due |
|------------|-------------|
| PF0 (TX) | Pin 19 (RX1) |
| GND | GND |

#### Programming (JTAG2UPDI)
| ATmega4809 | Arduino UNO |
|------------|-------------|
| UPDI | Pin 6 (via 4.7kΩ resistor) |
| GND | GND |

---

## System Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                         ATmega4809 @ 16 MHz                         │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐          │
│  │   TCB0 ISR   │───▶│  Scheduler   │───▶│  Main Loop   │          │
│  │   1 kHz      │    │    Flags     │    │    Tasks     │          │
│  └──────────────┘    └──────────────┘    └──────────────┘          │
│         │                                       │                   │
│         ▼                                       ▼                   │
│  ┌──────────────────────────────────────────────────────────┐      │
│  │                    Task Execution                         │      │
│  ├──────────┬──────────┬──────────┬──────────┬──────────────┤      │
│  │ MLX Task │ MPU Task │ PWM Task │ LED Task │ Drive Update │      │
│  │  20 ms   │  50 ms   │  20 ms   │  500 ms  │  Main Loop   │      │
│  └────┬─────┴────┬─────┴────┬─────┴──────────┴──────┬───────┘      │
│       │          │          │                        │              │
│       ▼          ▼          ▼                        ▼              │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐          ┌───────────┐        │
│  │MLX90640 │ │ MPU6050 │ │ Motors  │          │  Sensors  │        │
│  │ 400kHz  │ │ 400kHz  │ │ A & B   │          │ PD2/3/4   │        │
│  └─────────┘ └─────────┘ └─────────┘          └───────────┘        │
│       └──────────┴─────────────────────────────────┘                │
│                    I2C Bus @ 400 kHz                                │
└─────────────────────────────────────────────────────────────────────┘
```

---

## Timer & Interrupt System

### TCB0 Timer Configuration
The system uses **TCB0** in Periodic Interrupt mode to generate a **1 kHz tick** (1 ms period).

```c
// Timer setup in timer_init_1khz()
TCB0.CTRLB = TCB_CNTMODE_INT_gc;      // Periodic Interrupt mode
TCB0.CCMP = 7999;                      // (8 MHz / 8000) = 1 kHz
TCB0.CTRLA = (0x01 << 1) | TCB_ENABLE_bm;  // CLK_PER/2 = 8 MHz, Enable
```

### Scheduler Flags
The ISR sets boolean flags that are serviced in the main loop (cooperative scheduling):

| Flag | Period | Purpose |
|------|--------|---------|
| `mlx_due` | 20 ms (50 Hz) | Service MLX90640 frame acquisition |
| `mpu_due` | 50 ms (20 Hz) | Read MPU6050 accelerometer/gyroscope |
| `pwm_due` | 20 ms (50 Hz) | Update motor PWM duty cycle |
| `led_due` | 500 ms (2 Hz) | Toggle heartbeat LED |

### Task Cadence Diagram
```
Time (ms):  0    20    40    50    60    80   100   ...  500
            │     │     │     │     │     │     │         │
MLX:        ●─────●─────●─────●─────●─────●─────●─────────●
PWM:        ●─────●─────●─────●─────●─────●─────●─────────●
MPU:        ●───────────●───────────●───────────●─────────●
LED:        ●─────────────────────────────────────────────●
```

---

## I2C/TWI Bus Configuration

### Clock Speed Calculation
The I2C bus runs at **400 kHz** (Fast Mode) to maximize MLX90640 throughput:

```c
// TWI0_BAUD formula: MBAUD = (F_CPU / (2 × f_SCL)) - 5
// For 400 kHz: MBAUD = (16,000,000 / 800,000) - 5 = 15
#define TWI0_BAUD 15
```

### Speed Options
| MBAUD | I2C Clock | Use Case |
|-------|-----------|----------|
| 155 | 50 kHz | Long cables, noisy environments |
| 75 | 100 kHz | Standard mode (default I2C) |
| 35 | 200 kHz | Moderate speed |
| 25 | 266 kHz | Balanced speed/reliability |
| **15** | **400 kHz** | **Current setting (Fast Mode)** |

### Bus Recovery Mechanisms
The driver includes robust recovery for stuck I2C buses:

1. **`TWI0_reset_bus()`**: Full bus reset with 9 SCL pulses + STOP
2. **`TWI0_clock_pulse_stop()`**: Lightweight recovery for minor issues

### Multi-Device Handling
Both MLX90640 (0x33) and MPU6050 (0x68) share the same I2C bus. The software handles:
- Separate START/STOP sequences per device
- Bus recovery between long transfers
- Timeout detection with automatic retry

---

## MLX90640 Thermal Camera

### Frame Acquisition with Ping-Pong Buffering

The MLX90640 produces 768 pixels (32×24) per frame. To avoid blocking the main loop during the ~30-40 ms read time, the system uses **double buffering** (ping-pong):

```
┌────────────────────────────────────────────────────────────┐
│                    Ping-Pong Buffering                      │
├────────────────────────────────────────────────────────────┤
│                                                             │
│   Frame N        Frame N+1       Frame N+2                 │
│   ┌─────┐        ┌─────┐         ┌─────┐                   │
│   │ Buf │──Read──│ Buf │──Read───│ Buf │                   │
│   │  0  │        │  1  │         │  0  │                   │
│   └──┬──┘        └──┬──┘         └──┬──┘                   │
│      │              │               │                       │
│      ▼              ▼               ▼                       │
│   Process        Process         Process                    │
│   Buf 1          Buf 0           Buf 1                      │
│   (prev)         (prev)          (prev)                     │
│                                                             │
└────────────────────────────────────────────────────────────┘
```

### State Machine
```c
typedef enum {
    MLX_STATE_IDLE,        // Not actively acquiring
    MLX_STATE_WAIT_READY,  // Polling for new frame
    MLX_STATE_READ_ROWS,   // Incrementally reading rows
    MLX_STATE_PROCESS      // Processing complete frame
} MlxState;
```

### Incremental Row Reading
Each `task_mlx_frame()` call reads up to **12 rows** (384 pixels), allowing other tasks to run between chunks:

```c
// Read 12 rows per service call (~15-20 ms at 400 kHz)
for (uint8_t i = 0; i < 12 && mlx_ctx.current_row < MLX_HEIGHT; i++) {
    MLX_read_row(mlx_ctx.current_row, &buffer[row * 32]);
    mlx_ctx.current_row++;
}
```

### Frame Timing at 400 kHz I2C
| Parameter | Value |
|-----------|-------|
| Pixels per frame | 768 (32×24) |
| Bits per pixel | 16 |
| Raw data bits | 12,288 |
| I2C clock bits | ~15,360 (with ACK/NACK) |
| Theoretical time | 15,360 / 400,000 ≈ 38 ms |
| Actual time (with overhead) | ~40-50 ms |

### Hotspot Detection Algorithm
The system computes a **weighted centroid** of hot pixels:

```
centroid_x = Σ(x_i × weight_i) / Σ(weight_i)
centroid_y = Σ(y_i × weight_i) / Σ(weight_i)

where weight_i = pixel_value - threshold
      threshold = max_value - (range × 20%)  // Top 20% hottest
```

Result is returned in **8.8 fixed-point** format for sub-pixel precision.

---

## Motor Control & PWM

### Software PWM Implementation
Since the motor pins don't support hardware PWM, the system implements **software PWM** at 50 Hz:

```
PWM Period: 20 ms (from pwm_due flag)
Duty Steps: 10 (each step = 2 ms)

Speed -100 to +100 maps to:
  - Sign → Direction (forward/backward)
  - Magnitude → Duty cycle (0-100%)

Example: speed = 70
  → duty = 7/10 = 70%
  → Motor ON for 14 ms, OFF for 6 ms per 20 ms period
```

### PWM Timing Diagram
```
Speed = 50% (5/10 duty)

        20 ms period
    ◄───────────────────►
    ┌────────────┐
    │     ON     │  OFF
────┘            └────────
    ◄────10ms────►◄─10ms──►
```

### Motor Pin Configuration
```c
// Motor A: PORTC pins
#define MOTOR_A_IN1_bm PIN0_bm  // PC0 - Forward
#define MOTOR_A_IN2_bm PIN1_bm  // PC1 - Backward

// Motor B: Split across PORTC and PORTD
#define MOTOR_B_IN1_bm PIN5_bm  // PC5 - Forward
#define MOTOR_B_IN2_bm PIN0_bm  // PD0 - Backward
```

---

## Drive Algorithm

### Overview
The drive system combines **thermal tracking** with **obstacle avoidance**:

```
┌─────────────────────────────────────────────────────────────┐
│                      Drive Logic Flow                        │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│   ┌──────────┐    ┌──────────────┐    ┌────────────────┐   │
│   │ Obstacle │───▶│   Priority   │───▶│  Motor Speed   │   │
│   │ Sensors  │    │   Decision   │    │   Calculation  │   │
│   └──────────┘    └──────────────┘    └────────────────┘   │
│         │                │                     │            │
│   ┌──────────┐           │                     │            │
│   │   MLX    │───────────┘                     │            │
│   │ Centroid │                                 │            │
│   └──────────┘                                 ▼            │
│                                         ┌────────────┐      │
│                                         │  Motor A   │      │
│                                         │  Motor B   │      │
│                                         └────────────┘      │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

### Obstacle Avoidance (Priority)
| Left (PD2) | Center (PD3) | Right (PD4) | Action |
|:----------:|:------------:|:-----------:|--------|
| 0 | 1 | 0 | Back up (unless centered on heat) |
| 1 | × | 1 | Back up |
| 1 | × | 0 | Turn right |
| 0 | × | 1 | Turn left |
| 0 | 0 | 0 | Normal thermal tracking |

### Thermal Tracking Algorithm
When no obstacles are detected, the robot tracks the thermal centroid:

```c
// Calculate error from frame center (x = 16)
int16_t error_x = centroid_x - 16;

// Proportional speed compensation
int16_t compensation = (error_x × base_speed) / 16;

// Apply differential drive
if (error_x < -deadband) {
    // Heat is LEFT → turn left (motorB faster)
    motorA_speed = base_speed - compensation;
    motorB_speed = base_speed + compensation;
} else if (error_x > deadband) {
    // Heat is RIGHT → turn right (motorA faster)
    motorA_speed = base_speed + compensation;
    motorB_speed = base_speed - compensation;
} else {
    // Centered → go straight
    motorA_speed = motorB_speed = base_speed;
}
```

### Solenoid Activation
The solenoid (PA7) activates when:
1. Centroid is within ±2 columns of center (deadband)
2. Center obstacle sensor (PD3) detects target

---

## Building & Flashing

### Using PlatformIO (Recommended)

```bash
# Build
make build
# or
pio run -e ATmega4809

# Upload
make upload
# or
pio run -e ATmega4809 -t upload

# Monitor serial output
make monitor
# or
pio device monitor -b 9600
```

### Manual Build with AVR-GCC

```bash
# Clean and build
make clean
make

# Flash (requires proper port configuration)
make flash
```

### Configuration
Edit `platformio.ini` or `Makefile` for your setup:

```ini
[env:ATmega4809]
upload_port = /dev/ttyUSB0  ; Linux
; upload_port = COM4        ; Windows
```

---

## Debugging & Visualization

### UART Debug Output
The system outputs debug information at 9600 baud:

```
[LED]           ; Heartbeat toggle
[PWM]           ; Motor update
[MPU]           ; IMU reading
MPU A:123,-456,16384 G:10,-5,2
[MLX]           ; Thermal frame
=== Hotspot Detection (buffered) ===
Max pixel: (15, 12) = 28456
Hot centroid: (15.45, 11.89)
HOTSPOT:3955,3044,28456
```

### Python Thermal Viewer
```bash
cd tools
python mlx_viewer.py /dev/ttyUSB0 9600
```

---

## Project Structure

```
2025-Embedded-system/
├── Makefile              # PlatformIO wrapper
├── platformio.ini        # PlatformIO configuration
├── README.md             # This documentation
├── include/
│   ├── drive.h           # Drive algorithm interface
│   ├── flags.h           # Scheduler flags & MLX state enum
│   ├── interrupt.h       # Timer/scheduler interface
│   ├── mlx90640.h        # MLX90640 constants & functions
│   ├── mlxProcess.h      # Thermal processing algorithms
│   ├── mpu6050.h         # MPU6050 IMU interface
│   ├── pwm.h             # Motor control interface
│   ├── twi.h             # I2C driver interface
│   └── uart.h            # UART driver interface
├── src/
│   ├── main.c            # Application entry point
│   ├── drive.c           # Thermal tracking + obstacle avoidance
│   ├── interrupt.c       # Timer ISR & scheduler tasks
│   ├── mlxInput.c        # MLX90640 I2C communication
│   ├── mlxProcess.c      # Centroid calculation algorithms
│   ├── mpu6050.c         # MPU6050 driver
│   ├── pwm.c             # Software PWM motor control
│   ├── twi.c             # TWI/I2C master driver
│   └── uart.c            # USART1 & USART2 drivers
├── tools/
│   └── mlx_viewer.py     # Python thermal visualization
└── arduino-due-serial/
    └── arduino-due-serial.ino  # USB-Serial bridge sketch
```

---

## Configuration Options

### I2C Speed
In `src/twi.c`:
```c
#define TWI0_BAUD 15   // 400 kHz (Fast Mode)
// #define TWI0_BAUD 25   // 266 kHz (balanced)
// #define TWI0_BAUD 75   // 100 kHz (Standard Mode)
```

### MLX90640 Frame Rate
In `src/main.c`:
```c
MLX_set_framerate(MLX_FRAMERATE_32HZ);  // Options: 0.5, 1, 2, 4, 8, 16, 32, 64 Hz
```

### Task Scheduling Intervals
In `src/interrupt.c` ISR:
```c
if ((ms_counter % 20) == 0) sched_flags.mlx_due = true;  // 50 Hz
if ((ms_counter % 50) == 0) sched_flags.mpu_due = true;  // 20 Hz
if ((ms_counter % 20) == 0) sched_flags.pwm_due = true;  // 50 Hz
if ((ms_counter % 500) == 0) sched_flags.led_due = true; // 2 Hz
```

### Motor Base Speed
In `src/drive.c`:
```c
const int8_t base_speed = 35;  // 35% duty cycle (slow & controlled)
```

---

## Troubleshooting

### I2C Communication Fails
- Verify 3.3V power to sensors
- Check SDA/SCL wiring and pull-ups
- Run `TWI0_scan()` to detect devices
- Try lowering I2C speed (increase `TWI0_BAUD`)

### MLX90640 Returns 0xFFFF
- Sensor not ready; check `MLX_poll_data_ready()`
- Bus stuck; call `TWI0_reset_bus()`
- Power supply insufficient; MLX needs ~25mA

### Motors Don't Move
- Check motor driver connections (IN1/IN2)
- Verify motor power supply (separate from logic)
- Debug with `USART2_sendString()` in `task_pwm_update()`

### UART Garbled Output
- Ensure baud rates match (9600 default)
- Verify `F_CPU` is correctly set to 16000000UL
- Check `clock_init()` disables prescaler

---

## License

MIT License