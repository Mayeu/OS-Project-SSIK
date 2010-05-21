# Makefile

# Project directory
PROJECT_DIR=.

# SRC directory
SRC=src
SRC_KERNEL=$(PROJECT_DIR)/$(SRC)/kernel
SRC_USER=$(PROJECT_DIR)/$(SRC)/user

# bin directory
BIN=bin
BUILD=build

# Object files for the examples
OBJS_KERNEL= $(addprefix $(BUILD)/, kernel.o asm.o debug.o)
OBJS_USER=

# GCC prefix
MIPS_PREFIX=/it/sw/cross/mips-idt/bin/mips-idt-elf

# gcc flags for the MIPS architecture:
#  -EL     : Little endian
#  -G0     : Always use small data (using register gp)
#  -mips32 : Compile for a 32-bit MIPS architecture
#
ARCH=-EL -G0 -mips32

# Other gcc flags
CFLAGS	= -ggdb -Wall -Werror -fno-builtin -I$(PROJECT_DIR)/include
KCFLAGS	+= $(CFLAGS) -I$(PROJECT_DIR)/$(SRC_KERNEL)/include

# Compiler and linker commands
CC=$(MIPS_PREFIX)-gcc
LD=$(MIPS_PREFIX)-ld -Ttext 80020000

# Default indent, build the project and the doc
all: indent kernel user link

# Kernel building
kernel: $(OBJS_KERNEL) indent

$(BUILD)/%.o: $(SRC_KERNEL)/%.c
	$(CC) $(ARCH) $(KCFLAGS) -o $@ -c $<

$(BUILD)/%.o: $(SRC_KERNEL)/%.S
	$(CC) $(ARCH) $(KCFLAGS) -o $@ -c $<
	
# User building
user: $(BUILD)/$(OBJS_USER) kernel

$(BUILD)/%.o: $(SRC_USER)/%.c
	$(CC) $(ARCH) $(CFLAGS) -o $@ -c $<

# link all the binarie
link: $(BIN)/ssik indent kernel user

$(BIN)/ssik: $(OBJS_KERNEL) $(OBJS_USER)
	$(LD) $(ARCH) -o $@ $^

# Run the program
run: link
	bash $(PROJECT_DIR)/scripts/run.sh $(BIN)/ssik

# Indent everything
# indent option
IDT_OPT=-gnu -bli0 -npcs -c33 -di16 -nut

indent:
	indent $(IDT_OPT) $(PROJECT_DIR)/include/*.h
	indent $(IDT_OPT) $(SRC_KERNEL)/*.h
	indent $(IDT_OPT) $(SRC_KERNEL)/include/*.h
	indent $(IDT_OPT) $(SRC_KERNEL)/*.c
	indent $(IDT_OPT) $(SRC_USER)/*.h
	indent $(IDT_OPT) $(SRC_USER)/*.c
	
# Build the kernel doc and user doc
doc: doc_kernel doc_user

doc_kernel:
	mkdir -p doc/kernel
	doxygen doc_conf_kernel

doc_user:
	mkdir -p doc/user
	doxygen doc_conf_user

# clean: remove object files and emacs backup files
clean:
	rm -rf $(BUILD)/*.o $(BIN)/ssik doc/*
