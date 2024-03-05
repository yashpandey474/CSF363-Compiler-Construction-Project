// GROUP 01
// NAMES	                 BITS ID
// ------------------------------------------
// Yash Pandey	             2021A7PS0661P
// Achinthya Hebbar S	     2021A7PS1457P
// Ujjwal Aggarwal	         2021A7PS2427P
// Hardik Gupta 	         2021A7PS2421P
// Agrawal Vansh Anil        2021A7PS2215P

#include "lexer.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT_SECOND_DIMENSION 37 // number of character types
#define STATE_ARRAY_SIZE_TEMP_GEN 200
int nextStateTemp[STATE_ARRAY_SIZE_TEMP_GEN];
int checkStateTemp[STATE_ARRAY_SIZE_TEMP_GEN];
int defaultArrayTemp[NUM_STATES];
int offsetArrayTemp[NUM_STATES];
CharacterType characterTypeMapTemp[129];

char outputFileName[] = "output_transition_table_generator.txt";
void printArray(const char *name, int *array, int size, const char *type,
                const char *constantName) {
  FILE *file = fopen(outputFileName, "a");
  if (file == NULL) {
    printf("Error opening file\n");
    return;
  }
  fprintf(file, "%s %s[%s] = {", type, name, constantName);
  for (int i = 0; i < size; ++i) {
    fprintf(file, "%d", array[i]);
    if (i < size - 1) {
      fprintf(file, ",");
    }
  }
  fprintf(file, "};\n");
  fclose(file);
}
void initialisetooneNumber(int **input, int rownumber, int val) {
  defaultArrayTemp[rownumber] = val;
}

void initialiseforDigit(int **input, int rownumber, int next) {
  input[rownumber][characterTypeMapTemp['1']] = next;
  input[rownumber][characterTypeMapTemp['2']] = next;
}
void initialiseforIdLetter(int **input, int rownumber, int next) {
  input[rownumber][characterTypeMapTemp['b']] = next;
}
void initialiseforLowerLetter(int **input, int rownumber, int next) {
  initialiseforIdLetter(input, rownumber, next);
  input[rownumber][characterTypeMapTemp['a']] = next;
}
void initialiseforLetter(int **input, int rownumber, int next) {
  input[rownumber][characterTypeMapTemp['A']] = next;
  input[rownumber][characterTypeMapTemp['E']] = next;
  initialiseforLowerLetter(input, rownumber, next);
}
void initialiseforIdDigit(int **input, int rownumber, int next) {
  input[rownumber][characterTypeMapTemp['2']] = next;
}

