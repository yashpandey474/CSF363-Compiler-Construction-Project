// Enumerated token types
#include <stdio.h>
#include <stdlib.h>
#ifndef LEXICAL_H
#define LEXICAL_H
#define HASH_MAP_SIZE 91
#define MAX_ID_SIZE 20
#define MAX_FUNID_SIZE 30
#define NUM_STATES 40

extern int numStates;
// LEXICAL ANALYSER
struct LexicalAnalyzer
{
    int lineNo, begin, forward;
    struct TwinBuffer *twinBuffer;
};

// DECLARATIONS AND GLOBAL VARIABLES
extern struct SymbolTableNode *symbolTable[HASH_MAP_SIZE];
struct SymbolTableNode
{
    struct SymbolTableEntry *entry;
    struct SymbolTableNode *next;
};

// FUNCTION DECLARATIONS
struct SymbolTableEntry *getToken(struct SymbolTableEntry *symbolTableEntry);

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
    TK_SQR,
    TK_SQL,
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
    TK_RUID,
    CARRIAGE_RETURN,
    DELIMITER
} Tokentype;
struct SymbolTableEntry
{
    char *lexeme;
    int intValue;
    double doubleValue;
    int lineNo;
    enum Tokentype tokenType;
};
typedef enum CharacterType
{
    CT_LETTER_LOWER_EXCEPT_ID_LETTER,
    CT_ID_DIGIT,
    CT_ID_LETTER,
    CT_LETTER_UPPER,
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
    CT_ROUND_CLOSE, // Changed from CT_SQUARE_CLOSE to CT_ROUND_CLOSE
    CT_EXCLAMATION,
    CT_TILDE
} CharacterType;

// ID DIGIT IS 2-7. ID LETTER IS b-d
CharacterType characterTypeMap[128];

void initializeCharacterTypeMap();
#endif
