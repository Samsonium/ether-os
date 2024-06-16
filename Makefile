C_SOURCES = $(wildcard src/kernel/*.c src/drivers/*.c src/cpu/*.c src/libc/*.c)
C_HEADERS = $(wildcard src/kernel/*.h src/drivers/*.h src/cpu/*.h src/libc/*.h)
OBJ = ${C_SOURCES:.c=.o}

# Compiler and linker
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
LD = /usr/local/i386elfgcc/bin/i386-elf-ld
CFLAGS = -g -ffreestanding

# Dirs
build	 	= ./build
cpu_src		= ./src/cpu
libc_src 	= ./src/libc
boot_src 	= ./src/boot
kern_src 	= ./src/kernel
drivers_src = ./src/drivers

# Bin
boot = $(build)/boot.bin
kern = $(build)/kernel.bin
os	 = $(build)/os.bin

# Kernel
kern_in    	= $(wildcard $(build)/*.o)
kern_h		= $(wildcard $(kern_src)/*.h $(drivers_src)/*.h $(cpu_src)/*.h $(libc_src)/*.h)
kern_c 		= $(wildcard $(kern_src)/*.c $(drivers_src)/*.c $(cpu_src)/*.c $(libc_src)/*.c)
kern_e	    = $(kern_src)/kentry.s
kern_e_out	= $(build)/kentry.o
kern_i 		= $(cpu_src)/interrupt.asm
kern_i_out	= $(build)/interrupt.o

# Boot
boot_s = $(boot_src)/main.s

#
# =================== [ Jobs ] ===================
#

# Run all jobs
all: clean prepare ${OBJ} run

# Cleanup fs
clean:
	rm -rf $(build)

# Prepare project
prepare:
	@echo "-> Preparing"
	mkdir -p $(build)

# Run emulator
run: $(os)
	@echo "-> Running emulator"
	qemu-system-i386 -fda $<

# Build OS image
$(os): $(boot) $(kern)
	@echo "-> Writing OS image"
	@cat $^ > $@

#
# === [ Kernel ] ===
#

# Make kernel binary
$(kern): $(kern_e_out) $(kern_in) $(kern_i_out)
	@echo "-> Building kernel.bin"
	@$(LD) -o $@ -Ttext 0x1000 $^ --oformat binary --allow-multiple-definition

# Build each .c
$(OBJ):%.o:%.c ${C_HEADERS}
	@echo "-> Building" $(@F)
	@$(CC) $(CFLAGS) -c $< -o $(build)/$(@F)

# Build kernel entry
$(kern_e_out): $(kern_e)
	@echo "-> Compiling kernel entry"
	@nasm $< -f elf -o $@

# Build interrupts assembly code
$(kern_i_out): $(kern_i)
	@echo "-> Compiling interrupts"
	@nasm $< -f elf -o $@

#
# === [ Bootsector ] ===
#

# Build bootsector binary
$(boot): $(boot_s)
	@echo "-> Compiling bootloader"
	@nasm $< -f bin -o $@
