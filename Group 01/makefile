# Define compiler
CC=gcc

# Define any compile-time flags
# Assuming the "Lexical Analyzer" directory is not used anymore based on your description
CFLAGS=-I. -I"./Group 01" -I"./Group 01/Other Files" -Wall -g

# Define the source files explicitly, including the path for those in the "Group 01" and "Other Files" directory
SOURCES=driver.c \
        lexer.c \
        parser.c \
        Other\ Files/symbol_table.c \
        Other\ Files/transition_table.c 

# Define the executable file name
EXECUTABLE=stage1exe

# Default rule for making the executable
all: clean $(EXECUTABLE)

# Rule for compiling and linking the executable directly without generating .o files
$(EXECUTABLE):
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

# Define a clean rule
clean:
	rm -f $(EXECUTABLE)
