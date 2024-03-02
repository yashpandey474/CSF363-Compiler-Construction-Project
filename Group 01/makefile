# Define compiler
CC=gcc

# Define any compile-time flags
CFLAGS=-I. -I"./Lexical Analyzer" -Wall

# Define the source files explicitly, including the path for those in the "Lexical Analyzer" directory
# Note: Use the actual paths to the source files
SOURCES=driver.c \
        "./Lexical Analyzer/symbol_table.c" \
        "./Lexical Analyzer/transition_table.c" \
        "./Lexical Analyzer/lexer.c"

# Define the executable file name
EXECUTABLE=stage1exe

# Default rule for making the executable
all: $(EXECUTABLE)

# Rule for compiling and linking the executable directly without generating .o files
$(EXECUTABLE): 
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

# Define a clean rule
clean:
	rm -f $(EXECUTABLE)
