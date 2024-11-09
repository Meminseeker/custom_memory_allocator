# Compiler
CC = clang

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Target executable
TARGET = main

# Source and object files
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

# Default rule
all: clean compile

run: clean compile
	./$(TARGET)

compile: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Pattern rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up
clean:
	rm -f $(TARGET) $(OBJS)
