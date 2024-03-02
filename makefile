# Define compiler
CC=gcc

# Define any compile-time flags
# Assuming the "Lexical Analyzer" directory is not used anymore based on your description
CFLAGS=-I. -I"./Group 01" -I"./Group 01/Other Files" -Wall

# Define the source files explicitly, including the path for those in the "Group 01" and "Other Files" directory
SOURCES=./Group\ 01/driver.c \
        ./Group\ 01/lexer.c \
        ./Group\ 01/parser.c \
        ./Group\ 01/Other\ Files/symbol_table.c \
        ./Group\ 01/Other\ Files/transition_table.c \
        ./Group\ 01/Other\ Files/ffsets.c

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
