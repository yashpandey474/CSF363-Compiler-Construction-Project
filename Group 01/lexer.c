// GROUP 01
// NAMES	                 BITS ID
// ------------------------------------------
// Yash Pandey	             2021A7PS0661P
// Achinthya Hebbar S	     2021A7PS1457P
// Ujjwal Aggarwal	         2021A7PS2427P
// Hardik Gupta 	         2021A7PS2421P
// Agrawal Vansh Anil        2021A7PS2215P


// INCLUDE LIBRARIES
#include "lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

FILE *readTestFile(char *file_path) {
  FILE *file = fopen(file_path, "r");

  // UNABLE TO READ FILE
  if (file == NULL) {
    printf("Error opening file %s\n", file_path);
    return NULL;
  }

  return file;
}
/// hello shashank, if you are reading this, go fuck yourself..
int getSizeOfCustomString(lexicalAnalyser LA) {
  int b = LA->begin;
  int f = LA->forward;
  int pandey_chootiya = 0;
  if ((b < BUFFER_SIZE && f < BUFFER_SIZE) ||
      (b > BUFFER_SIZE && f > BUFFER_SIZE)) {
    if (b < f) {
      pandey_chootiya = f - b;
    } else {
      pandey_chootiya = 2 * BUFFER_SIZE - f + b;
    }
  } else if (b < BUFFER_SIZE && f > BUFFER_SIZE) {
    pandey_chootiya = f - b - 1;
  } else if (b > BUFFER_SIZE && f < BUFFER_SIZE) {
    pandey_chootiya = 2 * BUFFER_SIZE + 1 - f + b;
  } else {
    if (b == BUFFER_SIZE || b == 2 * BUFFER_SIZE + 1) {
      pandey_chootiya--;
    }
    if (f == BUFFER_SIZE || f == 2 * BUFFER_SIZE + 1) {
      pandey_chootiya--;
    }
  }
  return pandey_chootiya;
}

char *strncustomcpy(lexicalAnalyser LA) // copy forward to begin in a string
{
  // as per the design, forward and begin can never be at the buffer end marker
  // forward - begin is the size of lexemme + eofs if occured

  // for (int i=0;i<2*BUFFER_SIZE+2;i++){
  //     printf("%c", LA->bufferArray->buffer[i]);
  // }printf("\n");

  int twinBufferSize = 2 * BUFFER_SIZE + 2;
  int numchars = getSizeOfCustomString(LA) + 1;

  char *a = (char *)malloc(numchars);
  if (a == NULL) {
    printf("Memory allocation in strncustomcpy failed.");
    return "ERR_1010";
  }
  int b = LA->begin % twinBufferSize;
  int f = LA->forward % twinBufferSize;
  int i = 0;
  for (; b != f;) {
    if (LA->bufferArray->buffer[b] != EOF) {
      a[i] = LA->bufferArray->buffer[b];
      i++;
      b += 1;
    } else {
      b += 1;
      b %= twinBufferSize;
    }
  }
  a[numchars - 1] = '\0';
  return a;
}

int getStream(twinBuffer bufferArray) {
  char *buffer;
  if (bufferArray->readingFirst) {
    buffer = bufferArray->buffer;
  } else {
    buffer = bufferArray->buffer + BUFFER_SIZE + 1;
  }
  // READING ALTERNATE BUFFER
  bufferArray->readingFirst = 1 - bufferArray->readingFirst;
  size_t read_bytes =
      fread(buffer, sizeof(char), BUFFER_SIZE, bufferArray->file);

  // MARK END OF INPUT
  if (read_bytes < BUFFER_SIZE) {
    // MARK WITH ANOTHER EOF
    buffer[read_bytes] = EOF;
  }

  // FOR LA TO RECOGNISE END OF INPUT FROM END OF BUFFER
  return read_bytes;
}

// CREATE THE BUFFER AND RETURN IT
twinBuffer initialiseTwinBuffer(FILE *file) {
  // INITIALISE BUFFER
  twinBuffer bufferArray = (twinBuffer)malloc(sizeof(TwinBuffer));

  bufferArray->readingFirst = 1;
  bufferArray->buffer[BUFFER_SIZE] = EOF;
  bufferArray->buffer[2 * BUFFER_SIZE + 1] = EOF;
  bufferArray->file = file;

  return bufferArray;
}

void returnToStart(lexicalAnalyser LA) {
  LA->state = 0;
  LA->begin = LA->forward;
}

