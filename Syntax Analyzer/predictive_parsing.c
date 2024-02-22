#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

#define STACK_INITIAL_SIZE 128
struct stack
{
    struct Variable *stack;
    int top;
    int MAX;
};
void printStack(struct stack *st)
{
    for (int i = 0; i <= st->top; i++)
    {

        if (st->stack[i].flag == 1)
        {
            printf("%s ", NonTerminalToString(st->stack[i].val));
        }

        else
        {
            printf("%s ", TokenToString(st->stack[i].val));
        }
    }
    printf("\n");
}

bool isEmptyStack(struct stack *st)
{
    return st->top == -1;
}

bool isFull(struct stack *st)
{
    if (st->top == st->MAX - 1)
    {
        st->MAX += 128;
        st->stack = (struct Variable *)realloc(st->stack, 2 * st->MAX * sizeof(struct Variable));
        return true;
    }
    return false;
}

void push(struct stack *st, struct Variable data)
{
    // CHECKS IF FULL AND REALLOCATES
    isFull(st);

    st->stack[++st->top] = data;
}

struct Variable pop(struct stack *st)
{
    struct Variable stackBottom;
    stackBottom.val = -1;

    if (isEmptyStack(st))
    {
        printf("Stack is empty\n");
        return stackBottom;
    }
    return st->stack[st->top--];
}

struct Variable peek(struct stack *st)
{
    struct Variable stackBottom;
    stackBottom.val = -1;

    if (!isEmptyStack(st))
    {
        return st->stack[st->top];
    }
    else
    {
        printf("Stack is empty\n");
        return stackBottom;
    }
}

int predictive_parsing(struct SymbolTableEntry *token, struct ParsingTable *pt, struct stack *st, struct LexicalAnalyzer *LA)
{
    enum Tokentype a = token->tokenType;

    struct Variable X = st->stack[st->top];
    // BOTH ARE TERMINALS
    if (X.val == a && X.flag == 0)
    {
        // printf("POPPED TERMINAL: %s\n", TokenToString(X.val));
        pop(st);
        return 1;
        // a=next symbol of w
    }
    else if (X.flag == 0)
    {
        // CALL ERROR FUNCTION
        printf("Line no. %-5d Error: The token %s for lexeme %s  does not match with the expected token %s\n", LA->lineNo, TokenToString(a), token->lexeme, TokenToString(X.val));
        pop(st);
        return 0;
    }
    else if (pt->table[X.val][a] == NULL)
    {
        // printf("Line %-5d Error: Invalid token %s encountered with value %s stack top %s\n", LA->lineNo, TokenToString(a), token->lexeme, NonTerminalToString(X.val));
        // go and get the next token
        return 1;
    }
    // SYN
    else if (pt->table[X.val][a][0].val == -1)
    {
        // SYN TOKEN; POP THE NONTERMINAL
        printf("Line no. %-5d Error: The token %s for lexeme %s  does not match with the expected token %s\n", LA->lineNo, TokenToString(a), token->lexeme, NonTerminalToString(X.val));

        pop(st);
        return 0;
    }
    else
    {
        // GET THE RULE
        struct Variable *arr = pt->table[X.val][a];
        struct Variable topStack = pop(st);
        // printf("Pushing rule ");
        // printRule(topStack.val, arr);

        for (int var = 8; var >= 0; var -= 1)
        {
            if (isDefault(arr[var]))
            {
                continue;
            }

            if (arr[var].val == TK_EPS && arr[var].flag == 0)
            {
                break;
            }

            push(st, arr[var]);
        }

        return 0;
    }

    // TODO: WHAT TO RETURN IF ERROR
}

struct stack *initialiseStack()
{
    struct stack *stack = (struct stack *)malloc(sizeof(struct stack));

    stack->MAX = STACK_INITIAL_SIZE;
    stack->top = -1;
    stack->stack = (struct Variable *)malloc(sizeof(struct Variable) * STACK_INITIAL_SIZE);
    return stack;
}

