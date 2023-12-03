# build utilities (GNU)
CC = gcc
AS = as
LD = ld

# Compiler flags
CFLAGS = -Wall -Werror -O -fno-omit-frame-pointer -ggdb -m32
CFLAGS += -MD
CFLAGS += -ffreestanding -fno-common -nostdlib
CFLAGS += -fno-stack-protector

# linker flags
LDFLAGS = -melf_i386 -T kernel/kernel.ld -z noexecstack

# assembler flags
ASFLAGS = --32

# source files
SRC = $(shell find kernel -name "*.c")
OBJ = $(SRC:%.c=%.o)

# assembly sources
ASSRC = $(shell find kernel -name "*.S")
ASOBJ = $(ASSRC:%.S=%.o)

all: build

$(OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(ASOBJ): %.o: %.S
	$(AS) $(ASFLAGS) -o $@ $<

-include kernel/*.d

build: $(OBJ) $(ASOBJ)
	$(LD) $(LDFLAGS) -o kernel/kernel $(OBJ) $(ASOBJ)

clean:
	rm $(OBJ) $(OBJ:.o=.d) kernel/kernel

# emulation in qemu
qemu:
	qemu-system-i386 -kernel kernel/kernel -vga std

.PHONY: all build clean qemu

