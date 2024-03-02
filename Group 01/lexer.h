// Enumerated token types
#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"
#include <string.h>
#ifndef LEXER
#define LEXER_H
struct SymbolTableEntry * getToken(struct SymbolTableEntry *symbolTableEntry);
int getNextState(int currentState, int character);
void insertAllKeywords();
void printSymbolTable();
void changeForward(twinBuffer LA, int flag);
const char *CharacterTypeToString(enum CharacterType type);
const char *TokenToString(int token);
FILE *readTestFile(char *file_path);
twinBufferArray initialiseTwinBuffer(FILE *file);
twinBuffer initialiseLA(twinBufferArray bufferArray);
int getStream(twinBufferArray bufferArray);
tokenInfo getNextToken(twinBuffer LA);

#endif
