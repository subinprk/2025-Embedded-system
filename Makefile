MCU=atmega4809
CC=avr-gcc
CFLAGS=-mmcu=$(MCU) -Os -DF_CPU=20000000UL -DBAUD_RATE=9600
OBJCOPY=avr-objcopy
AVRDUDE=avrdude

PROGRAMMER=jtag2updi
PORT=COM4    # ← UNO가 잡힌 포트로 맞춰줘!

SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all: main.hex

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

main.elf: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

main.hex: main.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: main.hex
	$(AVRDUDE) -p m4809 -c $(PROGRAMMER) -P $(PORT) -U flash:w:main.hex

clean:
	del main.elf main.hex src\*.o 2> NUL || rm -f main.elf main.hex src/*.o