struct SymbolTableEntry *setErrorMessage(struct SymbolTableEntry *token,
                                         lexicalAnalyser LA,
                                         bool toIncludeString,
                                         char *errorMessage) {
  // printf("Called");

  if (toIncludeString) {
    if (LA->forward == LA->begin)
      LA->forward++;
    char *readSymbol = strncustomcpy(LA);
    // printf("%d %d", LA->begin, LA->forward);
    // printf("Read symbol %s\n", readSymbol);
    // +1 for the null terminator
    int length =
        strlen(readSymbol) + 22; // "Unknown pattern: <>" + null terminator
    token->lexeme = (char *)malloc(length * sizeof(char));
    if (token->lexeme == NULL) {
      return NULL;
    }
    if (strlen(readSymbol) > 1) {
      sprintf(token->lexeme, "Unknown pattern <%s>", readSymbol);
    } else {
      sprintf(token->lexeme, "Unknown Symbol <%s>", readSymbol);
    }
  } else {
    token->lexeme = (char *)malloc(strlen(errorMessage) + 1);
    if (token->lexeme == NULL) {
      return NULL;
    }
    strncpy(token->lexeme, errorMessage, strlen(errorMessage) + 1);
  }
  token->tokenType = LEXICAL_ERROR;

  // Return to start
  returnToStart(LA);
  return token;
}

struct SymbolTableEntry *lexicalError(struct SymbolTableEntry *token,
                                      lexicalAnalyser LA) {

  if (token->tokenType == TK_ID && strlen(token->lexeme) > MAX_ID_SIZE) {
    setErrorMessage(token, LA, false,
                    "Variable Identifier is longer than the prescribed length "
                    "of 20 characters.");
  } else if (token->tokenType == TK_FUNID &&
             strlen(token->lexeme) > MAX_FUNID_SIZE) {
    setErrorMessage(
        token, LA, ' ',
        "Lexical Error: Function Identifier of greater than maximum size");
  }
  return token;
}

// FUNCTION TO GET CORRESPONDING NUMBER
void equivalentNumber(lexicalAnalyser lex, int flag, tokenInfo token) {
  // printf("EQV NUMS %s\n", token->lexeme);
  if (flag == TK_NUM1 || flag == TK_NUM2) {
    // INTEGER
    token->intValue = atoi(token->lexeme);
  } else {
    // DOUBLE
    token->doubleValue = atof(token->lexeme);
  }
}
void changeForward(lexicalAnalyser LA, int flag) {
  // FLAG IS 1 FOR INCREMENT AND -1 FOR DECREMENT
  LA->forward = (LA->forward + flag);
}

void changeBegin(lexicalAnalyser LA, int flag) {
  // FLAG IS 1 FOR INCREMENT AND -1 FOR DECREMENT
  LA->begin = (LA->begin + flag);
}

