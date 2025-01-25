# Compiler and flags
CC = gcc
CFLAGS = -Wall -g
LDFLAGS =

# Source and object files
SRCS = main.c history.c alias.c environment.c executer.c
OBJS = $(SRCS:.c=.o)

# Output binary name
TARGET = shell

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Additional convenience commands
run: $(TARGET)
	./$(TARGET)

rebuild: clean all