void initializeCharacterTypeMap() {
  characterTypeMapTemp[128] = CT_EOF;
  for (int i = 0; i < 128; ++i)

  {
    if (i >= 'a' && i <= 'z') {
      if (i >= 'b' && i <= 'd') {
        characterTypeMapTemp[i] = CT_ID_LETTER;
      } else {
        characterTypeMapTemp[i] = CT_LETTER_LOWER_EXCEPT_ID_LETTER;
      }
    } else if (i >= 'A' && i <= 'Z') {
      if (i == 'E')
        characterTypeMapTemp[i] = CT_EXPONENT;
      else {
        characterTypeMapTemp[i] = CT_LETTER_UPPER;
      }
    } else if (i >= '0' && i <= '9') {
      if (i >= '2' && i <= '7') {
        characterTypeMapTemp[i] = CT_ID_DIGIT;
      } else {
        characterTypeMapTemp[i] = CT_DIGIT_EXCEPT_ID_DIGIT;
      }
    } else {
      switch (i) {
      case '\t':
      case ' ':
        characterTypeMapTemp[i] = CT_DELIMITER;
        break;
      case 10:
        characterTypeMapTemp[i] = CT_CARRIAGE_RETURN;
        break;
      case '_':
        characterTypeMapTemp[i] = CT_UNDERSCORE;
        break;
      case '>':
        characterTypeMapTemp[i] = CT_GREATER;
        break;
      case '<':
        characterTypeMapTemp[i] = CT_LESSER;
        break;
      case '=':
        characterTypeMapTemp[i] = CT_EQUAL;
        break;
      case ',':
        characterTypeMapTemp[i] = CT_COMMA;
        break;
      case ';':
        characterTypeMapTemp[i] = CT_SEM;
        break;
      case ':':
        characterTypeMapTemp[i] = CT_COLON;
        break;
      case '.':
        characterTypeMapTemp[i] = CT_DOT;
        break;
      case '(':
        characterTypeMapTemp[i] = CT_ROUND_OPEN;
        break;
      case ')':
        characterTypeMapTemp[i] = CT_ROUND_CLOSE;
        break;
      case '[':
        characterTypeMapTemp[i] = CT_SQUARE_OPEN;
        break;
      case ']':
        characterTypeMapTemp[i] = CT_SQUARE_CLOSE;
        break;
      case '+':
        characterTypeMapTemp[i] = CT_PLUS;
        break;
      case '-':
        characterTypeMapTemp[i] = CT_MINUS;
        break;
      case '*':
        characterTypeMapTemp[i] = CT_MUL;
        break;
      case '/':
        characterTypeMapTemp[i] = CT_DIV;
        break;
      case '&':
        characterTypeMapTemp[i] = CT_AND;
        break;
      case '|':
        characterTypeMapTemp[i] = CT_OR;
        break;
      case '!':
        characterTypeMapTemp[i] = CT_EXCLAMATION;
        break;
      case '~':
        characterTypeMapTemp['~'] = CT_TILDE;
        break;
      case '%':
        characterTypeMapTemp[i] = CT_PERCENT;
        break;
      case '@':
        characterTypeMapTemp[i] = CT_AT_THE_RATE;
        break;
      case '#':
        characterTypeMapTemp[i] = CT_HASH;
        break;
      default:
        characterTypeMapTemp[i] = CT_INVALID;
      }
    }
  }
}

