
//MAKE AN ENUM FOR NONTERMINALS#

#include "syntactical.h"
#define MAX_NUM_PRODUCTIONS 6
#define MAX_VARS 10
#define NUM_NON_TERMINALS 51 // num productions
#define MAX_FIRST_SET 100
#define MAX_FOLLOW_SET 100
//STRUCTURE FOR A GRAMMAR

struct Variable{
    int val;
    int flag; //0 FOR TERMINAL AND 1 FOR NONTERMINAL
};


struct Sets{
    struct Variable firstSets[NUM_NON_TERMINALS][MAX_FIRST_SET];
    struct Variable followSets[NUM_NON_TERMINALS][MAX_FOLLOW_SET];
};

struct GrammarRule
{
    // enum NonTerminals nonTerminal;
    
    int numProductions;
    enum NonTerminals nonTerminal;
    struct Variable rules[MAX_NUM_PRODUCTIONS][MAX_VARS];
};

struct Grammar{
    struct GrammarRule productions[NUM_NON_TERMINALS];
};


void computeFirstSet(struct GrammarRule rule, struct Sets sets){

    //FIND IF NT_EPS PRODUCTION
    for (int i = 0; i < rule.numProductions; i += 1){
        
        if (rule.rules[i][0].val == NT_EPS){
            append(sets.firstSets[rule.nonTerminal], NT_EPS);
            continue;
        }
    }

    for (int j = 0;j < sizeof(rule.rules[i])/sizeof(rule.rules[i][0]); j += 1){
        append(sets.firstSets[rule.nonTerminal], sets.firstSets[rule.rules[i][j].val]);

        if (!containsEPS(sets.firstSets[rule.rules[i][j].val])){
            break;
        }
    }

}
    



void computeFollowSet(struct GrammarRule rule, struct Sets sets){
//ITERATE THROGH ALL PRODUCTIONS
    for (int i = 0; i < rule.numProductions; i += 1){
        int sizeRule = sizeof(rule.rules[i]) / sizeof(rule.rules[i][0]);
                       // LENGTH OF PRODUCTION IS 2
        if (sizeRule == 2){
            //EVERYTHING IN FOLLOW A IS IN FOLLOW B
            append(sets.followSets[rule.rules[i][1].val], sets.followSets[rule.nonTerminal]);
        }

        //LENGTH GREATER THAN 2
        else if (sizeRule > 2){
            for(int j = 2; j<sizeRule; j += 1){
                //EVERYTHING IN FIRST[J] IS IN FOLLOW[B]
                append(sets.followSets[rule.rules[i][1].val], sets.firstSets[rule.rules[i][j].val]);

                if (!containsEps(sets.firstSets[rule.rules[i][j].val])){
                    break;
                }

                j += 1;
            }
        }
    }
}

