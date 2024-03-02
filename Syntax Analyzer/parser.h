#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parserDef.h"
#include <stdbool.h>

#ifndef PARSER
#define PARSER_H
#define MAX_NUM_PRODUCTIONS 6
#define MAX_VARS 9
#define NUM_TERMINALS 67     // including epsilon
#define NUM_NON_TERMINALS 49 // num productions
#define SET_SIZE 200

const char *TokenToString(int token);
const char *NonTerminalToString(enum NonTerminals nonTerminal);
struct stack *initialiseStack();
struct Variable *createCopy(struct Variable var);
void printStack(struct stack *st);
bool isEmptyStack(struct stack *st);
struct Variable *peek(struct stack *st);
bool onlyContainsEOF(struct stack *st);
bool isFull(struct stack *st);
void push(struct stack *st, struct Variable *data);
struct Variable *pop(struct stack *st);
int parseInputSourceCode(struct SymbolTableEntry *token, struct ParsingTable *pt, struct stack *st, twinBuffer LA, struct tree_node *parent, bool skipError, struct tree_node **parentpointer);
struct stack *initialiseStack();
void computeFirstSet(FirstAndFollow *sets_for_all, struct GrammarRule *G);
void computeFollowSet(FirstAndFollow *sets_for_all, struct GrammarRule *G);
void createParseTable(struct ParsingTable *PT, struct GrammarRule *productions, struct Sets **sets_for_all, int *synchSet, int defaultSynchSetSize);
void printParsingTable(struct ParsingTable *PT);
void printFFSetsTable(FILE *file, struct Sets **sets_for_all);
void insertAllKeywords();
FILE *readTestFile(const char *filename);
void printTree(struct tree_node *root, int depth);
void printParseTree(parseTree *PT, char *outfile);
void serialize_tree(struct tree_node *root);
void print_and_parse_tree(char *filename, FirstAndFollow *sets, struct ParsingTable *PT, Grammar G);
struct tree_node *create_tree_node(struct Variable *data);
FirstAndFollow *computeFirstAndFollow(struct GrammarRule *productions);
void addSyn(struct ParsingTable *PT, struct Sets **sets_for_all, int nonTerminal, int *synchSet, int defaultSynchSetSize);
#endif