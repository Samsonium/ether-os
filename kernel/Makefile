##### Ether OS ####
# Kernel Makefile #
####### 2024 ######
TARGET = i686
CC = /usr/local/$(TARGET)/bin/$(TARGET)-elf-gcc
CFLAGS = -ffreestanding -O2 -Wall -Wextra -std=gnu99

# Targets
kernel_in  = kernel.c
kernel_out = ../build/kernel.o

###################

##
all: $(kernel_out)

## Kernel
$(kernel_out): $(kernel_in)
	@echo "--> Compiling kernel"
	@$(CC) $(CFLAGS) -c $< -o $@

