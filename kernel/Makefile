####### 2024 ######
# Kernel Makefile #
#### Samsonium ####

# Utils
UTILS = /usr/local/i386elfgcc/bin
LD = $(UTILS)/i386-elf-ld
CC = $(UTILS)/i386-elf-gcc
CFLAGS = -g -ffreestanding

# Dirs
build = ../build
tmp = ./tmp
asm = ./asm
assets = ./assets
include = ./include
srcs = cpu drivers libc

# Targets
KERNEL_OUT := $(build)/kernel.bin
KERNEL_MAIN := ./kernel.c
KERNEL_MAIN_OUT := $(tmp)/kernel.o
KERNEL_ENTRY := ./kentry.s
KERNEL_ENTRY_OUT := $(tmp)/kentry.o
C_SOURCE := $(foreach dir, $(srcs), $(wildcard $(dir)/*.c))
C_HEADERS := $(wildcard $(include)/*.h)
ASM := $(patsubst $(asm)/%.s, $(tmp)/%.so, $(wildcard $(asm)/*.s))
OBJ := $(foreach f, $(C_SOURCE), $(patsubst %.c,%.o,$(f)))

####################

## All targets
all: clean prepare $(KERNEL_OUT) after

## Remove old bin
clean:
	@echo "--> Removing old kernel binary"
	@rm -f $(KERNEL_OUT)

## Build preparing
prepare:
	@echo "--> Creating temp dir"
	@mkdir -p $(tmp)

## Steps after build
after:
	@echo "--> Removing temp dir"
	@rm -rf $(tmp)

## Build kernel
$(KERNEL_OUT): $(KERNEL_ENTRY_OUT) ${ASM} $(KERNEL_MAIN_OUT) $(patsubst %.o,$(tmp)/%.o,${OBJ})
	@echo "--> Building kernel binary"
	@$(LD) -o $@ -Ttext 0x1000 $^ --oformat binary --allow-multiple-definition

## Build kernel entry
$(KERNEL_ENTRY_OUT): $(KERNEL_ENTRY)
	@echo "--> Building kernel entry"
	@nasm $< -f elf -o $@

## Compile kernel main
$(KERNEL_MAIN_OUT): $(KERNEL_MAIN)
	@echo "--> Compiling kernel main"
	@$(CC) $(CFLAGS) -c $< -o $@

## Build assembly sources
$(tmp)/%.so: $(asm)/%.s
	@echo "--> Building $(<F)"
	@nasm $< -f elf -o $@

## Build C sources
$(tmp)/%.o: %.c ${C_HEADERS}
	@echo "--> Compiling $(@F)"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@if [ "${@F}" = "font.o" ]; then \
		echo "-> Copying font asset into object"; \
		objcopy -O elf32-i386 -B i386 -I binary assets/gr8x16.psf $@; \
	fi