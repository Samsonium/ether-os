##### 2024 ####
# Kernel make #
## Samsonium ##

# Check for environment
ifeq ($(MITTOS64),)
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
LDFLAGS := -n -nostdlib -lgcc -T Link.ld

# Etc
DESTDIR ?= $(BUILDROOT)sysroot
PREFIX ?= /

#### Common targets ####

##
kernel: $(OBJ)
	$(LINK.c) $^ -o $@

##
install: $(DESTDIR)$(PREFIX)/kernel

##
$(DESTDIR)$(PREFIX)/kernel: kernel
	install -D kernel $(DESTDIR)$(PREFIX)/kernel

##
clean:
	rm -rf $(OBJ) $(DEP) kernel

##
.PHONY: install

#### Support targets ####

DEP := $(OBJ:.o=.d)
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d
$(OBJ): CPPFLAGS += $(DEPFLAGS)
%.d: ;

#### Dependencies ####

include $(DEP)
