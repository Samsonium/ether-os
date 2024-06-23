##### Ether OS ####
# Kernel Makefile #
####### 2024 ######
TARGET = i686
CC = /usr/local/$(TARGET)/bin/$(TARGET)-elf-gcc
CFLAGS = -ffreestanding -O2 -Wall -Wextra -std=gnu99

# Directories
build = ../build
out = ../build
s_asm = ./asm
s_cpu = ./cpu

# Targets
c_main = $(out)/kernel.c.o
c_asm = $(patsubst $(s_asm)/%.c, $(out)/%.co, $(wildcard $(s_asm)/*.c))
c_cpu = $(patsubst $(s_cpu)/%.c, $(out)/%.co, $(wildcard $(s_cpu)/*.c))
c_headers = $(wildcard $(s_asm)/*.h $(s_cpu)/*.h)

###################

##
all: clean $(c_main) $(c_asm) $(c_cpu)

##
clean:
	@echo "--> Removing old objects"
	@rm -f $(build)/*.co

## Compile kernel main
$(c_main): kernel.c ${c_headers}
	@echo "--> Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

## Compile assembly C source
$(out)/%.co: $(s_asm)/%.c ${c_headers}
	@echo "--> Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

## Compile cpu C source
$(out)/%.co: $(s_cpu)/%.c ${c_headers}
	@echo "--> Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@
