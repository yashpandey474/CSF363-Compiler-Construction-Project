#include "../Lexical Analyzer/lexical.h"
#include <stdbool.h>
#define MAX_NUM_PRODUCTIONS 6
#define MAX_VARS 9
#define NUM_TERMINALS 67     // including epsilon
#define NUM_NON_TERMINALS 49 // num productions
#define SET_SIZE 200
#include <stdio.h>
#include <stdlib.h>

struct Variable
{
  int val;
  int flag; // 0 FOR TERMINAL AND 1 FOR NONTERMINAL
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

struct input_structure
{
  struct Variable nonterminal;
  struct Variable *rule;
  int length;
};

struct tree_node
{
  struct Variable data;     // stores the variable that is represented by this node
  struct tree_node *next;   // stores a pointer to the next child of its parent node (next sibling)
  struct tree_node *head;   // stores pointer to its first child
  struct tree_node *parent; // stores a pointer back to its parent node
};

void insertAtBeginning(struct LinkedListArray *linkedList, int newData, int flag);
struct Node *createLLNode(struct Variable data);
bool containsEPS(struct LinkedListSet *set);
bool isDefault(struct Variable var);

// structure of Linked list node
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
  NT_TYPE_DEFINITION,
  NT_FIELD_DEFINITIONS,
  NT_FIELD_DEFINITION,
  NT_MORE_FIELDS,
  NT_DECLARATIONS,
  NT_DECLARATION,
  NT_GLOBAL_OR_NOT,
  NT_OTHER_STMTS,
  NT_STMT,
  NT_ASSIGNMENT_STMT,
  NT_SINGLE_OR_REC_ID,
  NT_REC_ID,
  NT_FUN_CALL_STMT,
  NT_OUTPUT_PARAMETERS,
  NT_INPUT_PARAMETERS,
  NT_ITERATIVE_STMT,
  NT_CONDITIONAL_STMT,
  NT_NEW3,
  NT_IO_STMT,
  NT_ARITHMETIC_EXPRESSION,
  NT_NEW5,
  NT_TERM,
  NT_NEW6,
  NT_FACTOR,
  NT_OPERATOR,
  NT_OP_H,
  NT_BOOLEAN_EXPRESSION,
  NT_VAR,
  NT_LOGICAL_OP,
  NT_RELATIONAL_OP,
  NT_RETURN_STMT,
  NT_OPTIONAL_RETURN,
  NT_ID_LIST,
  NT_MORE_IDS,
  NT_DEFINETYPE_STMT,
  NT_A
};

const char *NonTerminalToString(enum NonTerminals nonTerminal);

struct Sets **initialiseSetsWhole();
void computeFirstSet(struct Sets **sets_for_all, struct GrammarRule *productions);
void computeFollowSet(struct Sets **sets_for_all, struct GrammarRule *productions);
void populate_parsing_table(struct ParsingTable *PT, struct GrammarRule *productions, struct Sets **sets_for_all);
void printParsingTable(struct ParsingTable *pt);
void printFFSetsTable(FILE *cfile, struct Sets **sets_for_all);
void printRule(enum NonTerminals nt, struct Variable *ruleArray);
struct tree_node *add_to_tree(struct Variable nt, int a, struct ParsingTable *pt, struct tree_node *parent);
struct tree_node *create_tree_node(struct Variable data);
void printTree(struct tree_node *root, int depth);
void serialize_tree(struct tree_node *root);