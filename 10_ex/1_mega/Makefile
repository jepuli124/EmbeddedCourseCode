# Parameters:
F_CPU=16000000UL

#MCU=atmega328p # With atmega 2560 MCU -> atmega2560
MCU=atmega2560 # With atmega 2560 MCU -> atmega2560

#PROGRAMMER=arduino
PROGRAMMER=wiring

PORT=/dev/ttyACM0
BAUD=115200
TARGET=main


# Compiler
CC=avr-gcc

# -g debug, -Os optimization, -mmcu chip, -DF_CPU is the speed of chip
CFLAGS=-g -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)


# AVRDUUDE
AVRDUDE=avrdude -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD)

# Target for all:
all: $(TARGET).hex upload clean

# Compilation process
$(TARGET).hex:$(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET).elf $(TARGET).c
	avr-objcopy -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex

# Bit banging
upload: $(TARGET).hex
	$(AVRDUDE) -DU flash:w:$(TARGET).hex:i


# Tidying folder
clean:
	rm -f $(TARGET).elf $(TARGET).hex
# run "make all" to run compilation, upload and clean

