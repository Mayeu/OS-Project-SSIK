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
BUILD=build

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
kernel: $(BUILD)/$(OBJS_KERNEL)
	@echo "Kernel compilation\n"

$(BUILD)/%.o: $(SRC_KERNEL)/%.c
	$(CC) $(ARCH) $(CFLAGS) -o $@ -c $<

$(BUILD)/%.o: $(SRC_KERNEL)/%.s
	$(CC) $(ARCH) $(CFLAGS) -o $@ -c $<

# User building
user: $(BUILD)/$(OBJS_USER)
		@echo "User compilation\n"

$(BUILD)/%.o: $(SRC_USER)/%.c
	$(CC) $(ARCH) $(CFLAGS) -o $@ -c $<

# link all the binarie
link: $(BIN)/ssik
		@echo "Binaries linking\n"

$(BIN)/ssik: $(BUILD)/$(OBJS_KERNEL) $(BUILD)/$(OBJS_USER)
	$(LD) $(ARCH) -o $@ $<

# Run the program
run:
	bash $(PROJECT_DIR)/scripts/run.sh $(BIN)/ssik

# Build the kernel doc and user doc
doc: kerneldoc userdoc

# Indent everything
# indent option
IDT_OPT=-gnu -bli0 -npcs -c33 -di16 -nut

indent:
	indent $(IDT_OPT) $(PROJECT_DIR)/include/*.h
	indent $(IDT_OPT) $(SRC_KERNEL)/*.h
	indent $(IDT_OPT) $(SRC_KERNEL)/*.c
	indent $(IDT_OPT) $(SRC_USER)/*.h
	indent $(IDT_OPT) $(SRC_USER)/*.c
	
# clean: remove object files and emacs backup files
clean:
	rm -f $(BIN)/*.o
