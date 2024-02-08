#!/bin/bash

# Define the compiler
CC=gcc

# Define any compile-time flags
CFLAGS="-Wall -g"

# Define the source files
SOURCES="symbol_table.c transition_table.c read_input.c"

# Define the header files directory if necessary
# Include it in compilation with -I option in case it's not in the current directory
INCLUDE_DIR="."

# Define the output executable name
OUTPUT="myProgram"

# Compile the program
echo "Compiling $SOURCES into $OUTPUT..."
$CC $CFLAGS -I$INCLUDE_DIR $SOURCES -o $OUTPUT

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running $OUTPUT..."
    ./$OUTPUT
else
    echo "Compilation failed."
fi
