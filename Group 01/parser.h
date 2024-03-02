#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include <stdbool.h>
#include "parserDef.h"

const char *TokenToString(int token);
const char *NonTerminalToString(enum NonTerminals nonTerminal);
struct Variable *createCopy(struct Variable var);
void printStack(struct stack *st);
bool isEmptyStack(struct stack *st);
struct Variable *peek(struct stack *st);
bool onlyContainsEOF(struct stack *st);
bool isFull(struct stack *st);
void push(struct stack *st, struct Variable *data);
struct Variable *pop(struct stack *st);
void addSyn(struct ParsingTable *PT, struct Sets **sets_for_all, int nonTerminal, int *synchSet, int synchSetSize);
int parseInputSourceCode(struct SymbolTableEntry *token, struct ParsingTable *pt, struct stack *st, twinBuffer LA, struct tree_node *parent, bool skipError, struct tree_node **parentpointer);
struct stack *initialiseStack();
void computeFirstSet(struct Sets **sets_for_all, Productions productions);
void computeFollowSet(struct Sets **sets_for_all, Productions productions);
void createParseTable(struct ParsingTable *PT, Productions productions, struct Sets **sets_for_all, int *synchSet, int synchSetSize);
void printParsingTable(struct ParsingTable *PT);
void printFFSetsTable(FILE *file, struct Sets **sets_for_all);
void insertAllKeywords();
void serialize_tree(struct tree_node *root);
struct tree_node *create_tree_node(struct Variable *data);
bool isDefault(struct Variable var);
struct Sets **initialiseSetsWhole();
bool containsEPS(struct LinkedListSet *set);
