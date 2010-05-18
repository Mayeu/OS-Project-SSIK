# Makefile

# Object files for the examples
OBJS_KERNEL=
OBJS_USER=

# Project directory
PROJECT_DIR=.

# SRC directory
SRC=src
SRC_KERNEL=$(PROJECT_DIR)/$(SRC)/kernel
SRC_USER=$(PROJECT_DIR)/$(SRC)/user

# bin directory
BIN=bin

# GCC prefix
MIPS_PREFIX=/it/kurs/compsys/mips-devel/bin/mips-idt-elf

# gcc flags for the MIPS architecture:
#  -EL     : Little endian
#  -G0     : Always use small data (using register gp)
#  -mips32 : Compile for a 32-bit MIPS architecture
#
ARCH=-EL -G0 -mips32

# Other gcc flags
CFLAGS	+= -ggdb -Wall -Werror -I$(PROJECT_DIR)/include

# Compiler and linker commands
CC=$(MIPS_PREFIX)-gcc
LD=$(MIPS_PREFIX)-ld -Ttext 80020000

# Default indent, build the project and the doc
all: indent kernel user link doc

# Kernel building
kernel: $(OBJS_KERNEL)
	@echo "Kernel compilation\n"

$(BIN)/%.o: $(SRC_KERNEL)/%.c
	$(CC) $(ARCH) $(CFLAGS) -o $@ -c $<

$(BIN)/%.o: $(SRC_KERNEL)/%.s
	$(CC) $(ARCH) $(CFLAGS) -o $@ -c $<

# User building
user: $(OBJS_USER)
		@echo "User compilation\n"

$(BIN)/%.o: $(SRC_USER)/%.c
	$(CC) $(ARCH) $(CFLAGS) -o $@ -c $<

# link all the binarie
link: $(BIN)/ssik
		@echo "Binaries linking\n"

$(BIN)/ssik: $(OBJS_KERNEL) $(OBJS_USER)
	$(LD) $(ARCH) -o $@ $<

# Build the kernel doc and user doc
doc: kerneldoc userdoc

# Indent everything
indent: idt_include idt_user_h idt_user idt_kernel_h idt_kernel

# indent option
IDT_OPT=-gnu -bli0 -npcs -c33 -di16

idt_include: $(PROJECT_DIR)/include/%.h
	indent $(IDT_OPT) $<

idt_user_h: $(SRC_USER)/%.h
	indent $(IDT_OPT) $<

idt_user: $(SRC_USER)/%.c
	indent $(IDT_OPT) $<

idt_kernel_h: $(SRC_KERNEL)/%.h
	indent $(IDT_OPT) $<

idt_kernel: $(SRC_KERNEL)/%.c
	indent $(IDT_OPT) $<

# clean: remove object files and emacs backup files
clean:
	rm -f $(BIN)/*.o
