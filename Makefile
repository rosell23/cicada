# Copyright (c) 2025 The Cicada Project. All rights reserved.
#
# THE WORK IS PROVIDED “AS IS”, WITHOUT ANY WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES, OR OTHER LIABILITY ARISING FROM, OUT OF, OR IN CONNECTION
# WITH THE WORK OR ITS USE.

#== Config ==#

ARCH = x86_64

# Toolchain
CC  ?= $(ARCH)-elf-gcc
CXX ?= $(ARCH)-elf-g++
LD  ?= $(ARCH)-elf-ld
AS  ?= $(ARCH)-elf-as

QEMU ?= qemu-system-$(ARCH)

# Flags
CFLAGS ?= \
	-ffreestanding -O2 -Wall -Wextra -Iinclude \
	-fno-stack-protector -fno-builtin -fno-exceptions -fno-pic \
	-fno-unwind-tables -fomit-frame-pointer -m64 -no-pie

LDS     := linker.lds
LDFLAGS ?= -T $(LDS) -nostdlib -no-pie

#== Directories ==#

SRCDIR    := source
GEN_DIR   ?= gen
DISK_DIR  ?= iso

#== Sources ==#

CSRCS  := $(shell find $(SRCDIR) -name '*.c')
ASSRCS := $(shell find $(SRCDIR) -name '*.S')
FSRCS  := $(shell find $(SRCDIR) -name '*.4th')

COBJS  := $(patsubst $(SRCDIR)/%.c,$(GEN_DIR)/%.o,$(CSRCS))
ASOBJS := $(patsubst $(SRCDIR)/%.S,$(GEN_DIR)/%.o,$(ASSRCS))
FOBJS  := $(patsubst $(FORTH_DIR)/%.4th,$(GEN_DIR)/forth/%.o,$(FSRCS))
OBJS   := $(COBJS) $(ASOBJS) $(FOBJS)

#== Targets ==#

KERNEL ?= $(GEN_DIR)/kernel.elf
DISK   ?= $(DISK_DIR)/cicada.iso

#== Default ==#

.PHONY: all clean run
all: $(KERNEL) $(DISK)

#== Object build rules ==#

$(GEN_DIR):
	@mkdir -p $@

# Compile C sources
$(GEN_DIR)/%.o: $(SRCDIR)/%.c | $(GEN_DIR)
	@echo "CC	$< ==> $@"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble assembly sources
$(GEN_DIR)/%.o: $(SRCDIR)/%.S | $(GEN_DIR)
	@echo "AS	$< ==> $@"
	@mkdir -p $(dir $@)
	$(AS) $< -o $@

# Compile Forths
$(GEN_DIR)/forth/%.o: $(SRCDIR)/%.4th | $(GEN_DIR)/forth
	@echo "FORTH	$< ==> $@"
	@mkdir -p $(dir $@)
	# GForth doesn't really allow direct obj-file output, but it can output C.
	# TODO: implement

# Link kernel
$(KERNEL): $(OBJS)
	@echo "LD	$@"
	$(CC) $(LDFLAGS) -static -o $@ $^

#== Disk rules ==#

$(DISK): $(KERNEL)
	@echo "Creating GRUB ISO..."
	mkdir -p $(DISK_DIR)/boot/grub
	cp $(KERNEL) $(DISK_DIR)/boot/
	cp grub.cfg $(DISK_DIR)/boot/grub/
	grub-mkrescue -o $(GEN_DIR)/tmp.iso $(DISK_DIR)
	mv $(GEN_DIR)/tmp.iso $@

run: $(DISK)
	$(QEMU) -cdrom $(DISK)

#== Clean ==#

clean:
	rm -rf $(GEN_DIR) $(DISK_DIR)
