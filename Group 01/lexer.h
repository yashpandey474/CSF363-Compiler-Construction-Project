// Enumerated token types
#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"
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
