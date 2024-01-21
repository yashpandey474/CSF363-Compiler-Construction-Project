// SCHEME: TWO-INPUT BUFFERS EACH OF SIZE N WITH SENTINEL CHARACTER "EOF" MARKING THE END OF EACH BUFFER

// INCLUDE LIBRARIES
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include <stdlib.h>
#include <ctype.h>

// PARAMETER N = NUMBER OF CHARACTERS READ WITH EACH RELOAD
#define BUFFER_SIZE 256
// TYPICALLY 256 BYTES

// EOF CHARACTERS AT END OF BOTH BUFFERS ALWAYS

// FUNCTIONS TO READ "RELOAD" INTO BUFFER FROM A FILE

// CODE TO READ FROM THE BUFFERS CHARACTER BY CHARACTER
struct TwinBuffer
{
    //0 -> READING SECOND BUFFER, 1 -> READING FIRST BUFFER
    int readingFirst;
    char buffer[BUFFER_SIZE * 2 + 2];
};

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
void readIntoBuffer(struct TwinBuffer *twinBuffer, FILE *file)
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

    //MARK END OF INPUT
    if (read_bytes < BUFFER_SIZE){
        //MARK WITH ANOTHER EOF
        buffer[read_bytes] = '\0';
    }

    //FOR LA TO RECOGNISE END OF INPUT FROM END OF BUFFER
    return read_bytes
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

struct SymbolTableEntry * scanToken(struct LexicalAnalyzer LA, struct TwinBuffer *twinBuffer, FILE *file)
{

    struct SymbolTableEntry *token;
    token = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry *));
    int state = 0;
    while (1)
    {
        // GET CHARACTER CURRENTLY BEING READ
        char character = twinBuffer->buffer[LA.forward];

        // TODO: GENERALISE FOR ALL DELIMITERS;
        // INCRMENT LINENO
        if (character == '\n')
        {
            LA.lineNo += 1;
        }

        if (character == '\t'|| character == ' '){
            continue;
        }

        //END OF PROGRAM ?
        

        switch(state){
            //START STATE
            case 0:
                //LOWERCASE LETTER
                if (character >= 'a' && character <= 'z'){

                    if (character >= 'b' && character <= 'd'){
                        state = 10;
                        break;
                    }

                }

                //UPPERCASE LETTER
                if (character >= 'A' && character <= 'Z'){

                }

                //DIGIT
                if (isdigit(character)){
                    state = 3;
                    break;
                }


                switch(character){
                    case '#':
                        state = 1;
                        break;

                    case '_':
                        state = 20;
                        break;
                }

            //RECOGNISE T_NUM OR T_RNUM
            case 3:
                if (isdigit(character)){
                    
                }

        }
        
        // CHECK AGAINST DFA - TO BE DONE LATER

        
        // EOF ENCOUNTERED
        // TODO: DIFFERENTIATE BETWEEN END OF INPUT AND END OF BUFFER
        if (character == '\0')
        {
            // RELOAD OTHER BUFER
            int res = readIntoBuffer(twinBuffer, file);
            if (res == 0){
                //GET THE TOKEN OR RETURN 0?
            }
        }

        //INCREMENT FORWARD
        LA.forward = (LA.forward + 1) % (BUFFER_SIZE * 2 + 2);

        // GOT THE TOKEN
        break;
    }
    // GET THE LEXEME
    strncpy(token->lexeme, twinBuffer->buffer + LA.begin, LA.forward - LA.begin + 1);
    token->lineNo = LA.lineNo;

    // GET FINAL TOKEN
    token = insertIntoSymbolTable(token);
    // TODO: call dfa to get token type in case of id, num, rnum, funid, fieldid

    // ADVANCE BEGIN FOR NEXT TOKEN
    LA.begin = LA.forward;

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
