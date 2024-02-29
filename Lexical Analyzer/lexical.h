// Enumerated token types
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef LEXICAL_H
#define LEXICAL_H
#define HASH_MAP_SIZE 91
#define MAX_ID_SIZE 20
#define MAX_FUNID_SIZE 30
#define NUM_NON_ACCEPT_STATES 29
#define NUM_STATES 95 // 66 accept states, number of states including accept states
#define END_OF_FILE_VAL 128
#define BUFFER_SIZE 256
#define FINAL_STATE_OFFSET NUM_NON_ACCEPT_STATES + 1 // this is same as NON ACCEPT STATES + 1
#define MAX_LEXEME_SIZE 100
// extern int numStates;
// LEXICAL ANALYSER

typedef struct TwinBufferArray TwinBufferArray;

typedef TwinBufferArray *twinBufferArray;

typedef struct TwinBuffer TwinBuffer;

typedef TwinBuffer* twinBuffer;

typedef struct SymbolTableEntry *tokenInfo;

struct TwinBufferArray
{
    // 0 -> READING SECOND BUFFER, 1 -> READING FIRST BUFFER
    int readingFirst;
    FILE *file;
    char buffer[BUFFER_SIZE * 2 + 2];
};

struct TwinBuffer
{
    int lineNo, begin, forward, state;
    twinBufferArray bufferArray;
};

// DECLARATIONS AND GLOBAL VARIABLES

struct SymbolTableNode
{
    struct SymbolTableEntry *entry;
    struct SymbolTableNode *next;
};

typedef enum Tokentype
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
} Tokentype;

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

typedef enum CharacterType
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
} CharacterType;

// FUNCTION DECLARATIONS
void getToken(struct SymbolTableEntry *symbolTableEntry);
int getNextState(int currentState, int character);

extern struct SymbolTableNode *symbolTable[HASH_MAP_SIZE];

void insertAllKeywords();
void printSymbolTable();

void changeForward(twinBuffer *LA, int flag);

const char *CharacterTypeToString(enum CharacterType type);
const char *TokenToString(int token);

extern CharacterType characterTypeMap[129];

FILE *readTestFile(char *file_path);
twinBufferArray initialiseTwinBuffer(FILE *file);
twinBuffer *initialiseLA(twinBufferArray bufferArray);
int readIntoBuffer(twinBufferArray bufferArray);
struct SymbolTableEntry *scanToken(twinBuffer *LA);

#endif
