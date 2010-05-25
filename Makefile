# Makefile

# Project directory
PROJECT_DIR=.

# SRC directory
SRC=src
SRC_KERNEL=$(PROJECT_DIR)/$(SRC)/kernel
SRC_USER=$(PROJECT_DIR)/$(SRC)/user
SRC_TEST=$(PROJECT_DIR)/$(SRC)/test

# bin directory
BIN=bin
BUILD=build

# Object files for the examples
OBJS_KERNEL= $(addprefix $(BUILD)/, kernel.o asm.o debug.o kinout.o kerror.o kpcb.o kprocess.o kprocess_list.o kprogram.o kscheduler.o ksyscall.o kexception.o uart.o splash.o)
OBJS_USER= $(addprefix $(BUILD)/, string.o)
OBJS_TEST= $(addprefix $(BUILD)/, test.o)
TEST_DEPS= $(addprefix $(SRC_TEST)/, test_string.c test_uart_fifo.c test_kprogram.c test_kpcb.c test_kprocess2.c test_kprocess.c test_kprocess_list2.c test_kprocess_list.c)

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
all: link
nolink: test

# Kernel building
kernel: $(OBJS_KERNEL)

$(BUILD)/%.o: $(SRC_KERNEL)/%.c
	$(CC) $(ARCH) $(KCFLAGS) -o $@ -c $<

$(BUILD)/%.o: $(SRC_KERNEL)/%.S
	$(CC) $(ARCH) $(KCFLAGS) -o $@ -c $<
	
# User building
user: $(OBJS_USER) kernel

$(BUILD)/%.o: $(SRC_USER)/%.c
	$(CC) $(ARCH) $(CFLAGS) -o $@ -c $<
	
# Test building
test: $(OBJS_TEST) kernel user

$(BUILD)/test.o: $(SRC_TEST)/test.c $(TEST_DEPS)
	$(CC) $(ARCH) $(KCFLAGS) -o $@ -c $<

# link all the binarie
link: $(BIN)/ssik kernel user test

$(BIN)/ssik: $(OBJS_KERNEL) $(OBJS_USER) $(OBJS_TEST)
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
	rm -rf $(BUILD)/*.o $(BIN)/ssik doc/* $(SRC_KERNEL)/*~ $(SRC_KERNEL)/include/*~ $(SRC_USER)/*~ include/*~

gdb:
	/it/kurs/compsys/mips-devel/bin/mips-idt-elf-gdb bin/ssik
