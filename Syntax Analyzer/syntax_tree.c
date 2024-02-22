#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntactical.h"

// functions to use for help with printing
const char *NonTerminalToString(enum NonTerminals nonTerminal)
{
    switch (nonTerminal)
    {
    case NT_PROGRAM:
        return "NT_PROGRAM";
    case NT_MAIN_FUNCTION:
        return "NT_MAIN_FUNCTION";
    case NT_OTHER_FUNCTIONS:
        return "NT_OTHER_FUNCTIONS";
    case NT_FUNCTION:
        return "NT_FUNCTION";
    case NT_INPUT_PAR:
        return "NT_INPUT_PAR";
    case NT_OUTPUT_PAR:
        return "NT_OUTPUT_PAR";
    case NT_PARAMETER_LIST:
        return "NT_PARAMETER_LIST";
    case NT_DATA_TYPE:
        return "NT_DATA_TYPE";
    case NT_PRIMITIVE_DATA_TYPE:
        return "NT_PRIMITIVE_DATA_TYPE";
    case NT_CONSTRUCTED_DATA_TYPE:
        return "NT_CONSTRUCTED_DATA_TYPE";
    case NT_REMAINING_LIST:
        return "NT_REMAINING_LIST";
    case NT_STMTS:
        return "NT_STMTS";
    case NT_TYPE_DEFINITIONS:
        return "NT_TYPE_DEFINITIONS";
    case NT_TYPE_DEFINITION:
        return "NT_TYPE_DEFINITION";
    case NT_FIELD_DEFINITIONS:
        return "NT_FIELD_DEFINITIONS";
    case NT_FIELD_DEFINITION:
        return "NT_FIELD_DEFINITION";
    case NT_MORE_FIELDS:
        return "NT_MORE_FIELDS";
    case NT_DECLARATIONS:
        return "NT_DECLARATIONS";
    case NT_DECLARATION:
        return "NT_DECLARATION";
    case NT_GLOBAL_OR_NOT:
        return "NT_GLOBAL_OR_NOT";
    case NT_OTHER_STMTS:
        return "NT_OTHER_STMTS";
    case NT_STMT:
        return "NT_STMT";
    case NT_ASSIGNMENT_STMT:
        return "NT_ASSIGNMENT_STMT";
    case NT_SINGLE_OR_REC_ID:
        return "NT_SINGLE_OR_REC_ID";
    case NT_REC_ID:
        return "NT_REC_ID";
    case NT_FUN_CALL_STMT:
        return "NT_FUN_CALL_STMT";
    case NT_OUTPUT_PARAMETERS:
        return "NT_OUTPUT_PARAMETERS";
    case NT_INPUT_PARAMETERS:
        return "NT_INPUT_PARAMETERS";
    case NT_ITERATIVE_STMT:
        return "NT_ITERATIVE_STMT";
    case NT_CONDITIONAL_STMT:
        return "NT_CONDITIONAL_STMT";
    case NT_NEW3:
        return "NT_NEW3";
    case NT_IO_STMT:
        return "NT_IO_STMT";
    case NT_ARITHMETIC_EXPRESSION:
        return "NT_ARITHMETIC_EXPRESSION";
    case NT_NEW5:
        return "NT_NEW5";
    case NT_TERM:
        return "NT_TERM";
    case NT_NEW6:
        return "NT_NEW6";
    case NT_FACTOR:
        return "NT_FACTOR";
    case NT_OPERATOR:
        return "NT_OPERATOR";
    case NT_OP_H:
        return "NT_OP_H";
    case NT_BOOLEAN_EXPRESSION:
        return "NT_BOOLEAN_EXPRESSION";
    case NT_VAR:
        return "NT_VAR";
    case NT_LOGICAL_OP:
        return "NT_LOGICAL_OP";
    case NT_RELATIONAL_OP:
        return "NT_RELATIONAL_OP";
    case NT_RETURN_STMT:
        return "NT_RETURN_STMT";
    case NT_OPTIONAL_RETURN:
        return "NT_OPTIONAL_RETURN";
    case NT_ID_LIST:
        return "NT_ID_LIST";
    case NT_MORE_IDS:
        return "NT_MORE_IDS";
    case NT_DEFINETYPE_STMT:
        return "NT_DEFINETYPE_STMT";
    case NT_A:
        return "NT_A";

    default:
        return "Unknown NonTerminal";
    }
}