// TAKE ACTIONS BASED ON THE FINAL STATE AND RETURN A TOKEN
struct SymbolTableEntry *takeActions(lexicalAnalyser LA,
                                     struct SymbolTableEntry *token) {
  // NON FINAL STATE32
  int state = LA->state;
  if (state < FINAL_STATE_OFFSET) {
    return token;
  }

  token->lineNo = LA->lineNo;

  state -= FINAL_STATE_OFFSET;

  if (state == TK_COMMENT) {
    token->lexeme = "%";
    token->tokenType = TK_COMMENT;
    LA->begin = LA->forward;
    return token;
  }
  // DONT SET TOKEN WHEN DELIMITER
  if (state == CARRIAGE_RETURN || state == DELIMITER) {

    // DONT SET STATE OR LEXEME AND RETURN
    // RETURN TO START STATE AND RESET BEGIN TO FORWARD

    // ALL EXCEPT CARRIAGE RETUR ARRIVE ON READING A CHARACTER THAT MUST BE READ
    // AGAIN BEGIN MADE EQUAL TO FORWARD, BUT DECREMENT FORWARD BECASUSE IT GETS
    // INCREMENTED IN LOOP

    returnToStart(LA);

    if (state == CARRIAGE_RETURN) {
      LA->lineNo += 1;
      // INCREMENT BOTH BEGIN AND FORWARD TO NEXT CHARACTER
      LA->begin += 1;
      // OUTER WILL INCREMENT FORWARD
    } else {
      LA->forward -= 1;
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
    LA->forward -= 1;
  } else if (state == TK_RNUM1) {
    LA->forward += 1;
  }
  token->lexeme = strncustomcpy(LA);

  // SET LEXEME
  // EQUIVALENT NUMBER
  if (state == TK_RNUM1 || state == TK_RNUM2 || state == TK_NUM1 ||
      state == TK_NUM2) {
    // COMPUTE NUMBER
    equivalentNumber(LA, state, token);
  }

  // GET THE SYMBOL TABLE ENTRY

  else if (state == TK_FIELDID || state == TK_RUID) {
    getToken(token);
  }

  else if (state == TK_ID || state == TK_FUNID) {
    token = lexicalError(token, LA);
    if (token->tokenType != LEXICAL_ERROR) {
      getToken(token);
    }
  } else if (state == TK_LT1 || state == TK_GT) {
    token->lexeme = strncustomcpy(LA);
  }

  // FINAL STATE WITHOUT ANY OTHER ACTIONS
  else {
    // INCREMENT FORWARD
    LA->forward += 1;
    token->lexeme = strncustomcpy(LA);

    // SET LEXEME

    // DONT HAVE TO WORRY ABOUT THE INCREMENT FORWARD IN LOOP BECASUE TOKEN IS
    // RETURNED BEFORE INCREMENTß
  }

  // MULTIPLE ACCEPT STATES FOR THE SAME TOKEN
  if (token->tokenType == TK_RNUM1 || token->tokenType == TK_RNUM2) {
    token->tokenType = TK_RNUM;
  }

  if (token->tokenType == TK_NUM1 || token->tokenType == TK_NUM2) {
    token->tokenType = TK_NUM;
  }

  if (token->tokenType == TK_LT1 || token->tokenType == TK_LT2) {
    token->tokenType = TK_LT;
  }

  return token;
}

struct SymbolTableEntry *initialiseToken() {

  struct SymbolTableEntry *token =
      (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
  token->lexeme = NULL;
  // token->lexeme = (char *)malloc(sizeof(char)*MAX_LEXEME_SIZE);
  // lexeme is initialised later for efficiency
  token->intValue = 0;
  token->doubleValue = 0;
  token->lineNo = 0;
  token->tokenType = 0;
  return token;
}

tokenInfo getNextToken(lexicalAnalyser LA) {
  // INITIALSE TOKEN
  struct SymbolTableEntry *token = initialiseToken();

  // RESET TO START STATE
  LA->state = 0;

  char character;
  // while(1)
  while (1) {

    // GET CHARACTER CURRENTLY BEING READ
    character = LA->bufferArray->buffer[LA->forward % (BUFFER_SIZE * 2 + 2)];

    // EOF: LAST CHARACTER OF INPUT
    if (character == EOF) {

      // END OF BUFFER
      if ((LA->forward % (BUFFER_SIZE * 2 + 2)) == BUFFER_SIZE ||
          (LA->forward % (BUFFER_SIZE * 2 + 2)) == (2 * BUFFER_SIZE + 1)) {
        // RELOAD OTHER BUFER
        getStream(LA->bufferArray);
        LA->forward += 1;
        // printf("RELOADED BUFFER\n");

        // IF BOTH ARE AT EOF: INCREMENT BEGIN TOO
        if (LA->begin == LA->forward) {
          LA->begin += 1;
        }
        continue;
      }

      // END OF INPUT
      else {
        // AT START STATE
        if (LA->state == 0) {
          // printf("\nEnd of input. Finished scanning");
          return NULL;
        }

        // CHANGE STATE
        LA->state =
            getNextState(LA->state, END_OF_FILE_VAL); // 128 for end of file

        // REACHED TRAP STATE
        if (LA->state == -1) {
          setErrorMessage(token, LA, true, "");
          return token;
        }

        // TAKE ACTIONS FOR THE STATE
        token = takeActions(LA, token);

        // AT START STATE
        if (LA->state == 0) {
          // printf("\nEnd of input. Finished scanning");
          setErrorMessage(token, LA, true, "");
          return token;
        }

        if (token->tokenType == LEXICAL_ERROR) {
          return token;
        }

        // HAVE TO RETURN
        if (token->tokenType != 0) {
          // RESET BEGIN
          LA->begin = LA->forward;
          return token;
        }
      }
    }
    // TODO: CHECK
    //  CHANGE STATE
    if (characterTypeMap[(int)character] == CT_INVALID && LA->state == 0) {
      setErrorMessage(token, LA, true, "");

      // INCREMENT FORWARD BECAUSE CHARACTER IS INVALID; CANNOT RESUME
      // TOKENISATION
      LA->forward += 1;
      return token;
    }

    // GET NEXT STATE

    // printf("\nCHARACTER %c %d FROM STATE %d\xn", character, token->tokenType,
    // LA->state);
    LA->state = getNextState(LA->state, (int)character);
    // printf("\nTO STATE %d\n", LA->state);

    // TRAP STATE
    if (LA->state == -1) {
      setErrorMessage(token, LA, true, "");
      // exit(0);
      return token;
    }

    // TAKE ACTIONS FOR THE STATE
    token = takeActions(LA, token);

    if (token->tokenType == LEXICAL_ERROR) {
      return token;
    }

    // HAVE TO RETURN
    if (token->tokenType != 0) {
      LA->begin = LA->forward;
      return token;
    }

    // SIZE OF FUN_ID/TK_ID EXCEEDED

    // INCREMENT FORWARD
    LA->forward += 1;
  }

  return NULL;
}

lexicalAnalyser initialiseLA(twinBuffer bufferArray) {
  lexicalAnalyser LA;
  LA = (lexicalAnalyser)malloc(sizeof(LexicalAnalyser));

  LA->lineNo = 1;
  LA->begin = 0;
  LA->forward = 0;
  LA->state = 0;
  LA->bufferArray = bufferArray;
  return LA;
}