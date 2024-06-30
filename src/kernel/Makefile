# Check environment
ifeq ($(ISETHER),)
$(error Unsupported environment. Use toolchain inside container to compile kernel)
endif

# C compiler
CC := x86_64-elf-gcc

# Sysroot destination
DESTDIR ?= $(BUILDROOT)/sysroot

# Objects
SRC := $(wildcard **/*.[cS])
OBJ := $(patsubst %, %.o, $(basename $(SRC)))
DEP := $(OBJ:.o=.d)

# Flags
CFLAGS := -Wall -Wextra -pedantic -ffreestanding
CFLAGS += -ggdb -O0 -mcmodel=large
ASFLAGS += -ggdb
CPPFLAGS += -I include
LDFLAGS := -n -nostdlib -lgcc -T Link.ld
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

# Kernel binary target
kernel.bin: $(OBJ)
	$(LINK.c) $^ -o $@

# Dependencies
$(OBJ): CPPFLAGS += $(DEPFLAGS)
%.d: ;

# Copy kernel binary to sysroot
$(DESTDIR)$(PREFIX)/kernel.bin: kernel.bin
	install -D kernel.bin $(DESTDIR)$(PREFIX)/kernel.bin

install: $(DESTDIR)$(PREFIX)/kernel.bin

clean:
	rm -rf $(OBJ) $(DEP) kernel.bin

.phony: install

after:
	rm -rf $(OBJ) $(DEP)

include $(DEP)