const char *TokenToString(enum Tokentype token)
{
    switch (token)
    {
    case CARRIAGE_RETURN:
        return "CARRIAGE_RETURN";
    case TK_ASSIGNOP:
        return "TK_ASSIGNOP";
    case TK_COMMENT:
        return "TK_COMMENT";
    case TK_WITH:
        return "TK_WITH";
    case TK_PARAMETERS:
        return "TK_PARAMETERS";
    case TK_END:
        return "TK_END";
    case TK_WHILE:
        return "TK_WHILE";
    case TK_UNION:
        return "TK_UNION";
    case TK_ENDUNION:
        return "TK_ENDUNION";
    case TK_DEFINETYPE:
        return "TK_DEFINETYPE";
    case TK_AS:
        return "TK_AS";
    case TK_TYPE:
        return "TK_TYPE";
    case TK_MAIN:
        return "TK_MAIN";
    case TK_GLOBAL:
        return "TK_GLOBAL";
    case TK_PARAMETER:
        return "TK_PARAMETER";
    case TK_LIST:
        return "TK_LIST";
    case TK_INPUT:
        return "TK_INPUT";
    case TK_OUTPUT:
        return "TK_OUTPUT";
    case TK_INT:
        return "TK_INT";
    case TK_REAL:
        return "TK_REAL";
    case TK_ENDWHILE:
        return "TK_ENDWHILE";
    case TK_IF:
        return "TK_IF";
    case TK_THEN:
        return "TK_THEN";
    case TK_ENDIF:
        return "TK_ENDIF";
    case TK_READ:
        return "TK_READ";
    case TK_WRITE:
        return "TK_WRITE";
    case TK_RETURN:
        return "TK_RETURN";
    case TK_CALL:
        return "TK_CALL";
    case TK_RECORD:
        return "TK_RECORD";
    case TK_ENDRECORD:
        return "TK_ENDRECORD";
    case TK_ELSE:
        return "TK_ELSE";
    case TK_COMMA:
        return "TK_COMMA";
    case TK_SEM:
        return "TK_SEM";
    case TK_COLON:
        return "TK_COLON";
    case TK_DOT:
        return "TK_DOT";
    case TK_OP:
        return "TK_OP";
    case TK_CL:
        return "TK_CL";
    case TK_SQR:
        return "TK_SQR";
    case TK_SQL:
        return "TK_SQL";
    case TK_PLUS:
        return "TK_PLUS";
    case TK_MINUS:
        return "TK_MINUS";
    case TK_MUL:
        return "TK_MUL";
    case TK_DIV:
        return "TK_DIV";
    case TK_AND:
        return "TK_AND";
    case TK_OR:
        return "TK_OR";
    case TK_NOT:
        return "TK_NOT";
    case TK_LT1:
    case TK_LT2:
    case TK_LT:
        return "TK_LT";
    case TK_LE:
        return "TK_LE";
    case TK_EQ:
        return "TK_EQ";
    case TK_GT:
        return "TK_GT";
    case TK_GE:
        return "TK_GE";
    case TK_NE:
        return "TK_NE";
    case TK_FIELDID:
        return "TK_FIELDID";
    case TK_ID:
        return "TK_ID";
    case TK_NUM1:
    case TK_NUM2:
    case TK_NUM:
        return "TK_NUM";
    case TK_RNUM1:
    case TK_RNUM2:
    case TK_RNUM:
        return "TK_RNUM";
    case TK_FUNID:
        return "TK_FUNID";
    case TK_RUID:
        return "TK_RUID";
    case DELIMITER:
        return "DELIMITER";
    case TK_EPS:
        return "TK_EPS";
    case TK_EOF:
        return "TK_EOF";
    case LEXICAL_ERROR:
        return "LEXICAL_ERROR";
    case -1:
        return "SYN TOKEN";

    default:
        return "non accept state";
    }
}

// I am assuming that the function which gives me the rule to be added returns a structure which has
// a field called nonterminal whose value is the non-terminal whose rule is being added
// the other field in the structure is an array of variables which is the reverse ordered rule called rule
// another field is the length of the rule called length
struct input_structure
{
    struct Variable nonterminal;
    struct Variable *rule;
    int length;
};

// declaration of function to fetch rule from the predictive parsing table
// add its definition
struct input_structure fetch_rule();

// structure for the tree node
struct tree_node
{
    struct Variable data;     // stores the variable that is represented by this node
    struct tree_node *next;   // stores a pointer to the next child of its parent node (next sibling)
    struct tree_node *head;   // stores pointer to its first child
    struct tree_node *parent; // stores a pointer back to its parent node
};

// function to create a new tree_node
struct tree_node *create_tree_node(struct Variable data)
{
    struct tree_node *new_tree_node = (struct tree_node *)malloc(sizeof(struct tree_node *));
    new_tree_node->data = data;
    new_tree_node->next = NULL;
    new_tree_node->head = NULL;
    new_tree_node->parent = NULL;
    return new_tree_node;
}

// function to add a tree_node to the linkedlist which functions as the children of the parse tree
// the tree_nodes are added to the head (they are in reverse order)
void add_tree_node(struct tree_node *parent, struct tree_node *child)
{
    child->next = parent->head;
    parent->head = child;
    child->parent = parent;
}

// function to find the next non terminal for a given node
struct tree_node *nextNonTerminal(struct tree_node *current)
{
    while (current->next != NULL && current->data.flag == 0)
    {
        current = current->next;
    }
    if (current->data.flag == 1)
    {
        return current;
    }
    return nextNonTerminal(current->parent->next);
}

// function which takes the structure described in the first few comments as input and the tree_node where the rule has to be added
struct tree_node *repeated_add(struct tree_node *parent, struct input_structure input)
{
    if (parent->data.val != input.nonterminal.val)
    {
        printf("Error: The input does not match the first non-terminal found\n");
        printf("Non-terminal entered: %s\n", NonTerminalToString(input.nonterminal.val));
        printf("Non-terminal found: %s\n", NonTerminalToString(parent->data.val));
        return -1;
    }

    for (int i = 0; i < input.length; i++)
    {
        add_tree_node(parent, create_tree_node(input.rule[i]));
    }

    return nextNonTerminal(parent->head);
}

int main()
{ // initialise the root_node which is the NT_PROGRAM node and add the first input_structure to it using repeated_add
    struct tree_node *node_to_add_to = create_tree_node((struct Variable){0, 1});
    while (1)
    {
        struct input_structure input = fetch_rule();
        node_to_add_to = repeated_add(node_to_add_to, input);
    }
    return 0;
}