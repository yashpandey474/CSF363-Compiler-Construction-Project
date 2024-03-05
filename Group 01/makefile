# // GROUP 01
# // NAMES	                 BITS ID
# // ------------------------------------------
# Yash Pandey	                2021A7PS0661P
# Achinthya Hebbar S	        2021A7PS1457P
# Ujjwal Aggarwal	        2021A7PS2427P
# Hardik Gupta 	                2021A7PS2421P
# Agrawal Vansh Anil            2021A7PS2215P



CFLAGS=-I. -I"./Group 01" -Wall -g

# Define the source files
SOURCES=driver.c \
        lexer.c \
        parser.c \
        symbol_table.c \
        transition_table.c 

# Define the executable file name
EXECUTABLE=stage1exe

# Default rule
all: clean $(EXECUTABLE)

# Rule for compiling and linking the executable directly without generating .o files
$(EXECUTABLE):
	gcc $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
