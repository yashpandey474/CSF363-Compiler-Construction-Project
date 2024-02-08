// SCHEME: TWO-INPUT BUFFERS EACH OF SIZE N WITH SENTINEL CHARACTER "EOF" MARKING THE END OF EACH BUFFER

// INCLUDE LIBRARIES
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include <stdlib.h>
#include <ctype.h>

// PARAMETER N = NUMBER OF CHARACTERS READ WITH EACH RELOAD
#define BUFFER_SIZE 256
#define FINAL_STATE_OFFSET NUM_NON_ACCEPT_STATES + 1 // this is same as NON ACCEPT STATES + 1
// TYPICALLY 256 BYTES

// EOF CHARACTERS AT END OF BOTH BUFFERS ALWAYS

// FUNCTIONS TO READ "RELOAD" INTO BUFFER FROM A FILE

// CODE TO READ FROM THE BUFFERS CHARACTER BY CHARACTER
struct TwinBuffer
{
    // 0 -> READING SECOND BUFFER, 1 -> READING FIRST BUFFER
    int readingFirst;
    FILE *file;
    char buffer[BUFFER_SIZE * 2 + 2];
};

void incrementLineNo(struct LexicalAnalyzer *LA)
{
    LA->lineNo += 1;
    return;
}

FILE *readTestFile(char *file_path)
{
    FILE *file = fopen(file_path, "r");

    // UNABLE TO READ FILE
    if (file == NULL)
    {
        printf("Error opening file %s\n", file_path);
        return NULL;
    }

    return file;
}
int readIntoBuffer(struct TwinBuffer *twinBuffer)
{
    char *buffer;
    if (twinBuffer->readingFirst)
    {
        buffer = twinBuffer->buffer;
    }
    else
    {
        buffer = twinBuffer->buffer + BUFFER_SIZE + 1;
    }
    // READING ALTERNATE BUFFER
    twinBuffer->readingFirst = 1 - twinBuffer->readingFirst;
    size_t read_bytes = fread(buffer, sizeof(char), BUFFER_SIZE, twinBuffer->file);

    // MARK END OF INPUT
    if (read_bytes < BUFFER_SIZE)
    {
        // MARK WITH ANOTHER EOF
        buffer[read_bytes] = EOF;
    }

    // FOR LA TO RECOGNISE END OF INPUT FROM END OF BUFFER
    return read_bytes;
}

// CREATE THE BUFFER AND RETURN IT
struct TwinBuffer *initialiseTwinBuffer(FILE *file)
{
    // INITIALISE BUFFER
    struct TwinBuffer *twinBuffer = (struct TwinBuffer *)malloc(sizeof(struct TwinBuffer));

    twinBuffer->readingFirst = 1;
    twinBuffer->buffer[BUFFER_SIZE] = EOF;
    twinBuffer->buffer[2 * BUFFER_SIZE + 1] = EOF;
    twinBuffer->file = file;

    return twinBuffer;
}
void resetBegin(struct LexicalAnalyzer *LA)
{
    LA->begin = LA->forward;
}

void returnToStart(struct LexicalAnalyzer *LA)
{
    LA->state = 0;
    resetBegin(LA);
}

char *lexicalError(struct SymbolTableEntry *token)
{
    char *errorMessage = (char *)malloc(sizeof(char) * 100);

    if (token->tokenType == TK_ID)
    {
        if (strlen(token->lexeme) > MAX_ID_SIZE)
        {
            // char errorMessage[100];

            sprintf(errorMessage, "Lexical Error: Identifier %s of length %zu greater than maximum size", token->lexeme, strlen(token->lexeme));
            return errorMessage;
            // return "Lexical Error: Identifier %s of length %d greater than maximum size";
        }
    }

    if (token->tokenType == TK_FUNID)
    {
        if (strlen(token->lexeme) > MAX_FUNID_SIZE)
        {
            return "Lexical Error: Function Identifier %s of length %d greater than maximum size";
        }
    }

    return "";
}

// FUNCTION TO GET CORRESPONDING NUMBER
void equivalentNumber(struct LexicalAnalyzer *lex, int flag, struct SymbolTableEntry *token)
{
    printf("EQV NUMS %s\n", token->lexeme);
    if (flag == TK_NUM1 || flag == TK_NUM2)
    {
        // INTEGER
        token->intValue = atoi(token->lexeme);
    }
    else
    {
        // DOUBLE
        token->doubleValue = atof(token->lexeme);
    }
}
void changeForward(struct LexicalAnalyzer *LA, int flag)
{
    // FLAG IS 1 FOR INCREMENT AND -1 FOR DECREMENT
    LA->forward = (LA->forward + flag) % (BUFFER_SIZE * 2 + 2);
}

// TAKE ACTIONS BASED ON THE FINAL STATE AND RETURN A TOKEN
struct SymbolTableEntry *takeActions(struct LexicalAnalyzer *LA, struct SymbolTableEntry *token)
{
    // NON FINAL STATE32
    int state = LA->state;
    if (state < FINAL_STATE_OFFSET)
    {
        return token;
    }

