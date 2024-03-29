# Target processor architecture.
ARCH ?= x86_64

# Set desired compiler, linker and assembler.
override CC := gcc
override LD := ld
override AS := nasm

# This is the name that our final kernel executable will have.
override KERNEL := kernel.elf

# User controllable CFLAGS.
CFLAGS ?= -O2 -g -Wall -Wextra -Wpedantic -pipe

# User controllable linker flags. We set none by default.
LDFLAGS ?=

# Internal C flags that should not be changed by the user.
override INTERNALCFLAGS := \
    -I.                    \
    -std=c11               \
    -ffreestanding         \
    -fno-stack-protector   \
    -fno-stack-check       \
    -fno-pie               \
    -fno-pic               \
    -MMD

# Internal linker flags that should not be changed by the user.
override INTERNALLDFLAGS := \
	-Tlinker.ld             \
	-nostdlib               \
	-zmax-page-size=0x1000  \
	-static

# Internal assembler flags that should not be changed by the user.
override INTERNALASFLAGS := \
    -felf64

# Set architecture specific variables (and check that the architecture is supported).
ifeq ($(ARCH),x86_64)
    override INTERNALCFLAGS += \
        -m64                   \
        -march=x86-64          \
        -mabi=sysv             \
        -mno-80387             \
        -mno-mmx               \
        -mno-sse               \
        -mno-sse2              \
        -mno-red-zone          \
        -mcmodel=kernel
else
    $(error Architecture $(ARCH) not supported)
endif

# Use find to glob all *.c and *.asm files in the directory and extract the object names.
override CFILES := $(shell find ./ -type f -name '*.c')
override ASMFILES := $(shell find ./ -type f -name '*.asm')

override OBJ := $(CFILES:.c=.o) $(ASMFILES:.asm=.s.o)
override HEADER_DEPS := $(CFILES:.c=.d)

# Default target.
.PHONY: all
all: $(KERNEL)

limine.h:
	curl https://raw.githubusercontent.com/limine-bootloader/limine/trunk/limine.h -o $@

# Link rules for the final kernel executable.
$(KERNEL): $(OBJ)
	$(LD) $(LDFLAGS) $(INTERNALLDFLAGS) $^ -o $@

# Compilation rules for *.asm files.
%.s.o: %.asm
	$(AS) $(INTERNALASFLAGS) $< -o $@

# Compilation rules for *.c files.
-include $(HEADER_DEPS)
%.o: %.c limine.h
	$(CC) $(CFLAGS) $(INTERNALCFLAGS) -c $< -o $@

# Remove object files and the final executable.
.PHONY: clean
clean:
	rm -rf $(KERNEL) $(OBJ) $(HEADER_DEPS)

.PHONY: distclean
distclean: clean
	rm -f limine.h