main()
{
    struct Grammar* grammar = (struct Grammar *) malloc(sizeof(struct Grammar));

    grammar->productions[100] = {{1,{{{NT_OTHER_FUNCTIONS,1},{NT_MAIN_FUNCTION,1}}}},{1,{{{TK_MAIN,0},{NT_STMTS,1},{TK_END,0}}}},{2,{{{NT_FUNCTION,1},{NT_OTHER_FUNCTIONS,1}},{{EPS,0}}}},{1,{{{TK_FUNID,0},{NT_INPUT_PAR,1},{NT_OUTPUT_PAR,1},{TK_SEM,0},{NT_STMTS,1},{TK_END,0}}}},{1,{{{TK_INPUT,0},{TK_PARAMETER,0},{TK_LIST,0},{TK_SQL,0},{NT_PARAMETER_LIST,1},{TK_SQR,0}}}},{2,{{{TK_OUTPUT,0},{TK_PARAMETER,0},{TK_LIST,0},{TK_SQL,0},{NT_PARAMETER_LIST,1},{TK_SQR,0}},{{EPS,0}}}},{1,{{{NT_DATA_TYPE,1},{TK_ID,0},{NT_REMAINING_LIST,1}}}},{2,{{{NT_PRIMITIVE_DATA_TYPE,1}},{{NT_CONSTRUCTED_DATA_TYPE,1}}}},{2,{{{TK_INT,0}},{{TK_REAL,0}}}},{1,{{{NT_A,1},{TK_RUID,0}}}},{2,{{{TK_COMMA,0},{NT_PARAMETER_LIST,1}},{{EPS,0}}}},{1,{{{NT_TYPE_DEFINITIONS,1},{NT_DECLARATIONS,1},{NT_OTHER_STMTS,1},{NT_RETURN_STMT,1}}}},{3,{{{NT_TYPE_DEFINITION,1},{NT_TYPE_DEFINITIONS,1}},{{NT_DEFINETYPE_STMT,1},{NT_TYPE_DEFINITION,1}},{{EPS,0}}}},{2,{{{TK_RECORD,0},{TK_RUID,0},{NT_FIELD_DEFINITIONS,1},{TK_ENDRECORD,0}},{{TK_UNION,0},{TK_RUID,0},{NT_FIELD_DEFINITIONS,1},{TK_ENDUNION,0}}}},{1,{{{NT_FIELD_DEFINITION,1},{NT_FIELD_DEFINITION,1},{NT_MORE_FIELDS,1}}}},{1,{{{TK_TYPE,0},{NT_NEW1,1}}}},{2,{{{NT_PRIMITIVE_DATA_TYPE,1},{TK_COLON,0},{TK_FIELDID,0},{TK_SEM,0}},{{TK_FIELDID,0},{TK_COLON,0},{TK_FIELDID,0},{TK_SEM,0}}}},{2,{{{NT_FIELD_DEFINITION,1},{NT_MORE_FIELDS,1}},{{EPS,0}}}},{2,{{{NT_DECLARATION,1},{NT_DECLARATIONS,1}},{{EPS,0}}}},{1,{{{TK_TYPE,0},{NT_DATA_TYPE,1},{TK_COLON,0},{TK_ID,0},{NT_OPTIONAL_GLOBAL,1},{TK_SEM,0}}}},{2,{{{TK_COLON,0},{TK_GLOBAL,0}},{{EPS,0}}}},{2,{{{NT_STMT,1},{NT_OTHER_STMTS,1}},{{EPS,0}}}},{5,{{{NT_ASSIGNMENT_STMT,1}},{{NT_ITERATIVE_STMT,1}},{{NT_CONDITIONAL_STMT,1}},{{NT_IO_STMT,1}},{{NT_FUN_CALL_STMT,1}}}},{1,{{{NT_SINGLE_OR_REC_ID,1},{TK_ASSIGNOP,0},{NT_ARITHMETIC_EXPRESSION,1},{TK_SEM,0}}}},{1,{{{TK_ID,0},{NT_NEW2,1}}}},{2,{{{NT_REC_ID,1}},{{EPS,0}}}},{1,{{{TK_DOT,0},{TK_FIELDID,0},{NT_NEW2,1}}}},{1,{{{NT_OUTPUT_PARAMETERS,1},{TK_CALL,0},{TK_FUNID,0},{TK_WITH,0},{TK_PARAMETERS,0},{NT_INPUT_PARAMETERS,1},{TK_SEM,0}}}},{2,{{{TK_SQL,0},{NT_ID_LIST,1},{TK_SQR,0},{TK_ASSIGNOP,0}},{{EPS,0}}}},{1,{{{TK_SQL,0},{NT_ID_LIST,1},{TK_SQR,0}}}},{1,{{{TK_WHILE,0},{TK_OP,0},{NT_BOOLEAN_EXPRESSION,1},{TK_CL,0},{NT_STMT,1},{NT_OTHER_STMTS,1},{TK_ENDWHILE,0}}}},{1,{{{TK_IF,0},{TK_OP,0},{NT_BOOLEAN_EXPRESSION,1},{TK_CL,0},{TK_THEN,0},{NT_STMT,1},{NT_OTHER_STMTS,1},{NT_NEW3,1},{TK_ENDIF,0}}}},{2,{{{TK_ELSE,0},{NT_STMT,1},{NT_OTHER_STMTS,1}},{{EPS,0}}}},{2,{{{TK_READ,0},{TK_OP,0},{NT_VAR,1},{TK_CL,0},{TK_SEM,0}},{{TK_WRITE,0},{TK_OP,0},{NT_VAR,1},{TK_CL,0},{TK_SEM,0}}}},{1,{{{NT_TERM,1},{NT_NEW5,1}}}},{2,{{{NT_OPERATOR,1},{NT_TERM,1},{NT_NEW5,1}},{{EPS,0}}}},{1,{{{NT_FACTOR,1},{NT_NEW6,1}}}},{2,{{{NT_OP_H,1},{NT_FACTOR,1},{NT_NEW6,1}},{{EPS,0}}}},{2,{{{NT_VAR,1}},{{TK_OP,0},{NT_ARITHMETIC_EXPRESSION,1},{TK_CL,0}}}},{2,{{{TK_PLUS,0}},{{TK_MINUS,0}}}},{2,{{{TK_MUL,0}},{{TK_DIV,0}}}},{3,{{{TK_OP,0},{NT_BOOLEAN_EXPRESSION,1},{TK_CL,0},{NT_LOGICAL_OP,1},{TK_OP,0},{NT_BOOLEAN_EXPRESSION,1},{TK_CL,0}},{{NT_VAR,1},{NT_RELATIONAL_OP,1},{NT_VAR,1}},{{TK_NOT,0},{TK_OP,0},{NT_BOOLEAN_EXPRESSION,1},{TK_CL,0}}}},{3,{{{NT_SINGLE_OR_REC_ID,1}},{{TK_NUM,0}},{{TK_RNUM,0}}}},{2,{{{TK_AND,0}},{{TK_OR,0}}}},{6,{{{TK_LT,0}},{{TK_LE,0}},{{TK_EQ,0}},{{TK_GT,0}},{{TK_GE,0}},{{TK_NE,0}}}},{1,{{{TK_RETURN,0},{NT_OPTIONAL_RETURN,1},{TK_SEM,0}}}},{2,{{{TK_SQL,0},{NT_ID_LIST,1},{TK_SQR,0}},{{EPS,0}}}},{1,{{{TK_ID,0},{NT_MORE_IDS,1}}}},{2,{{{TK_COMMA,0},{NT_ID_LIST,1}},{{EPS,0}}}},{1,{{{TK_DEFINETYPE,0},{NT_A,1},{TK_RUID,0},{TK_AS,0},{TK_RUID,0}}}},{2,{{{TK_RECORD,0}},{{TK_UNION,0}}}}};
    
    //ADD DOLLAR TO FOLLOW OF START STATE
    //ADD X TO FIRST OF EACH SET
}