##### Ether OS ####
# Kernel Makefile #
####### 2024 ######
TARGET = i686
CC = /usr/local/$(TARGET)/bin/$(TARGET)-elf-g++
CFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

# Targets
kernel_in  = kernel.cpp
kernel_out = ../build/kernel.o

###################

##
all: $(kernel_out)

## Kernel
$(kernel_out): $(kernel_in)
	@echo "--> Compiling kernel"
	@$(CC) $(CFLAGS) -c $< -o $@

