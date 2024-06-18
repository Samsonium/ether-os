# Compiler and linker
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LD = /usr/local/i386elfgcc/bin/i386-elf-ld
CFLAGS = -g -ffreestanding

# Dirs
src = ./src
build = ./build

# Bin
boot = $(build)/boot.bin
kern = $(build)/kernel.bin
os	 = $(build)/os.bin

# Kernel
kern_in = $(wildcard $(build)/*.o)
kern_entry = $(src)/kernel/kentry.s

# Boot
boot_s = $(src)/boot/main.s

# Sources paths
subdirs := cpu libc kernel drivers
S_SOURCE := $(foreach dir, $(subdirs), $(wildcard $(src)/$(dir)/*.s))
S_SOURCE := $(kern_entry) $(filter-out ${kern_entry}, $(S_SOURCE))
C_SOURCE := $(foreach dir, $(subdirs), $(wildcard $(src)/$(dir)/*.c))
C_HEADERS := $(foreach dir, $(subdirs), $(wildcard $(src)/$(dir)/*.h))
ASM := ${S_SOURCE:.s=.o}
OBJ := ${C_SOURCE:.c=.o}

#
# =================== [ Jobs ] ===================
#

# Run all jobs
all: clean prepare run

# Cleanup fs
clean:
	@rm -rf $(build)
	@rm -rf $(src)/**/*.o

# Prepare project
prepare:
	@echo "-> Preparing"
	@mkdir -p $(build)

after:
	@echo "-> Removing temp files"
	@rm -rf $(src)/**/*.o

# Run emulator
run: $(os) after
	@echo "-> Running emulator"
	qemu-system-i386 -drive format=raw,file=$< -vga std

# Build OS image
$(os): $(boot) $(kern)
	@echo "-> Writing OS image"
	@cat $(boot) $(kern) > $@

#
# === [ Kernel ] ===
#

define copy_font
	@if [ "$1" = "font.o" ]; then \
		echo "-> Copying font gr8x16.psf to $1"; \
		objcopy -O elf32-i386 -B i386 -I binary $(src)/drivers/gr8x16.psf $2; \
	fi
endef

# Make kernel binary
$(kern): ${ASM} ${OBJ}
	@echo "-> Building kernel.bin"
	@echo "-> Source:" $^
	@$(LD) -o $@ -Ttext 0x1000 $^ --oformat binary --allow-multiple-definition

# Build each .s
$(ASM):%.o:%.s
	@echo "-> Compiling" $(@F)
	@nasm $< -f elf -o $@

# Build each .c
$(OBJ):%.o:%.c ${C_HEADERS}
	@echo "-> Compiling" $(@F)
	@$(CC) $(CFLAGS) -c $< -o $@
	$(call copy_font,$(@F),$@)

#
# === [ Bootsector ] ===
#

# Build bootsector binary
$(boot): $(boot_s)
	@echo "-> Compiling bootloader"
	@nasm $< -f bin -o $@
