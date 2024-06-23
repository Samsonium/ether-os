##### Ether OS #####
# EtherOS Makefile #
####### 2024 #######
TARGET = i686
CC = /usr/local/$(TARGET)/bin/$(TARGET)-elf-gcc
BUILD = build

# Targets
link = linker.ld
boot = $(BUILD)/boot.o
kern = $(wildcard $(BUILD)/*.c.o)
os   = $(BUILD)/os.bin
iso  = $(BUILD)/os.iso

####################

##
all: clean prepare $(iso)

## Run system
run: $(iso)
	@echo "-> Running emulator"
	@qemu-system-i386 -cdrom $(iso) -vga std -display sdl

## Cleanup
clean:
	@echo "-> Cleanup"
	@rm -rf $(BUILD)
	@rm -f iso/boot/os.bin

## Preparing
prepare:
	@echo "-> Prearing"
	@mkdir -p $(BUILD)

## Make .iso
$(iso): $(os)
	@echo "-> Verifying multiboot"
	@if grub-file --is-x86-multiboot $(os); then \
		echo "-> Multiboot confirmed"; \
		echo "-> Copying os binary"; \
		cp $< iso/boot/$(<F); \
		echo "-> Writing .iso"; \
		grub-mkrescue /usr/lib/grub/i386-pc -o $(iso) iso; \
	else \
		echo "$(os) is not multiboot"; \
		rm -rf ./build; \
		exit 64; \
	fi

## OS
$(os): build_boot build_kernel
	@echo "-> Building os binary"
	@$(CC) -T $(link) -o $@ -ffreestanding -O2 -nostdlib $(boot) $(kern) -lgcc

## Boot asm
build_boot:
	@echo "-> Building boot asm"
	@(cd boot && make)

## Kernel
build_kernel:
	@echo "-> Building kernel"
	@(cd kernel && make)