    state -= FINAL_STATE_OFFSET;

    // INCREMENT LINE NO
    if (state == TK_COMMENT || state == CARRIAGE_RETURN)
    {
        incrementLineNo(LA);
    }

    // DONT SET TOKEN WHEN DELIMITER
    if (state == CARRIAGE_RETURN || state == DELIMITER)
    {
        returnToStart(LA);
        changeForward(LA, -1);

        return token;
    }

    // SET TOKEN TYPE
    token->tokenType = state;

    // DOUBLE STAR STATES
    if (state == TK_NUM2 || state == TK_LT2)
    {
        changeForward(LA, -1);
    }

    // SET LEXEME
    strncpy(token->lexeme, LA->twinBuffer->buffer + LA->begin, LA->forward - LA->begin);

    // EQUIVALENT NUMBER
    if (state == TK_RNUM1 || state == TK_RNUM2 || state == TK_NUM1 || state == TK_NUM2)
    {
        // COMPUTE NUMBER
        equivalentNumber(LA, state, token);
    }

    // GET THE SYMBOL TABLE ENTRY
    if (state == TK_FIELDID || state == TK_RUID || state == TK_ID || state == TK_FUNID)
    {
        token = getToken(token);
    }

    // FINAL STATE WITHOUT ANY OTHER ACTIONS
    else if (state != TK_LT2 && state != TK_NUM2)
    {
        // INCREMENT FORWARD
        changeForward(LA, +1);

        // SET LEXEME
        strncpy(token->lexeme, LA->twinBuffer->buffer + LA->begin, LA->forward - LA->begin);
    }

    return token;
}

struct SymbolTableEntry *initialiseToken()
{

    struct SymbolTableEntry *token = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));

    token->lexeme = (char *)malloc(sizeof(char) * 100);
    token->intValue = 0;
    token->doubleValue = 0;
    token->lineNo = 0;
    token->tokenType = 0;
    return token;
}

struct SymbolTableEntry *scanToken(struct LexicalAnalyzer *LA)
{
    // INITIALSE TOKEN
    struct SymbolTableEntry *token = initialiseToken();

    // RESET TO START STATE
    LA->state = 0;

    char character;

    // KEEP ITERATING
    while (1)
    {
        // GET CHARACTER CURRENTLY BEING READ

        character = LA->twinBuffer->buffer[LA->forward];

        // LINE FEED: LITE (AFTER \N)
        if ((int)character == 10)
        {
            // INCREMENT FORWARD
            changeForward(LA, 1);

            // RESET
            resetBegin(LA);
            
            continue;
        }


        printf("\nSTATE %d CHARACTER %d (%c)\n", LA->state, character, character);

        // TODO: DIFFERENTIATE BETWEEN END OF INPUT AND END OF BUFFER
        if (character == EOF)
        {
            // RELOAD OTHER BUFER
            int res = readIntoBuffer(LA->twinBuffer);

            // ALL INPUT READ AND PROCESSED
            if (res == 0)
            {
                return NULL;
            }
        }

        // CHANGE STATE
        LA->state = getNextState(LA->state, character);

        // TAKE ACTIONS FOR THE STATE
        token = takeActions(LA, token);

        // HAVE TO RETURN
        if (token->tokenType != 0)
        {
            resetBegin(LA);
            return token;
        }

        // INCREMENT FORWARD
        changeForward(LA, 1);
    }
}

struct LexicalAnalyzer *initialiseLA(struct TwinBuffer *twinBuffer)
{
    struct LexicalAnalyzer *LA;
    LA = (struct LexicalAnalyzer *)malloc(sizeof(struct LexicalAnalyzer));

    LA->lineNo = 0;
    LA->begin = 0;
    LA->forward = 0;
    LA->state = 0;
    LA->twinBuffer = twinBuffer;
    return LA;
}

int main()
{
    printf("ENTERED MAIN\n");
    // INITIALISE THE SYMBOL TABLE
    insertAllKeywords();
    printSymbolTable();

    printf("KEYWORDS\n");

    // TEST THE TWIN BUFFER
    FILE *file = readTestFile("test_program.txt");

    // INITIALISE A TWIN BUFFER
    struct TwinBuffer *twinBuffer = initialiseTwinBuffer(file);

    // INITIALISE LA
    struct LexicalAnalyzer *LA = initialiseLA(twinBuffer);

    printf("LA INITIALISED\n");

    // START SCANNING
    readIntoBuffer(twinBuffer);

    printf("READ INPUT\n");

    // THE TOKEN
    struct SymbolTableEntry *token;

    printf("STARTING SCANNING\n");

    while (token = scanToken(LA))
    {
        printf("%s %s ", TokenToString(token->tokenType), token->lexeme);
    }
}
