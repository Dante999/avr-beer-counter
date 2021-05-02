################################################################################
# General project settings
################################################################################

# Name of the target (files will be named like that)
TARGET = avr-beercounter

# Path where objects files and binaries will be located after creation
BUILDPATH = ./build

# Name of the Microcontroller
MCU=attiny2313

# Clock of the Microcontroller (Hz)
F_CPU=8000000

# Fuses
L_FUSE=0x64
H_FUSE=0xDF
E_FUSE=0xFF

# Progammer
# [usbasp|arduino]
PROGRAMMER=usbasp
PORT=-P/dev/ttyUSB0
#BAUD=-B9600
#BAUD=-b 57600

# Target where the files should be placed
SYNC_DIR=../avr-boy/src/protocol

EXTRA_FLAGS = -DF_CPU=$(F_CPU)UL


################################################################################
# Project files
################################################################################

#VPATH = ../core
#VPATH += driver
#VPATH += util

# List of all sources (only source-files)
SOURCES  = main.cpp
SOURCES += display.cpp
SOURCES += buttons.c
SOURCES += state_machine.cpp
SOURCES += storage.c
#SOURCES += core.c
#SOURCES += logger.c
#SOURCES += spritelist.cpp
#SOURCES += ball.cpp

################################################################################
# Building Object files 
################################################################################

# Pattern of the objects files 
OBJ_PATTERN = $(BUILDPATH)/%.o

# Change file extension from .c to .o
TMP1 =  $(SOURCES:%.c=$(OBJ_PATTERN))

# Change file extension from .cpp to .o
TMP2 = $(TMP1:%.cpp=$(OBJ_PATTERN))

# Final list ob object files to generate
OBJECTS = $(TMP2)


################################################################################
# Compiler
################################################################################
CC = avr-g++

CFLAGS = -I.                       # Include current file location to search path
CFLAGS += -I..
CFLAGS += -c                       # Compile and assemble, but do not link
#CFLAGS += -g                       # Generate debug information in default format
CFLAGS += -Os                      # Optimize for space rather than speed
CFLAGS += -std=c++11               # Use this C standard

CFLAGS += -Werror                  # Treat all warnings as errors
CFLAGS += -Wall                    # Print all warnings
CFLAGS += -Wextra                  # Print extra (possibly unwanted) warnings
CFLAGS += -Wunused                 # Enable all -Wunused- warnings
CFLAGS += -Wshadow                 # Warn if a variable declaration shadows one from the parent context
CFLAGS += -Wpedantic

CFLAGS += -fpack-struct            # Pack all structure members together without holes
CFLAGS += -fshort-enums            # Use the smallest needed integer for enum values
CFLAGS += -ffunction-sections      # Place each function their own section
CFLAGS += -fdata-sections          # Place data items into their own section
CFLAGS += -funsigned-char          # Make any unqualfied char type an unsigned char
CFLAGS += -funsigned-bitfields     # Make any unqualified bitfield type unsigned

CFLAGS += $(EXTRA_FLAGS)


################################################################################
# Linker
################################################################################
LD = avr-g++

LDFLAGS = 


################################################################################
# Makefile targets
################################################################################

all: prepare clean hex eeprom flash_hex

prepare:
	mkdir -p $(BUILDPATH)
	@echo "object files: $(OBJECTS)"

sync:
	cp $(SYNC_DIR)/protocol.*  protocol/
	cp $(SYNC_DIR)/cartridge.* protocol/
	cp $(SYNC_DIR)/core.*      protocol/

list_targets:
	avr-gcc --target-help

hex: $(TARGET).hex size

eeprom: $(TARGET)_eeprom.hex

$(TARGET).hex: $(TARGET).elf
	@echo "\n\n-----\n building .hex file\n-----"
	avr-objcopy -O ihex -j .data -j .text $(BUILDPATH)/$(TARGET).elf $(BUILDPATH)/$(TARGET).hex

$(TARGET)_eeprom.hex: $(TARGET).elf
	avr-objcopy -O ihex -j .eeprom --change-section-lma .eeprom=1 $(BUILDPATH)/$(TARGET).elf $(BUILDPATH)/$(TARGET)_eeprom.hex

$(TARGET).elf: $(OBJECTS)
	@echo "\n\n-----\n building .elf file\n-----"
	$(LD) $(LDFLAGS) -mmcu=$(MCU) $(OBJECTS) -o $(BUILDPATH)/$(TARGET).elf


$(OBJ_PATTERN): %.c
	@echo "\n\n-----\n compiling c file '$<' -> '$@'\n-----"
	$(CC) $(CFLAGS) -mmcu=$(MCU) $< -o $@

$(OBJ_PATTERN): %.cpp
	@echo "\n\n-----\n compiling c++ file '$<' -> '$@'\n-----"
	$(CC) $(CFLAGS) -mmcu=$(MCU) $< -o $@

size:
	avr-size --mcu=$(MCU) -C $(BUILDPATH)/$(TARGET).elf

flash_hex:
	avrdude -p $(MCU) $(PORT) $(BAUD) -c $(PROGRAMMER) -U flash:w:$(BUILDPATH)/$(TARGET).hex:a
	#	micronucleus --run $(BUILDPATH)/$(TARGET).hex

read_fuses:
	avrdude -p $(MCU) $(PORT) $(BAUD) -c $(PROGRAMMER) -U lfuse:r:-:i

flash_fuses:
	avrdude -p $(MCU) $(PORT) $(BAUD) -c $(PROGRAMMER) -U lfuse:w:$(L_FUSE):m -U hfuse:w:$(H_FUSE):m -U efuse:w:$(E_FUSE):m

clean_tmp:
	rm -rf $(BUILDPATH)/*.o
	rm -rf $(BUILDPATH)/*.elf

clean:
	rm -rf $(BUILDPATH)/*.o
	rm -rf $(BUILDPATH)/*.elf
	rm -rf $(BUILDPATH)/*.hex
