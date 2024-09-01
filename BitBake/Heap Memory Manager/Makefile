include sources.mk

CC = gcc
CFLAGS = -g
CPPFLAGS = $(INCLUDES)

TARGET= FINAL

PREP = $(SOURCES:.c=.i)		# Preprocessed Files
DEPS = $(SOURCES:.c=.d)		# Dependancy Files
ASMS = $(SOURCES:.c=.asm)	# Assembly Files
OBJS = $(SOURCES:.c=.o)		# Object files

.PHONY: compile-all build clean

# Compile all object files and link into a final executable.
build: $(TARGET).out

$(TARGET).out: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(CPPFLAGS) -o $@

# Generate Preprocesed output of all c-program implementation files (use the -E flag)
%.i : %.c
	$(CC) -E $< $(CFLAGS) $(CPPFLAGS) -o $@

# Generate assembly output of c-program implementation files (Use the –S flag).
%.asm : %.c
	$(CC) -S $< $(CFLAGS) $(CPPFLAGS) -o $@

# Generate the object file for all c-source files (but do not link) by specifying the object file you want to compile.
%.o : %.c
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) -o $@

# Generate Dependancy files for each Source File
%.d: %.c
	$(CC) -E -M $< $(CPPFLAGS) -o $@

# Compile all object files, but DO NOT link.
compile-all : $(OBJS)

# This should remove all compiled objects, preprocessed outputs, assembly outputs, executable files and build output files.
clean:
	@echo Removing all built files
	rm -f *.out *.map *.o *.asm *.i