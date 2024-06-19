######### 2024 #########
# Boot sector Makefile #
####### Samsonium ######

# Dirs
build = ../build

# Targets
input = ./main.s
output = $(build)/boot.bin

#########################

##
all: clean $(output)

##
clean:
	@echo "--> Clean old boot binary"
	@rm -f $(output)

##
$(output): $(input)
	@echo "--> Building boot binary"
	@nasm $< -f bin -o $@
