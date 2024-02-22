#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "syntactical.h"

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
        return "EPSILON";

    case TK_EOF:
        return "TK_EOF";

    default:
        return "non accept state";
    }
}


struct node{
    struct Variable *value;
    int type; // 0 for non-terminal, 1 for terminal
    struct node** children;  //children can be terminals or non_terminals
    // struct node children[];   
    int number_of_children; //to store the number of children
    struct node* parent;
};

struct node* create_node(struct Variable *value){
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->value = value;
    new_node->children = NULL;
    new_node->number_of_children = 0;
    new_node->parent = NULL;
    return new_node;
}

//if the value returned is 0, then the child is successfully added
int add_child(struct node *parent, struct node *child){
    if (parent->value->flag == 0){
        printf("Error: Parent is a terminal\n");
        return -1;
    }
    
    struct node** temp = (struct node**)realloc(parent->children, (parent->number_of_children+1)*sizeof(struct node*));
    if (temp == NULL) {
        printf("Error: Memory reallocation failed\n");
        return -1;
    }
    parent->children = temp;
    parent->children[parent->number_of_children] = child; 
    parent->number_of_children++;
    return  0;
}

// Add multiple children to the parent node
// Returns 0 if all children are successfully added, -1 

int add_parent_children(struct node *parent, struct Variable *children, int num_children) {
    if (parent == NULL || children == NULL || num_children <= 0) {
        printf("Error: Invalid input\n");
        return -1;
    }

    for (int i = 0; i < num_children; i++) {
        struct node* temp = 
        if (add_child(parent, children[i]) == -1) {
            // Error occurred while adding child, rollback and return -1
            for (int j = 0; j < i; j++) {
                free(parent->children[parent->number_of_children - 1]);
                parent->number_of_children--;
            }
            return -1;
        }
    }
    return 0;
}

void print_tree(struct node *root){
    if (root->value->flag == 0){
        printf("%s\n", TokenToString(root->value->val));
    }
    else{
        printf("%s\n", NonTerminalToString(root->value->val));
        for (int i = 0; i < root->number_of_children; i++){
            print_tree(root->children[i]);
        }
    }
}

int main(){
    //add a few nodes and check if the tree is being created properly
    
    return 0;
}