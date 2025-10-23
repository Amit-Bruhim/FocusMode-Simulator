# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Wno-unused-parameter

# Directories
SRC_DIR = src
BIN_DIR = bin
TARGET = $(BIN_DIR)/FocusMode

# Source file
SRC = $(SRC_DIR)/Focus-Mode.c

# Default target: build
all: $(TARGET)

# Compile the binary
$(TARGET): $(SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Run the program with default arguments
run: $(TARGET)
	$(TARGET) Focus-Mode 2 2

# Remove the binary folder
clean:
	rm -rf $(BIN_DIR)
