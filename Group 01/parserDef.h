#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#ifndef PARSERDEF_H
#define PARSERDEF_H

#define MAX_NUM_PRODUCTIONS 6
#define MAX_VARS 8
#define NUM_TERMINALS 68     // including epsilon
#define NUM_NON_TERMINALS 54 // num production rules (for unique non terminals)
#define SET_SIZE 200
#define STACK_INITIAL_SIZE 128

typedef struct parseTree parseTree;
typedef struct tree_node tree_node;
typedef struct Sets *FirstAndFollow;
typedef struct ParsingTable *table;
typedef struct Grammar *grammar;

struct Variable
{
    int val;
    int flag; // 0 FOR TERMINAL AND 1 FOR NONTERMINAL
    struct SymbolTableEntry *token;
};

struct LinkedListArray
{
    struct Node *head;
    int length;
};

struct LinkedListSet
{
    bool booleanArr[NUM_TERMINALS];
    struct LinkedListArray *linkedList;
};

struct stack
{
    struct Variable **stack;
    int top;
    int MAX;
};

struct input_structure
{
    struct Variable *nonterminal;
    struct Variable *rule;
    int length;
};

struct parseTree
{
    struct tree_node *root;
};

struct tree_node
{
    struct Variable *data;    // stores the variable that is represented by this node
    struct tree_node *next;   // stores a pointer to the next child of its parent node (next sibling)
    struct tree_node *head;   // stores pointer to its first child
    struct tree_node *parent; // stores a pointer back to its parent node
};

struct Node
{
    struct Variable data;
    struct Node *next;
};

struct Sets
{
    /// EACH SET HAS A BOOLEAN ARRAY AND A LINKED LIST
    struct LinkedListSet *firstSets;
    struct LinkedListSet *followSets;
};

struct GrammarRule
{
    // enum NonTerminals nonTerminal;
    int numProductions;
    struct Variable rules[MAX_NUM_PRODUCTIONS][MAX_VARS];
};

typedef struct Grammar
{
    struct GrammarRule productions[NUM_NON_TERMINALS];
} Grammar;

struct ParsingTable
{
    struct Variable *table[NUM_NON_TERMINALS][NUM_TERMINALS];
    // rule is array of variables of size maxvars
};

enum NonTerminals
{
  NT_PROGRAM,
  NT_MAIN_FUNCTION,
  NT_OTHER_FUNCTIONS,
  NT_FUNCTION,
  NT_INPUT_PAR,
  NT_OUTPUT_PAR,
  NT_PARAMETER_LIST,
  NT_DATA_TYPE,
  NT_PRIMITIVE_DATA_TYPE,
  NT_CONSTRUCTED_DATA_TYPE,
  NT_REMAINING_LIST,
  NT_STMTS,
  NT_TYPE_DEFINITIONS,
  NT_ACTUAL_OR_REDEFINED,
  NT_TYPE_DEFINITION,
  NT_FIELD_DEFINITIONS,
  NT_FIELD_DEFINITION,
  NT_FIELD_TYPE,
  NT_MORE_FIELDS,
  NT_DECLARATIONS,
  NT_DECLARATION,
  NT_GLOBAL_OR_NOT,
  NT_OTHER_STMTS,
  NT_STMT,
  NT_ASSIGNMENT_STMT,
  NT_SINGLE_OR_REC_ID,
  NT_OPTION_SINGLE_CONSTRUCTED,
  NT_ONE_EXPANSION,
  NT_MORE_EXPANSIONS,
  NT_FUN_CALL_STMT,
  NT_OUTPUT_PARAMETERS,
  NT_INPUT_PARAMETERS,
  NT_ITERATIVE_STMT,
  NT_CONDITIONAL_STMT,
  NT_ELSE_PART,
  NT_IO_STMT,
  NT_ARITHMETIC_EXPRESSION,
  NT_EXP_PRIME,
  NT_TERM,
  NT_TERM_PRIME,
  NT_FACTOR,
  NT_HIGH_PRECEDENCE_OPERATORS,
  NT_LOW_PRECEDENCE_OPERATORS,
  NT_BOOLEAN_EXPRESSION,
  NT_VAR,
  NT_LOGICAL_OP,
  NT_RELATIONAL_OP,
  NT_RETURN_STMT,
  NT_OPTIONAL_RETURN,
  NT_ID_LIST,
  NT_MORE_IDS,
  NT_DEFINETYPE_STMT,
  NT_A,
  NT_EPS
};

#endif