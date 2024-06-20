#### Ether OS ###
# Boot Makefile #
###### 2024 #####
TARGET = i686
AS = /usr/local/$(TARGET)/bin/$(TARGET)-elf-as

# Target
boot_in  = boot.s
boot_out = ../build/boot.o

#################

##
all: $(boot_out)

## Build boot
$(boot_out): $(boot_in)
	@echo "--> Building $(@F)"
	@$(AS) $< -o $@
