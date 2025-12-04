# 2025-Embedded-system

ATmega4809 project with MLX90640 thermal camera and MPU6050 IMU sensor.

## Hardware Setup

### Components
- **ATmega4809** (running at 16MHz, 3.3V)
- **MLX90640** - 32x24 thermal camera (I2C address: 0x33)
- **MPU6050** - 6-axis IMU (I2C address: 0x68)
- **Arduino UNO** - as JTAG2UPDI programmer
- **Arduino Due** - as USB-Serial bridge for debugging

### Pin Connections

#### ATmega4809 I2C (TWI0)
| ATmega4809 | Device |
|------------|--------|
| PA2 (SDA)  | MLX90640 SDA, MPU6050 SDA |
| PA3 (SCL)  | MLX90640 SCL, MPU6050 SCL |
| GND        | All GNDs connected |
| VCC (3.3V) | Sensor power |

#### Debug UART (USART2 → Arduino Due)
| ATmega4809 | Arduino Due |
|------------|-------------|
| PF0 (TX)   | Pin 19 (RX1) |
| GND        | GND |

#### Programming (JTAG2UPDI via Arduino UNO)
| ATmega4809 | Arduino UNO |
|------------|-------------|
| UPDI       | Pin 6 (via 4.7kΩ resistor) |
| GND        | GND |

## Software Requirements

### For Building & Flashing
- **AVR-GCC** toolchain
- **AVRDUDE** programmer
- **Make** (GNU Make or compatible)
- **JTAG2UPDI** firmware on Arduino UNO

### For Thermal Visualization (Optional)
- **Python 3.x**
- Python packages: `pyserial`, `numpy`, `matplotlib`

## Building the Project

### 1. Clone the Repository
```bash
git clone https://github.com/subinprk/2025-Embedded-system.git
cd 2025-Embedded-system
```

### 2. Configure COM Port
Edit `Makefile` and set the correct COM port for your JTAG2UPDI programmer:
```makefile
PORT=COM4    # Change to your Arduino UNO's COM port
```

### 3. Compile
```bash
make clean
make
```

### 4. Flash to ATmega4809
```bash
make flash
```

## UART Debugging with Arduino Due

### 1. Upload Serial Bridge to Arduino Due
Open `arduino-due-serial/arduino-due-serial.ino` in Arduino IDE and upload to Arduino Due.

### 2. Connect Hardware
- ATmega4809 PF0 (TX) → Arduino Due Pin 19 (RX1)
- ATmega4809 GND → Arduino Due GND

### 3. Open Serial Monitor
Use PuTTY or any terminal:
- **COM Port**: Arduino Due's COM port (e.g., COM9)
- **Baud Rate**: 115200
- **Data Bits**: 8
- **Parity**: None
- **Stop Bits**: 1

## Thermal Image Visualization

### 1. Install Python Dependencies
```bash
pip install pyserial numpy matplotlib
```

### 2. Close PuTTY/Serial Monitor
The Python script needs exclusive access to the COM port.

### 3. Run the Viewer
```bash
cd tools
python ./tools/mlx_viewer.py COM9 115200
```
Replace `COM9` with your Arduino Due's COM port.

### 4. View Thermal Image
A matplotlib window will display the 32x24 thermal image as a heatmap, updating every 2 seconds.

## Project Structure

```
2025-Embedded-system/
├── Makefile              # Build configuration
├── README.md             # This file
├── include/              # Header files
│   ├── mlx90640.h        # MLX90640 thermal camera
│   ├── mpu6050.h         # MPU6050 IMU sensor
│   ├── twi.h             # I2C/TWI driver
│   └── uart.h            # UART driver
├── src/                  # Source files
│   ├── main.c            # Main application
│   ├── mlxInput.c        # MLX90640 & MPU6050 I2C functions
│   ├── mpu6050.c         # MPU6050 driver
│   ├── twi.c             # TWI/I2C master driver
│   └── uart.c            # UART driver (USART1 & USART2)
└── tools/                # PC-side tools
    └── mlx_viewer.py     # Python thermal image viewer
```

## Configuration Options

### Clock Speed
The ATmega4809 runs at 16MHz (prescaler disabled in `clock_init()`). If you need to change this, modify:
- `Makefile`: `-DF_CPU=16000000UL`
- `src/main.c`: `clock_init()` function

### I2C Speed
Default: 100kHz. To change, modify `TWI0.MBAUD` in `src/twi.c`:
```c
// For 16MHz @ 100kHz: MBAUD = 75
// For 16MHz @ 400kHz: MBAUD = 15
TWI0.MBAUD = 75;
```

### Debug UART Selection
In `include/twi.h`, you can select which UART is used for TWI debug output:
```c
#define TWI_DEBUG_UART  2    // 1 = USART1, 2 = USART2
#define TWI_DEBUG_ENABLE 1   // 0 = disable, 1 = enable
```

## Troubleshooting

### UART Shows Garbled Text
- Check baud rate matches on both ends (9600 for ATmega↔Due, 115200 for Due↔PC)
- Verify `F_CPU` matches actual clock speed

### I2C Devices Not Found
- Check wiring (SDA, SCL, GND, VCC)
- Verify 3.3V power supply
- Run `TWI0_scan()` to find device addresses

### Programming Fails
- Check JTAG2UPDI connections (UPDI pin, GND)
- Verify COM port in Makefile
- Ensure 4.7kΩ resistor between UNO Pin 6 and UPDI

## License

MIT License