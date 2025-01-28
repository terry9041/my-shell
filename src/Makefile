# Makefile for shell.c

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Target executable
TARGET = myshell

# Source files
SRC = shell.c

# Build the target
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# Clean up generated files
clean:
	rm -f $(TARGET)
