// SCHEME: TWO-INPUT BUFFERS EACH OF SIZE N WITH SENTINEL CHARACTER "EOF" MARKING THE END OF EACH BUFFER

// INCLUDE LIBRARIES
#include <stdio.h>
#include <string.h>
#include "lexical.h"
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>
// PARAMETER N = NUMBER OF CHARACTERS READ WITH EACH RELOAD
#define END_OF_FILE_VAL 128
#define BUFFER_SIZE 150
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
    // printf("\n >> line no : %d ended\n", LA->lineNo);
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

char *strncustomcpy(struct LexicalAnalyzer *LA) // copy forward to begin in a string
{
    // as per the design, forward and begin can never be at the buffer end marker
    // forward - begin is the size of lexemme + eofs if occured

    // for (int i=0;i<2*BUFFER_SIZE+2;i++){
    //     printf("%c", LA->twinBuffer->buffer[i]);
    // }printf("\n");

    int startpos = LA->begin;

    int b = LA->begin, f = LA->forward;
    int numofchars = f - b;

    b = b % (2 * BUFFER_SIZE + 2);
    f = f % (2 * BUFFER_SIZE + 2);

    if (f >= b)
    {
        if (f > BUFFER_SIZE && BUFFER_SIZE > b)
        {
            numofchars--;
        }
    }
    else
    { // f<b
        // have to skip 1 or 2
        if (b < BUFFER_SIZE)
        { // b in first buffer
            numofchars--;
            numofchars--;
        }
        else
        { // b in second buffer
            if (f < BUFFER_SIZE)
            {
                numofchars--;
            }
            else
            {
                numofchars--;
                numofchars--;
            }
        }
    }

    char *a = (char *)malloc((numofchars + 1) * sizeof(char));

    if (a == NULL)
    {
        printf("Memory allocation in strncustomcpy failed.");
        return "ERR_1010";
    }

    // printf("%d\n",numofchars);

    int la_index = startpos - 1;
    for (int i = 0; i < numofchars; i++)
    {
        la_index = (la_index + 1) % (BUFFER_SIZE * 2 + 2);
        if (la_index == BUFFER_SIZE || la_index == BUFFER_SIZE * 2 + 1)
        {
            la_index = (la_index + 1) % (BUFFER_SIZE * 2 + 2);
        }
        // printf("%d %c\n",la_index,LA->twinBuffer->buffer[la_index]);
        a[i] = LA->twinBuffer->buffer[(la_index)];
    }
    a[numofchars] = '\0';

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

struct SymbolTableEntry *setErrorMessage(struct SymbolTableEntry *token, struct LexicalAnalyzer *LA, bool toIncludeString, char *errorMessage)
{
    // printf("Called");

    if (toIncludeString)
    {
        if (LA->forward == LA->begin)
            LA->forward++;
        char *readSymbol = strncustomcpy(LA);
        // printf("%d %d", LA->begin, LA->forward);
        // printf("Read symbol %s\n", readSymbol);
        // +1 for the null terminator
        int length = strlen(readSymbol) + 22; // "Unknown pattern: <>" + null terminator
        token->lexeme = (char *)malloc(length * sizeof(char));
        if (token->lexeme == NULL)
        {
            return NULL;
        }
        if (strlen(readSymbol) > 1)
        {
            sprintf(token->lexeme, "Unknown pattern <%s>", readSymbol);
        }
        else
        {
            sprintf(token->lexeme, "Unknown Symbol <%s>", readSymbol);
        }
    }
    else
    {
        token->lexeme = (char *)malloc(strlen(errorMessage) + 1);
        if (token->lexeme == NULL)
        {
            return NULL;
        }
        strncpy(token->lexeme, errorMessage, strlen(errorMessage) + 1);
    }
    token->tokenType = LEXICAL_ERROR;

    // Return to start
    returnToStart(LA);
    return token;
}

struct SymbolTableEntry *lexicalError(struct SymbolTableEntry *token, struct LexicalAnalyzer *LA)
{

    if (token->tokenType == TK_ID && strlen(token->lexeme) > MAX_ID_SIZE)
    {
        setErrorMessage(token, LA, false, "Variable Identifier is longer than the prescribed length of 20 characters.");
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

    if (state == TK_COMMENT)
    {
        token->lexeme = "%";
        token->tokenType = TK_COMMENT;
        resetBegin(LA);
        return token;
    }
    // DONT SET TOKEN WHEN DELIMITER
    if (state == CARRIAGE_RETURN || state == DELIMITER)
    {

        // DONT SET STATE OR LEXEME AND RETURN
        // RETURN TO START STATE AND RESET BEGIN TO FORWARD

        // ALL EXCEPT CARRIAGE RETUR ARRIVE ON READING A CHARACTER THAT MUST BE READ AGAIN
        // BEGIN MADE EQUAL TO FORWARD, BUT DECREMENT FORWARD BECASUSE IT GETS INCREMENTED IN LOOP

        returnToStart(LA);

        if (state == CARRIAGE_RETURN)
        {
            incrementLineNo(LA);
            // INCREMENT BOTH BEGIN AND FORWARD TO NEXT CHARACTER
            changeBegin(LA, 1);
            // OUTER WILL INCREMENT FORWARD
        }
        else
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
    else if (state == TK_RNUM1)
    {
        changeForward(LA, 1);
    }
    token->lexeme = strncustomcpy(LA);

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
    else if (state == TK_LT1 || state == TK_GT)
    {
        token->lexeme = strncustomcpy(LA);
    }

    // FINAL STATE WITHOUT ANY OTHER ACTIONS
    else
    {
        // INCREMENT FORWARD
        changeForward(LA, +1);
        token->lexeme = strncustomcpy(LA);

        // SET LEXEME

        // DONT HAVE TO WORRY ABOUT THE INCREMENT FORWARD IN LOOP BECASUE TOKEN IS RETURNED BEFORE INCREMENTß
    }

    // MULTIPLE ACCEPT STATES FOR THE SAME TOKEN
    if (token->tokenType == TK_RNUM1 || token->tokenType == TK_RNUM2)
    {
        token->tokenType = TK_RNUM;
    }

    if (token->tokenType == TK_NUM1 || token->tokenType == TK_NUM2)
    {
        token->tokenType = TK_NUM;
    }

    if (token->tokenType == TK_LT1 || token->tokenType == TK_LT2)
    {
        token->tokenType = TK_LT;
    }

    return token;
}

struct SymbolTableEntry *initialiseToken()
{

    struct SymbolTableEntry *token = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
    token->lexeme = NULL;
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
        character = LA->twinBuffer->buffer[LA->forward % (BUFFER_SIZE * 2 + 2)];

        // EOF: LAST CHARACTER OF INPUT
        if (character == EOF)
        {

            // END OF BUFFER
            if ((LA->forward % (BUFFER_SIZE * 2 + 2)) == BUFFER_SIZE || (LA->forward % (BUFFER_SIZE * 2 + 2)) == (2 * BUFFER_SIZE + 1))
            {
                // RELOAD OTHER BUFER
                readIntoBuffer(LA->twinBuffer);
                changeForward(LA, 1);
                // printf("RELOADED BUFFER\n");

                // IF BOTH ARE AT EOF: INCREMENT BEGIN TOO
                if (LA->begin == LA->forward)
                {
                    changeBegin(LA, 1);
                }
                continue;
            }

            // END OF INPUT
            else
            {
                // AT START STATE
                if (LA->state == 0)
                {
                    // printf("\nEnd of input. Finished scanning");
                    return NULL;
                }

                // CHANGE STATE
                LA->state = getNextState(LA->state, END_OF_FILE_VAL); // 128 for end of file

                // REACHED TRAP STATE
                if (LA->state == -1)
                {
                    setErrorMessage(token, LA, true, "");
                    return token;
                }

                // TAKE ACTIONS FOR THE STATE
                token = takeActions(LA, token);

                // AT START STATE
                if (LA->state == 0)
                {
                    // printf("\nEnd of input. Finished scanning");
                    setErrorMessage(token, LA, true, "");
                    return token;
                }

                if (token->tokenType == LEXICAL_ERROR)
                {
                    return token;
                }

                // HAVE TO RETURN
                if (token->tokenType != 0)
                {
                    // RESET BEGIN
                    resetBegin(LA);
                    return token;
                }
            }
        }
        // TODO: CHECK
        //  CHANGE STATE
        if (characterTypeMap[(int)character] == CT_INVALID && LA->state == 0)
        {
            setErrorMessage(token, LA, true, "");

            // INCREMENT FORWARD BECAUSE CHARACTER IS INVALID; CANNOT RESUME TOKENISATION
            changeForward(LA, 1);
            return token;
        }

        // GET NEXT STATE

        // printf("\nCHARACTER %c %d FROM STATE %d\xn", character, token->tokenType, LA->state);
        LA->state = getNextState(LA->state, (int)character);
        // printf("\nTO STATE %d\n", LA->state);

        // TRAP STATE
        if (LA->state == -1)
        {
            setErrorMessage(token, LA, true, "");
            // exit(0);
            return token;
        }
        // if (character == '<')
        // {
        //     printf("STATE %d\n", LA->state);
        // }

        // TAKE ACTIONS FOR THE STATE
        token = takeActions(LA, token);

        if (token->tokenType == LEXICAL_ERROR)
        {
            return token;
        }

        // HAVE TO RETURN
        if (token->tokenType != 0)
        {
            resetBegin(LA);
            return token;
        }

        // SIZE OF FUN_ID/TK_ID EXCEEDED

        // INCREMENT FORWARD
        changeForward(LA, 1);
    }

    return NULL;
}

struct LexicalAnalyzer *initialiseLA(struct TwinBuffer *twinBuffer)
{
    struct LexicalAnalyzer *LA;
    LA = (struct LexicalAnalyzer *)malloc(sizeof(struct LexicalAnalyzer));

    LA->lineNo = 1;
    LA->begin = 0;
    LA->forward = 0;
    LA->state = 0;
    LA->twinBuffer = twinBuffer;
    return LA;
}



// int main()
// {

//     // printf("ENTERED MAIN\n");

//     // INITIALISE THE SYMBOL TABLE
//     insertAllKeywords();

//     // printf("KEYWORDS\n");

//     // TEST THE TWIN BUFFER
//     FILE *file = readTestFile("test_program_with_errors.txt");

//     // INITIALISE A TWIN BUFFER
//     struct TwinBuffer *twinBuffer = initialiseTwinBuffer(file);

//     // INITIALISE LA
//     struct LexicalAnalyzer *LA = initialiseLA(twinBuffer);

//     // printf("LA INITIALISED\n");

//     // START SCANNING
//     readIntoBuffer(twinBuffer);

//     // printf("READ INPUT\n");

//     // THE TOKEN
//     struct SymbolTableEntry *token;

//     // printf("STARTING SCANNING\n");

//     while ((token = scanToken(LA)))
//     {
//         // printf("HU");
//         printf("(%s : %s) \n", TokenToString(token->tokenType), token->lexeme);
//     }

//     // printSymbolTable();
// }
