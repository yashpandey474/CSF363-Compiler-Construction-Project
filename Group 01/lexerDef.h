#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LEXERDEF_H
#define LEXERDEF_H

#define HASH_MAP_SIZE 91
#define MAX_ID_SIZE 20
#define MAX_FUNID_SIZE 30
#define NUM_NON_ACCEPT_STATES 29
#define NUM_STATES 95 // 66 accept states, number of states including accept states
#define END_OF_FILE_VAL 128
#define BUFFER_SIZE 256
#define FINAL_STATE_OFFSET NUM_NON_ACCEPT_STATES + 1 // this is same as NON ACCEPT STATES + 1
#define MAX_LEXEME_SIZE 100
struct TwinBuffer
{
    // 0 -> READING SECOND BUFFER, 1 -> READING FIRST BUFFER
    int readingFirst;
    FILE *file;
    char buffer[BUFFER_SIZE * 2 + 2];
};
typedef struct TwinBuffer TwinBuffer;

// typedefing the pointer to the array
typedef TwinBuffer *twinBuffer;

struct LexicalAnalyser
{
    int lineNo, begin, forward, state;
    twinBuffer bufferArray;
};

typedef struct LexicalAnalyser LexicalAnalyser;

// pointer typedef
typedef LexicalAnalyser *lexicalAnalyser;
struct SymbolTableNode
{
    struct SymbolTableEntry *entry;
    struct SymbolTableNode *next;
};
enum Tokentype
{
    CARRIAGE_RETURN,
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
    TK_SQR,
    TK_SQL,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT1,
    TK_LT2,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,
    TK_FIELDID,
    TK_ID,
    TK_NUM1,
    TK_NUM2,
    TK_RNUM1,
    TK_RNUM2,
    TK_FUNID,
    TK_RUID,
    DELIMITER,
    TK_RNUM,
    TK_NUM,
    TK_LT,
    TK_EPS,
    TK_EOF,
    LEXICAL_ERROR
};
typedef enum TokenType TokenType;

struct SymbolTableEntry
{
    char *lexeme;
    // INTVALUE -> NUM
    int intValue;
    int lineNo;
    // DOUBLEVALUE -> RNUM
    double doubleValue;
    enum Tokentype tokenType;
};
typedef struct SymbolTableEntry *tokenInfo;

enum CharacterType
{
    CT_LETTER_LOWER_EXCEPT_ID_LETTER,
    CT_ID_DIGIT,
    CT_ID_LETTER,
    CT_LETTER_UPPER,
    CT_EXPONENT,
    CT_DIGIT_EXCEPT_ID_DIGIT,
    CT_DELIMITER,
    CT_INVALID,
    CT_GREATER,
    CT_LESSER,
    CT_EQUAL,
    CT_COMMA,
    CT_SEM,
    CT_COLON,
    CT_DOT,
    CT_OP,
    CT_CL,
    CT_PLUS,
    CT_MINUS,
    CT_MUL,
    CT_DIV,
    CT_AND,
    CT_OR,
    CT_NOT,
    CT_HYPHEN,
    CT_PERCENT,
    CT_AT_THE_RATE,
    CT_SQUARE_OPEN,
    CT_SQUARE_CLOSE,
    CT_ROUND_OPEN,
    CT_ROUND_CLOSE,
    CT_EXCLAMATION,
    CT_TILDE,
    CT_UNDERSCORE,
    CT_CARRIAGE_RETURN,
    CT_EOF,
    CT_HASH
};
typedef enum CharacterType CharacterType;

extern CharacterType characterTypeMap[129];

extern struct SymbolTableNode *symbolTable[HASH_MAP_SIZE];

#endif