# PlatformIO-based Makefile for ATmega4809 project
# This Makefile wraps PlatformIO commands for convenience

.PHONY: all build upload flash clean monitor help pio-clean

# Environment
ENV ?= ATmega4809
PIO := pio

# Default target
all: build

# Build the project
build:
	@echo "Building for $(ENV)..."
	$(PIO) run -e $(ENV)

# Upload/Flash the firmware to the device
upload flash:
	@echo "Uploading firmware to $(ENV) via JTAG2UPDI..."
	$(PIO) run -e $(ENV) -t upload

# Monitor serial output
monitor:
	@echo "Opening serial monitor for $(ENV)..."
	$(PIO) device monitor -e $(ENV) -b 9600

# Build and upload in one command
all-upload: build upload
	@echo "Build and upload complete!"

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	$(PIO) run -e $(ENV) -t clean
	rm -f main.elf main.hex
	rm -f src/*.o

# Deep clean (removes .pio directory)
pio-clean: clean
	@echo "Removing PlatformIO build directory..."
	rm -rf .pio

# Show project information
info:
	@echo "Project Information:"
	@echo "  Environment: $(ENV)"
	@echo "  Board: ATmega4809"
	@echo "  CPU Clock: 16 MHz"
	@echo "  Baud Rate: 9600"
	@echo "  Upload Protocol: jtag2updi"

# Help target
help:
	@echo "ATmega4809 Project Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make build       - Build the firmware"
	@echo "  make upload      - Upload/flash firmware to device"
	@echo "  make flash       - Alias for upload"
	@echo "  make monitor     - Open serial monitor"
	@echo "  make all-upload  - Build and upload in one command"
	@echo "  make clean       - Clean build artifacts"
	@echo "  make pio-clean   - Deep clean (remove all build files)"
	@echo "  make info        - Show project information"
	@echo "  make help        - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make             # Build the firmware"
	@echo "  make upload      # Upload to device"
	@echo "  make monitor     # Watch serial output"