int main()
{
    struct stack *stack = initialiseStack();
    insertAllKeywords();
    FILE *file = readTestFile("t6.txt");

    struct TwinBuffer *twinBuffer = initialiseTwinBuffer(file);
    struct LexicalAnalyzer *LA = initialiseLA(twinBuffer);
    struct SymbolTableEntry *token;
    struct GrammarRule productions[NUM_NON_TERMINALS] = {{1, {{{NT_OTHER_FUNCTIONS, 1}, {NT_MAIN_FUNCTION, 1}}}}, {1, {{{TK_MAIN, 0}, {NT_STMTS, 1}, {TK_END, 0}}}}, {2, {{{NT_FUNCTION, 1}, {NT_OTHER_FUNCTIONS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_FUNID, 0}, {NT_INPUT_PAR, 1}, {NT_OUTPUT_PAR, 1}, {TK_SEM, 0}, {NT_STMTS, 1}, {TK_END, 0}}}}, {1, {{{TK_INPUT, 0}, {TK_PARAMETER, 0}, {TK_LIST, 0}, {TK_SQL, 0}, {NT_PARAMETER_LIST, 1}, {TK_SQR, 0}}}}, {2, {{{TK_OUTPUT, 0}, {TK_PARAMETER, 0}, {TK_LIST, 0}, {TK_SQL, 0}, {NT_PARAMETER_LIST, 1}, {TK_SQR, 0}}, {{TK_EPS, 0}}}}, {1, {{{NT_DATA_TYPE, 1}, {TK_ID, 0}, {NT_REMAINING_LIST, 1}}}}, {2, {{{NT_PRIMITIVE_DATA_TYPE, 1}}, {{NT_CONSTRUCTED_DATA_TYPE, 1}}}}, {2, {{{TK_INT, 0}}, {{TK_REAL, 0}}}}, {2, {{{NT_A, 1}, {TK_RUID, 0}}, {{TK_RUID, 0}}}}, {2, {{{TK_COMMA, 0}, {NT_PARAMETER_LIST, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_TYPE_DEFINITIONS, 1}, {NT_DECLARATIONS, 1}, {NT_OTHER_STMTS, 1}, {NT_RETURN_STMT, 1}}}}, {3, {{{NT_TYPE_DEFINITION, 1}, {NT_TYPE_DEFINITIONS, 1}}, {{NT_DEFINETYPE_STMT, 1}, {NT_TYPE_DEFINITIONS, 1}}, {{TK_EPS, 0}}}}, {2, {{{TK_RECORD, 0}, {TK_RUID, 0}, {NT_FIELD_DEFINITIONS, 1}, {TK_ENDRECORD, 0}}, {{TK_UNION, 0}, {TK_RUID, 0}, {NT_FIELD_DEFINITIONS, 1}, {TK_ENDUNION, 0}}}}, {1, {{{NT_FIELD_DEFINITION, 1}, {NT_FIELD_DEFINITIONS, 1}, {NT_MORE_FIELDS, 1}}}}, {1, {{{TK_TYPE, 0}, {NT_DATA_TYPE, 1}, {TK_COLON, 0}, {TK_FIELDID, 0}, {TK_SEM, 0}}}}, {2, {{{NT_FIELD_DEFINITION, 1}, {NT_MORE_FIELDS, 1}}, {{TK_EPS, 0}}}}, {2, {{{NT_DECLARATION, 1}, {NT_DECLARATIONS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_TYPE, 0}, {NT_DATA_TYPE, 1}, {TK_COLON, 0}, {TK_ID, 0}, {NT_GLOBAL_OR_NOT, 1}, {TK_SEM, 0}}}}, {2, {{{TK_COLON, 0}, {TK_GLOBAL, 0}}, {{TK_EPS, 0}}}}, {2, {{{NT_STMT, 1}, {NT_OTHER_STMTS, 1}}, {{TK_EPS, 0}}}}, {5, {{{NT_ASSIGNMENT_STMT, 1}}, {{NT_ITERATIVE_STMT, 1}}, {{NT_CONDITIONAL_STMT, 1}}, {{NT_IO_STMT, 1}}, {{NT_FUN_CALL_STMT, 1}}}}, {1, {{{NT_SINGLE_OR_REC_ID, 1}, {TK_ASSIGNOP, 0}, {NT_ARITHMETIC_EXPRESSION, 1}, {TK_SEM, 0}}}}, {1, {{{TK_ID, 0}, {NT_REC_ID, 1}}}}, {2, {{{TK_DOT, 0}, {TK_FIELDID, 0}, {NT_REC_ID, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_OUTPUT_PARAMETERS, 1}, {TK_CALL, 0}, {TK_FUNID, 0}, {TK_WITH, 0}, {TK_PARAMETERS, 0}, {NT_INPUT_PARAMETERS, 1}, {TK_SEM, 0}}}}, {2, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}, {TK_ASSIGNOP, 0}}, {{TK_EPS, 0}}}}, {1, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}}}}, {1, {{{TK_WHILE, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}, {TK_ENDWHILE, 0}}}}, {1, {{{TK_IF, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {TK_THEN, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}, {NT_NEW3, 1}, {TK_ENDIF, 0}}}}, {2, {{{TK_ELSE, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}}, {{TK_EPS, 0}}}}, {2, {{{TK_READ, 0}, {TK_OP, 0}, {NT_VAR, 1}, {TK_CL, 0}, {TK_SEM, 0}}, {{TK_WRITE, 0}, {TK_OP, 0}, {NT_VAR, 1}, {TK_CL, 0}, {TK_SEM, 0}}}}, {1, {{{NT_TERM, 1}, {NT_NEW5, 1}}}}, {2, {{{NT_OPERATOR, 1}, {NT_TERM, 1}, {NT_NEW5, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_FACTOR, 1}, {NT_NEW6, 1}}}}, {2, {{{NT_OP_H, 1}, {NT_FACTOR, 1}, {NT_NEW6, 1}}, {{TK_EPS, 0}}}}, {2, {{{NT_VAR, 1}}, {{TK_OP, 0}, {NT_ARITHMETIC_EXPRESSION, 1}, {TK_CL, 0}}}}, {2, {{{TK_PLUS, 0}}, {{TK_MINUS, 0}}}}, {2, {{{TK_MUL, 0}}, {{TK_DIV, 0}}}}, {3, {{{TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {NT_LOGICAL_OP, 1}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}}, {{NT_VAR, 1}, {NT_RELATIONAL_OP, 1}, {NT_VAR, 1}}, {{TK_NOT, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}}}}, {3, {{{NT_SINGLE_OR_REC_ID, 1}}, {{TK_NUM, 0}}, {{TK_RNUM, 0}}}}, {2, {{{TK_AND, 0}}, {{TK_OR, 0}}}}, {6, {{{TK_LT, 0}}, {{TK_LE, 0}}, {{TK_EQ, 0}}, {{TK_GT, 0}}, {{TK_GE, 0}}, {{TK_NE, 0}}}}, {1, {{{TK_RETURN, 0}, {NT_OPTIONAL_RETURN, 1}, {TK_SEM, 0}}}}, {2, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}}, {{TK_EPS, 0}}}}, {1, {{{TK_ID, 0}, {NT_MORE_IDS, 1}}}}, {2, {{{TK_COMMA, 0}, {TK_ID, 0}, {NT_MORE_IDS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_DEFINETYPE, 0}, {NT_A, 1}, {TK_RUID, 0}, {TK_AS, 0}, {TK_RUID, 0}}}}, {2, {{{TK_RECORD, 0}}, {{TK_UNION, 0}}}}};
    struct Sets **sets_for_all = initialiseSetsWhole();
    struct ParsingTable *PT = (struct ParsingTable *)malloc(sizeof(struct ParsingTable));

    readIntoBuffer(twinBuffer);
    printf("READ INPUT\n");
    computeFirstSet(sets_for_all, productions);
    printf("FIRST SET COMPUTED\n");
    computeFollowSet(sets_for_all, productions);
    printf("FOLLOW SET COMPUTED\n");
    populate_parsing_table(PT, productions, sets_for_all);
    printf("PARSING TABLE POPULATED\n");
    // printParsingTable(PT);
    push(stack, (struct Variable){NT_PROGRAM, 1});

    // write to computed_sets.txt
    FILE *cfile = fopen("computed_sets.txt", "w");
    if (cfile == NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }
    printFFSetsTable(cfile, sets_for_all);
    fclose(cfile);

    while ((token = scanToken(LA)))
    {
        if(token->tokenType==LEXICAL_ERROR)
{        printf("Line no. %-5d Lexeme %-30s Token %-s\n", LA->lineNo, token->lexeme, TokenToString(token->tokenType));
}        if (!(token->tokenType == LEXICAL_ERROR || token->tokenType == TK_COMMENT))
        {
            // printf("Stack before:\n");
            // printStack(stack);
            while (predictive_parsing(token, PT, stack, LA) == 0)
            {
                // printf("Stack after:\n");

                // printStack(stack);

                // keep doing it basically. youll only go to the next token if there's a valid accepting thing( in which case it returns something)
            }
        }
    }
    if (isEmptyStack(stack))
    {
        printf("TUTUTUDUUU MAX VERSTAPPEN: SYNTAX ANALYSIS COMPLETE\\n");
    }
    return 0;
}
