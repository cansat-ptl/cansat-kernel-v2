# Make does not offer a recursive wildcard function, so here's one:
rwildcard=$(wildcard $(addsuffix $2, $1)) $(foreach d,$(wildcard $(addsuffix *, $1)),$(call rwildcard,$d/,$2))

# How to recursively find all files that match a pattern
#ALL_C_FILES := $(call rwildcard,$(SRCDIR)/,*.c)

TARG = yktsat-rtos
 
CC = "D:\BuildTools\avr-gcc-9.2.0-x64-mingw\bin\avr-gcc.exe"
OBJCOPY = "D:\BuildTools\avr-gcc-9.2.0-x64-mingw\bin\avr-objcopy.exe"
ATMEL_DFP = "D:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\ATmega_DFP\1.4.346\include"

INC = -I"include"

AVR_SIZE_CMD = "D:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "yktsat-rtos.elf"
 
SRCDIR = cubesat-rtos

SRCS = $(call rwildcard, $(SRCDIR)/,*.c) main.c
 
OBJS = $(SRCS:.c=.o)

MCU=atmega128
 
CFLAGS = -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I$(ATMEL_DFP) $(INC) -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=$(MCU) -c -std=gnu99

LDFLAGS = -Wl,-static -Wl,-Map="yktsat-rtos.map" -Wl,-u,vfprintf -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mrelax -Wl,-section-start=.bootloader=0x3c000   -mmcu=$(MCU)  -Wl,-u,vfprintf -lprintf_flt -lm
 
all: $(TARG)
    
$(TARG): $(OBJS)
	$(CC) -o $@.elf $(OBJS) $(LDFLAGS)
	$(OBJCOPY) -O binary -R .eeprom -R .nwram  $@.elf $@.bin
	$(OBJCOPY) -O ihex -R .eeprom -R .nwram  $@.elf $@.hex  
    $(AVR_SIZE_CMD)
 
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
 
clean:
	rm -f $(TARG).elf $(TARG).bin $(TARG).hex  $(OBJS) $(TARG).map $(TARG).usersignatures $(TARG).srec $(TARG).lss $(TARG).eeprom 