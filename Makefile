TARGET = FlashLoader

OBJS = Src/FlashPrg.o Src/hal/gpio.o Src/hal/qspi.o Src/qspi_init.o Src/FlashDev.o

AS = arm-none-eabi-gcc
CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

ASFLAGS = -mcpu=cortex-m7
CFLAGS = -O2 -Wall -mcpu=cortex-m7 -fno-builtin -fshort-enums -fno-delete-null-pointer-checks -std=c11 -MMD -fno-toplevel-reorder
LDFLAGS = -mcpu=cortex-m7 -nostartfiles -Wl,-T,Src/FlashLoader.ld -Wl,-x

INC = -I Src/hal
LIBS =

DEPS = $(OBJS:.o=.d)

all: $(TARGET).elf
	$(SIZE) $<

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@ $(LIBS)

%.o: %.c | $(BSP_HEADERS)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f $(TARGET).elf $(OBJS) $(DEPS)
