// SCHEME: TWO-INPUT BUFFERS EACH OF SIZE N WITH SENTINEL CHARACTER "EOF" MARKING THE END OF EACH BUFFER

// INCLUDE LIBRARIES
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include <stdlib.h>
#include <ctype.h>

// PARAMETER N = NUMBER OF CHARACTERS READ WITH EACH RELOAD
#define BUFFER_SIZE 256
#define FINAL_STATE_OFFSET 30 // this is same as NON ACCEPT STATES + 1
// TYPICALLY 256 BYTES

// EOF CHARACTERS AT END OF BOTH BUFFERS ALWAYS

// FUNCTIONS TO READ "RELOAD" INTO BUFFER FROM A FILE

// CODE TO READ FROM THE BUFFERS CHARACTER BY CHARACTER
struct TwinBuffer
{
    // 0 -> READING SECOND BUFFER, 1 -> READING FIRST BUFFER
    int readingFirst;
    FILE* file;
    char buffer[BUFFER_SIZE * 2 + 2];
};

char *computeNumber(struct LexicalAnalyzer *lex)
{
    // BUFFER OF LEXICAL ANALYSER
    char *buffer = lex->twinBuffer->buffer;

    // STORE THE STRING
    strncpy(buffer, lex->twinBuffer->buffer + lex->begin, lex->forward - lex->begin+1);
    buffer[lex->forward] = '\0';

    // RETURN COPIED STRING
    return buffer;
}

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
        buffer[read_bytes] = '\0';
    }

    // FOR LA TO RECOGNISE END OF INPUT FROM END OF BUFFER
    return read_bytes;
}

// CREATE THE BUFFER AND RETURN IT
struct TwinBuffer* initialiseTwinBuffer(FILE* file)
{
    // INITIALISE BUFFER
    struct TwinBuffer* twinBuffer = (struct TwinBuffer*) malloc(sizeof(struct  TwinBuffer*));

    twinBuffer->readingFirst = 1;
    twinBuffer->buffer[BUFFER_SIZE] = EOF;
    twinBuffer->buffer[2 * BUFFER_SIZE + 1] = EOF;
    twinBuffer->file = file;

    return twinBuffer;
}

void returnToStart(struct LexicalAnalyzer *LA)
{
    LA->state = 0;
}

char *lexicalError(struct SymbolTableEntry *token)
{
    if (token->tokenType == TK_ID)
    {
        if (strlen(token->lexeme) > MAX_ID_SIZE)
        {
            // char errorMessage[100];
            char *errorMessage;
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

//FUNCTION TO GET CORRESPONDING NUMBER
void equivalentNumber(struct LexicalAnalyzer *lex, int flag, struct SymbolTableEntry *token)
{
    char *buffer = computeNumber(lex);
    if (flag == TK_NUM1 || flag == TK_NUM2)
    {
        // INTEGER
        token->intValue = atoi(buffer);
    }
    else
    {
        // DOUBLE
        token->doubleValue = atof(buffer);
    }
}
void changeForward(struct LexicalAnalyzer *LA, int flag)
{
    // FLAG IS 1 FOR INCREMENT AND -1 FOR DECREMENT
    LA->forward = (LA->forward + flag) % (BUFFER_SIZE * 2 + 2);
}
//TAKE ACTIONS BASED ON THE FINAL STATE AND RETURN A TOKEN
void takeActions(struct LexicalAnalyzer* LA, struct SymbolTableEntry* token){

    //NON FINAL STATE
    int state = LA->state;
    if (state < FINAL_STATE_OFFSET){
        return;
    }
   
    state -= FINAL_STATE_OFFSET;

    //INCREMENT LINE NO
    if (state == TK_COMMENT || state == CARRIAGE_RETURN)
    {
        incrementLineNo(LA);
    }

    //DONT SET TOKEN WHEN DELIMITER
    if (state == CARRIAGE_RETURN || state == DELIMITER)
    {
        returnToStart(LA);
        return;
    }

    //SET TOKEN TYPE
    token->tokenType = state;

    // SET LEXEME
    strncpy(token->lexeme, LA->twinBuffer->buffer + LA->begin, LA->forward - LA->begin + 1);

    //EQUIVALENT NUMBER
    if (state == TK_RNUM1 || state == TK_RNUM2 || state == TK_NUM1 || state == TK_NUM2){
        //COMPUTE NUMBER
        equivalentNumber(LA, state, token);
    }
    
    //GET THE SYMBOL TABLE ENTRY
    if (state == TK_FIELDID || state == TK_RUID || state == TK_ID || state == TK_FUNID){
        token = getToken(token);
    }

    //DOUBLE STAR STATES
    if (state == TK_NUM2 || state == TK_LE){
        changeForward(LA, -1);
    }

    //FINAL STATE WITHOUT ANY OTHER ACTIONS
    else{
        //INCREMENT FORWARD
        changeForward(LA, +1);
    }
}

struct SymbolTableEntry* initialiseToken(){
    struct SymbolTableEntry *token;
    token = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry *));

    token->lexeme = NULL;
    token->intValue = 0;
    token->doubleValue = 0;
    token->lineNo = 0;
    token->tokenType = -1;

    return token;
}

struct SymbolTableEntry *scanToken(struct LexicalAnalyzer *LA)
{

    //INITIALSE TOKEN
    struct SymbolTableEntry *token = initialiseToken();
    
    //RESET TO START STATE
    LA->state = 0;

    //KEEP ITERATING
    while (1)
    {
        // GET CHARACTER CURRENTLY BEING READ
        char character = LA->twinBuffer->buffer[LA->forward];

        // DELIMITER
        if (character == '\t' || character == ' ')
        {
            continue;
        }
        
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
        takeActions(LA, token);

        // HAVE TO RETURN
        if (token->tokenType != -1)
        {
            return token;
        }

        // INCREMENT FORWARD
        changeForward(LA, 1);
    }
}

struct LexicalAnalyzer *initialiseLA(struct TwinBuffer *twinBuffer)
{
    struct LexicalAnalyzer * LA;
    LA = (struct LexicalAnalyzer *) malloc(sizeof(struct LexicalAnalyzer));
    
    LA->lineNo = 0;
    LA->begin = 0;
    LA->forward = 0;
    LA->state = 0;
    LA->twinBuffer = twinBuffer;
    return LA;

}
    
int main()
{
    // TEST THE TWIN BUFFER
    FILE *file = readTestFile("test_program.txt");

    // INITIALISE A TWIN BUFFER
    struct TwinBuffer* twinBuffer = initialiseTwinBuffer(file);

    // INITIALISE LA
    struct LexicalAnalyzer *LA = initialiseLA(twinBuffer);

    //START SCANNING
    readIntoBuffer(twinBuffer);

    // THE TOKEN
    struct SymbolTableEntry *token;

    //KEEP GETTING TOKEN UNTIL INPUT CONSUMED 
    while (token){
        token = scanToken(LA);
        printf("%d ", token->tokenType);
        
    }
}
