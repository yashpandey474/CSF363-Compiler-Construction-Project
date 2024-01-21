// Enumerated token types
#include <stdio.h>
#include <stdlib.h>
#ifndef LEXICAL_H
#define LEXICAL_H
#define HASH_MAP_SIZE 91
#define MAX_ID_SIZE 20
#define MAX_FUNID_SIZE 30

// LEXICAL ANALYSER
struct LexicalAnalyzer
{
    int lineNo, begin, forward;
};

// DECLARATIONS AND GLOBAL VARIABLES
extern struct SymbolTableNode *symbolTable[HASH_MAP_SIZE];
struct SymbolTableNode
{
    struct SymbolTableEntry *entry;
    struct SymbolTableNode *next;
};

typedef enum Tokentype
{
    TK_ASSIGNOP,
    TK_COMMENT,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_ENDWHILE,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_OP,
    TK_CL,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_FUNID,
} Tokentype;
struct SymbolTableEntry
{
    char *lexeme;
    int lineNo;
    enum Tokentype tokenType;
};
// typedef enum sigma{
//     "<",
//     ">",
//     "=",
//     "-",
//     "%",
//     ",",
//     ";",
//     ":",
//     ".",
//     "(",
//     ")",
//     "[",
//     "]",
//     "+",
//     "-",
//     "*",
//     "/",
//     "&",
//     "@",
//     "~",
//     "\n",
//     "\t",
//     " ",
//     "=",
//     "#"
// };
#endif
struct SymbolTableEntry *insertIntoSymbolTable(struct SymbolTableEntry * token);
