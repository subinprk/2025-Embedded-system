# Makefile for Arduino DUE (SAM3X8E)

# Board settings
BOARD = arduino:sam:arduino_due_x_dbg
PORT ?= COM9  # Change this to your DUE's COM port

# Arduino CLI executable
ARDUINO_CLI = arduino-cli

# Sketch name
SKETCH = arduino-due-serial.ino

.PHONY: all upload clean install-core list-ports

all: compile

# Install Arduino SAM core if not present
install-core:
	@echo "Installing Arduino SAM core for DUE..."
	$(ARDUINO_CLI) core update-index
	$(ARDUINO_CLI) core install arduino:sam

# Compile the sketch
compile:
	@echo "Compiling sketch for Arduino DUE..."
	$(ARDUINO_CLI) compile --fqbn $(BOARD) $(SKETCH)

# Upload to Arduino DUE
upload:
	@echo "Uploading to Arduino DUE on $(PORT)..."
	$(ARDUINO_CLI) upload -p $(PORT) --fqbn $(BOARD) $(SKETCH)

# Compile and upload
flash: compile upload

# List available serial ports
list-ports:
	$(ARDUINO_CLI) board list

# Clean build artifacts
clean:
	@echo "Cleaning build files..."
	@if exist build rd /s /q build 2>NUL || rm -rf build

# Help
help:
	@echo "Arduino DUE Serial Bridge Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make install-core  - Install Arduino SAM core"
	@echo "  make compile       - Compile the sketch"
	@echo "  make upload        - Upload to DUE (set PORT=COMx)"
	@echo "  make flash         - Compile and upload"
	@echo "  make list-ports    - List available ports"
	@echo "  make clean         - Remove build files"
	@echo ""
	@echo "Usage:"
	@echo "  make flash PORT=COM5"
