# Makefile for Raylib Falling Sand Game

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I/usr/local/include -D_DEFAULT_SOURCE # Adjust include path if Raylib is elsewhere
LDFLAGS = -L/usr/local/lib -lraylib -lm -lpthread -ldl -lrt -lX11 # Adjust lib path if Raylib is elsewhere

# Project name
PROJECT_NAME = falling_sand

# Source files
SRC_DIR = src
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ_DIR = obj
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Executable
EXECUTABLE = $(PROJECT_NAME)

# Default target
all: $(EXECUTABLE)

# Rule to build executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)

# Phony targets
.PHONY: all clean

# Run target (optional)
run: $(EXECUTABLE)
	./$(EXECUTABLE)
