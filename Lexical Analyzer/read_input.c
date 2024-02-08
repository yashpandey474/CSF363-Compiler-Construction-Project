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
    char buffer[BUFFER_SIZE * 2 + 2];
};

char *computeNumber(struct LexicalAnalyzer *lex)
{
    // BUFFER OF LEXICAL ANALYSER
    char *buffer = lex->twinBuffer->buffer;

    // STORE THE STRING
    strncpy(buffer, lex->twinBuffer + lex->begin, lex->forward);
    buffer[lex->forward] = '\0';

    // RETURN COPIED STRING
    return buffer;
}

void incrementLineNo(struct LexicalAnalyzer *LA)
{
    LA->lineNo += 1;
    return LA->lineNo;
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
int readIntoBuffer(struct TwinBuffer *twinBuffer, FILE *file)
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
    size_t read_bytes = fread(buffer, sizeof(char), BUFFER_SIZE, file);

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
struct TwinBuffer initialiseTwinBuffer()
{
    // INITIALISE BUFFER
    struct TwinBuffer twinBuffer;

    twinBuffer.readingFirst = 1;
    twinBuffer.buffer[BUFFER_SIZE] = '\0';
    twinBuffer.buffer[2 * BUFFER_SIZE + 1] = '\0';

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

//TAKE ACTIONS BASED ON THE FINAL STATE AND RETURN A TOKEN
void takeActions(struct LexicalAnalyzer* LA, struct SymbolTableEntry* token){

    //NON FINAL STATE
    int state = LA->state;
    if (state < FINAL_STATE_OFFSET){
        return NULL;
    }
   
    state -= FINAL_STATE_OFFSET;

    //SET TOKEN TYPE
    token->tokenType = state;

    // SET LEXEME
    strncpy(token->lexeme, LA->twinBuffer->buffer + LA->begin, LA->forward - LA->begin);

    //EQUIVALENT NUMBER
    if (state == TK_RNUM1 || state == TK_RNUM2 || state == TK_NUM1 || state == TK_NUM2){
        //COMPUTE NUMBER
        equivalentNumber(LA, state,  token);
    }
    
    //GET TOKEN
    if (state == TK_FIELDID){
        token->tokenType = getToken(token);
    }

    //GET POINTER TO SYMBOL TABLE ENTRY
    if (state == TK_FIELDID || state == TK_RUID || state == TK_ID || state == TK_FUNID){
        token = installId(token->lexeme);
    }

    if (state == TK_COMMENT || state == CARRIAGE_RETURN){
        incrementLineNo(LA);
    }


    if (state == CARRIAGE_RETURN || state == DELIMITER){
        returnToStart(LA);
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
    token->tokenType = NULL;

    return token;
}
void changeForward(struct LexicalAnalyzer *LA, int flag)
{
    //FLAG IS 1 FOR INCREMENT AND -1 FOR DECREMENT
    LA->forward = (LA->forward + flag) % (BUFFER_SIZE * 2 + 2);
}
struct SymbolTableEntry *scanToken(struct LexicalAnalyzer *LA, FILE *file)
{

    struct SymbolTableEntry *token;
    token = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry *));
    LA->state = 0;
    while (1)
    {
        // GET CHARACTER CURRENTLY BEING READ
        char character = LA->twinBuffer->buffer[LA->forward];

        // INCREMENT FORWARD
        incrementForward(LA);

        // TODO: GENERALISE FOR ALL DELIMITERS;

        // CARRIAGE RETURN
        if (character == '\n')
        {
            incrementLineNo(LA);
        }

        // DELIMITER
        if (character == '\t' || character == ' ')
        {
            continue;
        }

        // END OF PROGRAM ?

        switch (LA->state)
        {
        // START STATE
        case 0:
            // LOWERCASE LETTER
            if (character >= 'a' && character <= 'z')
            {
                // IDENTIFIER
                if (character >= 'b' && character <= 'd')
                {
                    LA->state = 10;
                }

                // FIELD ID
                else
                {
                    LA->state = 14;
                }

                break;
            }

            // DIGIT
            if (isdigit(character))
            {
                LA->state = 3;
                break;
            }

            switch (character)
            {
            case '#':
                LA->state = 1;
                break;

            case '_':
                LA->state = 20;
                break;
            }

        // RECOGNISE T_NUM OR T_RNUM
        case 3:
            if (character == '.')
            {
                LA->state = 4;
            }
            else if (~isdigit(character))
            {
                token->tokenType = TK_NUM1;
            }
            break;
        }

        // CHECK AGAINST DFA - TO BE DONE LATER

        // EOF ENCOUNTERED
        // TODO: DIFFERENTIATE BETWEEN END OF INPUT AND END OF BUFFER
        if (character == '\0')
        {
            // RELOAD OTHER BUFER
            int res = readIntoBuffer(LA->twinBuffer, file);

            // ALL INPUT READ AND PROCESSED
            if (res == 0)
            {
                break;
            }
        }

        // GOT THE TOKEN
        break;
    }
    // GET THE LEXEME
    strncpy(token->lexeme, LA->twinBuffer->buffer + LA->begin, LA->forward - LA->begin);
    token->lineNo = LA->lineNo;

    // GET FINAL TOKEN
    token = getToken(token);
    // TODO: call dfa to get token type in case of id, num, rnum, funid, fieldid

    // ADVANCE BEGIN FOR NEXT TOKEN
    LA->begin = LA->forward;

    // RETURN TOKEN
    return token;
}

int main()
{
    // TEST THE TWIN BUFFER
    FILE *file = readTestFile("test_program.txt");

    // INITIALISE A TWIN BUFFER
    struct TwinBuffer twinBuffer = initialiseTwinBuffer();

    // READ INTO THE BUFFER
    readIntoBuffer(&twinBuffer, file);
    readIntoBuffer(&twinBuffer, file);

    // CHECK
    for (int i = 0; i < BUFFER_SIZE * 2 + 2; i += 1)
    {
        printf("%c", twinBuffer.buffer[i]);
    }
}
