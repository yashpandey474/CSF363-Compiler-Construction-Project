// GROUP 01
// NAMES	                 BITS ID
// ------------------------------------------
// Yash Pandey	             2021A7PS0661P
// Achinthya Hebbar S	     2021A7PS1457P
// Ujjwal Aggarwal	         2021A7PS2427P
// Hardik Gupta 	         2021A7PS2421P
// Agrawal Vansh Anil        2021A7PS2215P

#include "lexerDef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LEXER_H
#define LEXER_H

struct SymbolTableEntry *getToken(struct SymbolTableEntry *symbolTableEntry);
int getNextState(int currentState, int character);
void insertAllKeywords();
void printSymbolTable();
void changeForward(lexicalAnalyser LA, int flag);
const char *CharacterTypeToString(enum CharacterType type);
twinBuffer initialiseTwinBuffer(FILE *file);
lexicalAnalyser initialiseLA(twinBuffer bufferArray);
int getStream(twinBuffer bufferArray);
tokenInfo getNextToken(lexicalAnalyser LA);

#endif
