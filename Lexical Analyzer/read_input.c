// SCHEME: TWO-INPUT BUFFERS EACH OF SIZE N WITH SENTINEL CHARACTER "EOF" MARKING THE END OF EACH BUFFER

// INCLUDE LIBRARIES
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

// PARAMETER N = NUMBER OF CHARACTERS READ WITH EACH RELOAD
#define END_OF_FILE_VAL 128
#define BUFFER_SIZE 2569
#define FINAL_STATE_OFFSET NUM_NON_ACCEPT_STATES + 1 // this is same as NON ACCEPT STATES + 1
#define MAX_LEXEME_SIZE 100
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
    printf("\n >> line no : %d\n", LA->lineNo);
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
char* strncustomcpy(int startpos, int numofchars,struct LexicalAnalyzer* LA)
{
    char* a=(char*) malloc((numofchars+1)*sizeof(char));
    if(a==NULL)
    {
        printf("Memory allocation in strncustomcpy failed.");
    }
    for(int i=0;i<numofchars;i++)
    {
        a[i]=LA->twinBuffer->buffer[(startpos+i) %(BUFFER_SIZE * 2 + 2)];
    }
    a[numofchars]='\0';
    return a;
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
struct SymbolTableEntry *setErrorMessage(struct SymbolTableEntry *token, struct LexicalAnalyzer *LA, char additional, char *errorMessage)
{
    char* readSymbol=strncustomcpy(LA->begin,LA->forward - LA->begin,LA);

    //SET TO DEFAULT SO THAT IT CAN CONTINUE SCANNING WITHOUT RETURNING
    token->tokenType = LEXICAL_ERROR;
    token->lexeme = (char *)realloc(token->lexeme, strlen(errorMessage) + 40 + strlen(readSymbol));
    sprintf(token->lexeme, "String %s : %s %c, [Line no. %d]", readSymbol, errorMessage, additional, LA->lineNo + 1);

    printf("LEXICAL ERROR: %s\n", token->lexeme);
    returnToStart(LA);
    return token;
}

struct SymbolTableEntry *lexicalError(struct SymbolTableEntry *token, struct LexicalAnalyzer *LA)
{

    if (token->tokenType == TK_ID && strlen(token->lexeme) > MAX_ID_SIZE)
    {
        setErrorMessage(token, LA, ' ', "Lexical Error: Token Identifier of greater than maximum size");
    }
    else if (token->tokenType == TK_FUNID && strlen(token->lexeme) > MAX_FUNID_SIZE)
    {
        setErrorMessage(token, LA, ' ', "Lexical Error: Function Identifier of greater than maximum size");
    }
    return token;
}

// FUNCTION TO GET CORRESPONDING NUMBER
void equivalentNumber(struct LexicalAnalyzer *lex, int flag, struct SymbolTableEntry *token)
{
    // printf("EQV NUMS %s\n", token->lexeme);
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
    LA->forward = (LA->forward + flag);
}

void changeBegin(struct LexicalAnalyzer *LA, int flag)
{
    // FLAG IS 1 FOR INCREMENT AND -1 FOR DECREMENT
    LA->begin = (LA->begin + flag);
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

    // DONT SET TOKEN WHEN DELIMITER
    if (state == CARRIAGE_RETURN || state == DELIMITER || state == TK_COMMENT)
    {
        // RETURN TO START STATE
        returnToStart(LA);

        // DONT SET STATE OR LEXEME AND RETURN

        if (state == CARRIAGE_RETURN)
        {
            incrementLineNo(LA);

            // RESET BEGIN AND INCREMENT FORWARD
            changeBegin(LA, 1);
        }

        if (state == DELIMITER)
        {
            changeForward(LA, -1);
        }

        // printf("TOOK ACTIONS");
        return token;
    }

    // SET TOKEN TYPE [SET FOR TOKEN, NOT SET FOR CARRIAGE_RETURN OR DELIMITER]
    token->tokenType = state;

    // DOUBLE STAR STATES
    if (state == TK_NUM2 || state == TK_LT2)
    {
        // DECREMENT FORWARD POINTER
        changeForward(LA, -1);
    }
    token->lexeme=strncustomcpy(LA->begin,LA->forward - LA->begin,LA);

    // SET LEXEME
    // EQUIVALENT NUMBER
    if (state == TK_RNUM1 || state == TK_RNUM2 || state == TK_NUM1 || state == TK_NUM2)
    {
        // COMPUTE NUMBER
        equivalentNumber(LA, state, token);
    }

    // GET THE SYMBOL TABLE ENTRY

    else if (state == TK_FIELDID || state == TK_RUID)
    {
        token = getToken(token);
    }

    else if (state == TK_ID || state == TK_FUNID)
    {
        token = lexicalError(token, LA);
        if (token->tokenType != LEXICAL_ERROR)
        {
            token = getToken(token);
        }
    }

    // FINAL STATE WITHOUT ANY OTHER ACTIONS
    else if (state != TK_COMMENT && state != TK_LT2 && state != TK_NUM2)
    {
        // INCREMENT FORWARD
        changeForward(LA, +1);
        token->lexeme=strncustomcpy(LA->begin,LA->forward - LA->begin,LA);

        // SET LEXEME
    }

    //MULTIPLE ACCEPT STATES FOR THE SAME TOKEN
    if (token->tokenType == TK_RNUM1 || token->tokenType == TK_RNUM2){
        token->tokenType = TK_RNUM;
    }
    
    if (token->tokenType == TK_NUM1 || token->tokenType == TK_NUM2){
        token->tokenType = TK_NUM;
    }

    if (token->tokenType == TK_LT1 || token->tokenType == TK_LT2){
        token->tokenType = TK_LT;
    }

    return token;
}

struct SymbolTableEntry *initialiseToken()
{

    struct SymbolTableEntry *token = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
    token->lexeme=NULL;
    // token->lexeme = (char *)malloc(sizeof(char)*MAX_LEXEME_SIZE);
    // lexeme is initialised later for efficiency
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
    // while(1)
    while (1)
    {

        // GET CHARACTER CURRENTLY BEING READ
        character = LA->twinBuffer->buffer[LA->forward% (BUFFER_SIZE * 2 + 2)];
        // printf("begin %d forward %d",LA->begin,LA->forward);
        // printf("current character: %c, %d\n", character,character);

        // CHECK FOR ILLEGAL CHARACTER
        
        if (character == EOF)
        {

            // END OF BUFFER
            if (LA->forward == BUFFER_SIZE || LA->forward == (2 * BUFFER_SIZE + 1))
            {
                // RELOAD OTHER BUFER
                readIntoBuffer(LA->twinBuffer);
            }

            // END OF INPUT 
            else
            {
                if (LA->state == 0)
                {
                    printf("\nEnd of input. Finished scanning");
                    return NULL;
                }

                // CHANGE STATE
                LA->state = getNextState(LA->state, END_OF_FILE_VAL); // 128 for end of file

                //REACHED TRAP STATE
                if (LA->state == -1)
                {
                    setErrorMessage(token, LA, character, "REACHED TRAP STATE CHARACTER");
                    token=initialiseToken();
                    //CONTINUE TOKENISATION FROM THIS CHARACTER
                    continue;
                }

                // TAKE ACTIONS FOR THE STATE
                token = takeActions(LA, token);

                // HAVE TO RETURN
                if (token->tokenType != 0)
                {
                    // RESET BEGIN
                    resetBegin(LA);
                    return token;
                }

                //NO CONTINUE IF EOF AND LEXICAL ERROR [NOTHING ELSE TO SCAN]
                setErrorMessage(token, LA, character, "COULD NOT REACH ACCEPT STATE");
                
                //CONTINUE SCANNING
                continue;
            }
        }
        // CHANGE STATE
        if (characterTypeMap[(int)character] == CT_INVALID && LA->state == 0)
        {
            setErrorMessage(token, LA, character, "INVALID CHARACTER");

            //INCREMENT FORWARD BECAUSE CHARACTER IS INVALID; CANNOT RESUME TOKENISATION 
            changeForward(LA, 1);

            //CONTINUE SCANNING
            continue;
        }

        //GET NEXT STATE
        LA->state = getNextState(LA->state, (int)character);

        printf("CHARACTER %c STATE %d\n", character, LA->state);
        
        //TRAP STATE
        if (LA->state == -1)
        {
            setErrorMessage(token, LA, character, "REACHED TRAP STATE CHARACTER");

            //PRINT AND RETURN NEXT TOKEN
            continue;
        }

        // TAKE ACTIONS FOR THE STATE
        token = takeActions(LA, token);

        if (token->tokenType == LEXICAL_ERROR){
            token = initialiseToken();
            
            continue;
        }
        // HAVE TO RETURN
        if (token->tokenType != 0)
        {
            resetBegin(LA);
            return token;
        }

        //SIZE OF FUN_ID/TK_ID EXCEEDED



        // INCREMENT FORWARD
        changeForward(LA, 1);
    }

    return NULL;
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

    // printf("ENTERED MAIN\n");

    // INITIALISE THE SYMBOL TABLE
    insertAllKeywords();

    // printf("KEYWORDS\n");

    // TEST THE TWIN BUFFER
    FILE *file = readTestFile("test_program_with_errors.txt");

    // INITIALISE A TWIN BUFFER
    struct TwinBuffer *twinBuffer = initialiseTwinBuffer(file);

    // INITIALISE LA
    struct LexicalAnalyzer *LA = initialiseLA(twinBuffer);

    // printf("LA INITIALISED\n");

    // START SCANNING
    readIntoBuffer(twinBuffer);

    // printf("READ INPUT\n");

    // THE TOKEN
    struct SymbolTableEntry *token;

    // printf("STARTING SCANNING\n");

    while ((token = scanToken(LA)))
    {
        // printf("HU");
        printf("(%s : %s) ", TokenToString(token->tokenType), token->lexeme);
    }

    // printSymbolTable();
}
