##### 2024 ####
# Kernel make #
## Samsonium ##

# Check for environment
ifeq ($(ISETHER),)
$(error Unsupported environment)
endif

#### Variables ####

# Tools
CC := x86_64-elf-gcc

# Paths
SRC := $(wildcard **/*.[cS])
OBJ := $(patsubst %, %.o, $(basename $(SRC)))

# Flags
CFLAGS := -Wall -Wextra -pedantic -ffreestanding
CFLAGS += -ggdb -O0
ASFLAGS := -ggdb
CPPFLAGS := -I include
LDFLAGS := -n -nostdlib -lgcc -T link.ld

# Etc
DESTDIR ?= $(BUILDROOT)sysroot
PREFIX ?= /

#### Common targets ####

##
kernel.bin: $(OBJ)
	$(LINK.c) $^ -o $@

##
install: $(DESTDIR)$(PREFIX)/kernel.bin

##
$(DESTDIR)$(PREFIX)/kernel.bin: kernel.bin
	install -D kernel.bin $(DESTDIR)$(PREFIX)/kernel.bin

##
after:
	rm -rf $(OBJ) $(DEP)

##
clean: after
	rm -rf kernel.bin

##
.PHONY: install

#### Support targets ####

DEP := $(OBJ:.o=.d)
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d
$(OBJ): CPPFLAGS += $(DEPFLAGS)
%.d: ;

#### Dependencies ####

include $(DEP)
