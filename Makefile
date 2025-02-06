# DOS Redux Makefile

# Compiler settings
CC = gcc
ASM = nasm
CFLAGS = -Wall -Wextra -I./include
ASMFLAGS = -f elf32

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Default target
all: bootloader kernel shell

# Bootloader
bootloader:
	@echo "Building bootloader..."
	$(ASM) $(ASMFLAGS) $(SRC_DIR)/bootloader/boot.asm -o $(BUILD_DIR)/boot.o

# Kernel
kernel:
	@echo "Building kernel..."
	$(CC) $(CFLAGS) -c $(SRC_DIR)/kernel/kernel.c -o $(BUILD_DIR)/kernel.o

# Shell
shell:
	@echo "Building shell..."
	$(CC) $(CFLAGS) -c $(SRC_DIR)/shell/shell.c -o $(BUILD_DIR)/shell.o

# Clean build files
clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: all bootloader kernel shell clean 