int reinitialiseArrays() {

  printf("Started with the execution !!!\n");
  initializeCharacterTypeMap();
  // the length of ans and the number of columns of input are randomly chosen

  int s = NUM_NON_ACCEPT_STATES + 1;

  for (int i = 0; i < NUM_STATES; i += 1) {
    offsetArrayTemp[i] = 0;
    defaultArrayTemp[i] = -1;
  }

  for (int i = 0; i < STATE_ARRAY_SIZE_TEMP_GEN; i++) {
    nextStateTemp[i] = -1;
    checkStateTemp[i] = -1;
  }
  int **input = (int **)malloc(NUM_STATES * sizeof(int *));

  for (int i = 0; i < NUM_STATES; ++i) {
    input[i] = (int *)malloc(INPUT_SECOND_DIMENSION * sizeof(int));
  }

  for (int i = 0; i < NUM_STATES; i++) {
    for (int j = 0; j < INPUT_SECOND_DIMENSION; j++) {
      input[i][j] = -1;
    }
  }

  input[0][characterTypeMapTemp['~']] = s + TK_NOT;
  input[0][characterTypeMapTemp['/']] = s + TK_DIV;
  input[0][characterTypeMapTemp['*']] = s + TK_MUL;
  input[0][characterTypeMapTemp['-']] = s + TK_MINUS;
  input[0][characterTypeMapTemp['+']] = s + TK_PLUS;
  input[0][characterTypeMapTemp['.']] = s + TK_DOT;
  input[0][characterTypeMapTemp[',']] = s + TK_COMMA;
  input[0][characterTypeMapTemp[':']] = s + TK_COLON;
  input[0][characterTypeMapTemp[';']] = s + TK_SEM;
  input[0][characterTypeMapTemp[')']] = s + TK_CL;
  input[0][characterTypeMapTemp['(']] = s + TK_OP;
  input[0][characterTypeMapTemp['[']] = s + TK_SQL;
  input[0][characterTypeMapTemp[']']] = s + TK_SQR;
  input[0][characterTypeMapTemp['\n']] = s + CARRIAGE_RETURN;
  input[s + CARRIAGE_RETURN][characterTypeMapTemp['\n']] = s + CARRIAGE_RETURN;

  input[0][characterTypeMapTemp[' ']] = 23;
  input[0][characterTypeMapTemp['\t']] = 23;
  initialisetooneNumber(input, 23, s + DELIMITER);
  input[23][characterTypeMapTemp[' ']] = 23;
  input[23][characterTypeMapTemp['\t']] = 23;

  input[0][characterTypeMapTemp['!']] = 25;
  input[25][characterTypeMapTemp['=']] = s + TK_NE;

  input[0][characterTypeMapTemp['#']] = 1;
  initialiseforLowerLetter(input, 1, 2);
  initialisetooneNumber(input, 2, s + TK_RUID);
  initialiseforLowerLetter(input, 2, 2);

  input[0][characterTypeMapTemp['_']] = 20;

  // STATE 20 TRANSITIONS
  initialiseforLetter(input, 20, 21);

  // TK_FUNID
  initialisetooneNumber(input, 21, s + TK_FUNID);
  initialiseforLetter(input, 21, 21);
  initialiseforDigit(input, 21, 22);
  initialisetooneNumber(input, 22, s + TK_FUNID);
  initialiseforDigit(input, 22, 22);

  // NUMBERS TK_NUM & TK_RNUM
  initialiseforDigit(input, 0, 3);
  initialisetooneNumber(input, 3, s + TK_NUM1);
  initialiseforDigit(input, 3, 3);
  input[3][characterTypeMapTemp['.']] = 4;
  initialisetooneNumber(input, 4, s + TK_NUM2);
  initialiseforDigit(input, 4, 5);
  initialiseforDigit(input, 5, 6);

  initialisetooneNumber(input, 6, s + TK_RNUM2);
  input[6][characterTypeMapTemp['E']] = 7;

  initialiseforDigit(input, 7, 8);
  input[7][characterTypeMapTemp['-']] = 9;
  input[7][characterTypeMapTemp['+']] = 9;

  initialiseforDigit(input, 9, 8);
  initialiseforDigit(input, 8, s + TK_RNUM1);

  // COMPARATIVE OPERATORS
  input[0][characterTypeMapTemp['>']] = 26;
  input[0][characterTypeMapTemp['<']] = 27;
  initialisetooneNumber(input, 26, s + TK_GT);
  input[26][characterTypeMapTemp['=']] = s + TK_GE;
  initialisetooneNumber(input, 27, s + TK_LT1);
  input[27][characterTypeMapTemp['=']] = s + TK_LE;
  input[27][characterTypeMapTemp['-']] = 28;
  initialisetooneNumber(input, 28, s + TK_LT2);
  input[28][characterTypeMapTemp['-']] = 29;
  input[29][characterTypeMapTemp['-']] = s + TK_ASSIGNOP;

  // LOG. OPERATORS
  input[0][characterTypeMapTemp['@']] = 17;
  input[0][characterTypeMapTemp['&']] = 15;
  input[15][characterTypeMapTemp['&']] = 16;
  input[16][characterTypeMapTemp['&']] = s + TK_AND;
  input[17][characterTypeMapTemp['@']] = 18;
  input[18][characterTypeMapTemp['@']] = s + TK_OR;
  input[0][characterTypeMapTemp['=']] = 19;
  input[19][characterTypeMapTemp['=']] = s + TK_EQ;

  // TK_ID TK_FIELDID
  input[0][characterTypeMapTemp['a']] = 14;
  initialiseforIdLetter(input, 0, 10);

  initialisetooneNumber(input, 10, s + TK_FIELDID);
  initialiseforLowerLetter(input, 10, 14);
  initialisetooneNumber(input, 14, s + TK_FIELDID);
  initialiseforLowerLetter(input, 14, 14);

  initialiseforIdDigit(input, 10, 11);

  initialisetooneNumber(input, 11, s + TK_ID);
  initialiseforIdLetter(input, 11, 12);
  initialiseforIdDigit(input, 11, 13);
  initialisetooneNumber(input, 13, s + TK_ID);
  initialiseforIdDigit(input, 13, 13);
  initialisetooneNumber(input, 12, s + TK_ID);
  initialiseforIdLetter(input, 12, 12);
  initialiseforIdDigit(input, 12, 13);

  input[26][characterTypeMapTemp['=']] = s + TK_GE;

  input[0][characterTypeMapTemp['%']] = 24;
  initialisetooneNumber(input, 24, 24);
  input[24][characterTypeMapTemp['\n']] = s + TK_COMMENT;
  input[24][CT_EOF] = s + TK_COMMENT;

  input[19][characterTypeMapTemp['=']] = s + TK_EQ;
  for (int row = 0; row < NUM_STATES; row++) {

    int j = 0;
    while (
        j + INPUT_SECOND_DIMENSION <
        STATE_ARRAY_SIZE_TEMP_GEN) { // if there is a collision then we make the
                                     // flag 1 and increment the value of j
      // then we break out of that checker loop
      int flag = 0;
      for (int k = 0; k < INPUT_SECOND_DIMENSION; k++) {
        // checker loop
        if (nextStateTemp[j + k] != -1 && input[row][k] != -1) {
          flag = 1;
          j++;
          break;
        }
      }
      if (flag == 0) {
        for (int k = 0; k < INPUT_SECOND_DIMENSION; k++) {
          // write into the ans array
          if (input[row][k] != -1) {
            nextStateTemp[j + k] = input[row][k];
            checkStateTemp[j + k] = row;
          }
        }
        break;
      }
    }
    offsetArrayTemp[row] = j;
  }
  int actualSizeUsed = sizeof(nextStateTemp) / sizeof(nextStateTemp[0]) - 1;
  // get used size of next and check arrays
  while (actualSizeUsed >= 0) {
    if (checkStateTemp[actualSizeUsed] == -1 &&
        nextStateTemp[actualSizeUsed] == -1) {
      actualSizeUsed -= 1;
    } else {
      break;
    }
  }
  actualSizeUsed += 1;
  printf("Actual size used: %d\n", actualSizeUsed);
  FILE *file = fopen(outputFileName, "w");
  fprintf(file, "#define STATE_ARRAY_SIZE %d\n", actualSizeUsed);
  fclose(file);
  printArray("nextState", nextStateTemp, actualSizeUsed, "int",
             "STATE_ARRAY_SIZE");
  printArray("checkState", checkStateTemp, actualSizeUsed, "int",
             "STATE_ARRAY_SIZE");
  printArray("defaultArray", defaultArrayTemp,
             sizeof(defaultArrayTemp) / sizeof(defaultArrayTemp[0]), "int",
             "NUM_STATES");
  printArray("offset", offsetArrayTemp,
             sizeof(offsetArrayTemp) / sizeof(offsetArrayTemp[0]), "int",
             "NUM_STATES");
  file = fopen(outputFileName, "a");
  fprintf(file, "CharacterType characterTypeMap [%lu] = {",
          sizeof(characterTypeMapTemp) / sizeof(CharacterType));
  for (int i = 0; i < sizeof(characterTypeMapTemp) / sizeof(CharacterType);
       ++i) {
    fprintf(file, "%d", characterTypeMapTemp[i]);
    if (i < sizeof(characterTypeMapTemp) / sizeof(CharacterType) - 1) {
      fprintf(file, ",");
    }
  }
  fprintf(file, "};\n");

  return 0;
}

void printCurrentTime() {
  // Get the current time
  time_t currentTime;
  time(&currentTime);

  // Convert the current time to a string
  char *timeString = ctime(&currentTime);

  // Print the current time
  printf("Current Time: %s", timeString);
}

int main() {
  printCurrentTime();
  reinitialiseArrays();
  return 0;
}
