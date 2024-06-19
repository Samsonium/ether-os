########################
# Boot sector Makefile #
########################

build = ../build

# Targets
input = ./main.s
output = $(build)/boot.bin

#########################

##
all: clean $(output)

##
clean:
	@echo "-> Clean old boot binary"
	@rm -f $(output)

##
$(output): $(input)
	@echo "-> "
	nasm $< -f bin -o $@
