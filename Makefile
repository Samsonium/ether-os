########## 2024 ###########
# EtherOS common Makefile #
######## Samsonium ########

# Dirs
build_dir = ./build
boot_dir = ./boot
kern_dir = ./kernel

# Bin
boot = $(build_dir)/boot.bin
kern = $(build_dir)/kernel.bin
os	 = $(build_dir)/os.bin

###########################

## Build all
all: prepare $(os)

## Cleanup build
clean:
	@echo "-> Removing build directory"
	@rm -rf $(build_dir)

## Prepare project to build
prepare:
	@echo "-> Preparing"
	@rm -rf $(build_dir)
	@mkdir -p $(build_dir)

## Run OS image on i386 system
run:
	qemu-system-i386 -drive format=raw,file=$(os) -vga std -display sdl

## Build OS image
$(boot):
	@echo "-> Creating bootloader"
	(cd $(boot_dir) && make)

## Build kernel
$(kern):
	@echo "-> Building kernel"
	(cd $(kern_dir) && make)

## Build bootsector
$(os): $(boot) $(kern)
	@echo "-> Exporting OS binary"
	@cat $^ > $(build_dir)/os.bin
