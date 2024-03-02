# Define the compiler
CC=gcc

# Define compiler flags if any (e.g., -Wall for all warnings, -g for debugging)
CFLAGS=-Wall -g

# Define the source files explicitly, including the path for those in the "Lexical Analyzer" directory
# Note: Adjust the paths according to your directory structure
SOURCES=ffsets.c \
        parsing_table.c \
        parseInputSourceCode.c \
        syntax_tree.c\
        ../Lexical\ Analyzer/symbol_table.c \
        ../Lexical\ Analyzer/transition_table.c \
        ../Lexical\ Analyzer/read_input.c

# Define the executable file name
EXECUTABLE=stage2exe

# Default rule for making the executable
all: $(EXECUTABLE)

# Rule for compiling and linking the executable directly without generating .o files
$(EXECUTABLE):
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

# Define a clean rule
clean:
	rm -f $(EXECUTABLE)
