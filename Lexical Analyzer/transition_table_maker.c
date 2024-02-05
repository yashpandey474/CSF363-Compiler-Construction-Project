#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"
#define INPUT_SECOND_DIMENSION 32 // number of character types
#define NUM_NON_ACCEPT_STATES 29
#define STATE_ARRAY_SIZE 505
#define NUM_STATES 88 // 59 accept states number of states including accept states
int nextState[STATE_ARRAY_SIZE] = {14, 3, 10, 13, -1, 3, 23, 20, 26, 27, 19, 60, 61, 62, 63, 77, 80, 68, 69, 70, 71, 15, 2, 16, 2, 24, 17, 67, 66, 64, 65, 25, 2, 86, 2, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 86, 83, 3, 83, 83, 83, 3, 83, 83, 83, 83, 83, 83, 83, 83, 4, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 5, 83, 83, 83, 5, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 83, 6, 72, 18, 73, 6, 84, 84, 84, 84, 7, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 84, 8, 55, 8, 29, 8, 55, 8, 21, -1, 21, 21, 21, -1, -1, -1, 30, 9, 9, 14, 11, 14, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 81, 82, 13, 12, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 13, 12, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 21, 22, 21, 21, 21, 22, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 22, 85, 85, 85, 22, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 87, 87, 87, 87, 87, 87, 23, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 24, 24, 24, 24, 24, 24, 31, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 79, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 76, 75, 75, 75, 75, 75, 75, 75, 28, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 29, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, 76, -1, -1};
int checkState[STATE_ARRAY_SIZE] = {0, 0, 0, 13, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 25, 0, 0, 0, 0, 0, 1, 15, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 16, 17, 18, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 8, 9, 29, 7, 8, 9, 20, -1, 20, 20, 20, -1, -1, -1, 29, 7, 7, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, -1, -1};
int defaultArray[NUM_STATES] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 79, 76, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int offset[NUM_STATES] = {0, 22, 32, 64, 96, 127, 133, 164, 165, 166, 183, 215, 247, 2, 0, 2, 108, 104, 105, 5, 172, 279, 311, 343, 375, 6, 407, 439, 471, 162, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
CharacterType characterTypeMap[128] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 31, 7, 7, 7, 25, 21, 7, 29, 30, 19, 17, 11, 18, 14, 20, 5, 5, 1, 1, 1, 1, 1, 1, 5, 5, 13, 12, 9, 10, 8, 7, 26, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 27, 7, 28, 7, 7, 7, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 22, 7, 32, 7};

void initialisetooneNumber(int **input, int rownumber, int val)
{
    for (int i = 0; i < INPUT_SECOND_DIMENSION; i++)
    {
        input[rownumber][i] = val;
    }
}
void initialiseforDigit(int **input, int rownumber, int nextState)
{
    input[rownumber][characterTypeMap['1']] = nextState;
    input[rownumber][characterTypeMap['2']] = nextState;
}
void initialiseforIdLetter(int **input, int rownumber, int nextState)
{
    input[rownumber][characterTypeMap['b']] = nextState;
}
void initialiseforLowerLetter(int **input, int rownumber, int nextState)
{
    initialiseforIdLetter(input, rownumber, nextState);
    input[rownumber][characterTypeMap['a']] = nextState;
}
void initialiseforLetter(int **input, int rownumber, int nextState)
{
    input[rownumber][characterTypeMap['A']] = nextState;
    input[rownumber][characterTypeMap['E']] = nextState;
    initialiseforLowerLetter(input, rownumber, nextState);
}
void initialiseforIdDigit(int **input, int rownumber, int nextState)
{
    input[rownumber][characterTypeMap['2']] = nextState;
}

void initializeCharacterTypeMap()
{
    for (int i = 0; i < 128; ++i)

    {
        if (i >= 'a' && i <= 'z')
        {
            if (i >= 'b' && i <= 'd')
            {
                characterTypeMap[i] = CT_ID_LETTER;
            }
            else
            {
                characterTypeMap[i] = CT_LETTER_LOWER_EXCEPT_ID_LETTER;
            }
        }
        else if (i >= 'A' && i <= 'Z')
        {
            if (i == 'E')
                characterTypeMap[i] = CT_EXPONENT;
            else
            {
                characterTypeMap[i] = CT_LETTER_UPPER;
            }
        }
        else if (i >= '0' && i <= '9')
        {
            if (i >= '2' && i <= '7')
            {
                characterTypeMap[i] = CT_ID_DIGIT;
            }
            else
            {
                characterTypeMap[i] = CT_DIGIT_EXCEPT_ID_DIGIT;
            }
        }
        else
        {
            switch (i)
            {
            case '\t':
            case ' ':
            case '\n':
                characterTypeMap[i] = CT_DELIMITER;
                break;
            case '>':
                characterTypeMap[i] = CT_GREATER;
                break;
            case '<':
                characterTypeMap[i] = CT_LESSER;
                break;
            case '=':
                characterTypeMap[i] = CT_EQUAL;
                break;
            case ',':
                characterTypeMap[i] = CT_COMMA;
                break;
            case ';':
                characterTypeMap[i] = CT_SEM;
                break;
            case ':':
                characterTypeMap[i] = CT_COLON;
                break;
            case '.':
                characterTypeMap[i] = CT_DOT;
                break;
            case '(':
                characterTypeMap[i] = CT_ROUND_OPEN;
                break;
            case ')':
                characterTypeMap[i] = CT_ROUND_CLOSE;
                break;
            case '[':
                characterTypeMap[i] = CT_SQUARE_OPEN;
                break;
            case ']':
                characterTypeMap[i] = CT_SQUARE_CLOSE;
                break;
            case '+':
                characterTypeMap[i] = CT_PLUS;
                break;
            case '-':
                characterTypeMap[i] = CT_MINUS;
                break;
            case '*':
                characterTypeMap[i] = CT_MUL;
                break;
            case '/':
                characterTypeMap[i] = CT_DIV;
                break;
            case '&':
                characterTypeMap[i] = CT_AND;
                break;
            case '|':
                characterTypeMap[i] = CT_OR;
                break;
            case '!':
                characterTypeMap[i] = CT_EXCLAMATION;
                break;
            case '~':
                characterTypeMap['~'] = CT_TILDE;
                break;
            case '%':
                characterTypeMap[i] = CT_PERCENT;
                break;
            case '@':
                characterTypeMap[i] = CT_AT_THE_RATE;
                break;
            // Any other character is considered invalid
            default:
                characterTypeMap[i] = CT_INVALID;
            }
        }
    }
}
int reinitialiseArrays()
{
    printf("Started with the execution !!!\n");
    initializeCharacterTypeMap();
    printf("Character type map \n");
    for (int i = 0; i < 128; ++i)
    {
        printf("%d,", characterTypeMap[i]);
    }
    // the length of ans and the number of columns of input are randomly chosen

    int s = NUM_NON_ACCEPT_STATES;

    for (int i = 0; i < NUM_STATES; i += 1)
    {
        offset[i] = 0;
        defaultArray[i] = -1;
    }

    // SET DEFAULT STATE FOR SIGMA - TO DESTINATION
    defaultArray[26] = s + TK_GE;
    defaultArray[27] = s + TK_LE;

    for (int i = 0; i < STATE_ARRAY_SIZE; i++)
    {
        nextState[i] = -1;
        checkState[i] = -1;
    }
    int **input = (int **)malloc(NUM_STATES * sizeof(int *));

    for (int i = 0; i < NUM_STATES; ++i)
    {
        input[i] = (int *)malloc(INPUT_SECOND_DIMENSION * sizeof(int));
    }

    for (int i = 0; i < NUM_STATES; i++)
    {
        for (int j = 0; j < INPUT_SECOND_DIMENSION; j++)
        {
            input[i][j] = -1;
        }
    }

    input[0][characterTypeMap['~']] = s + TK_NOT;
    input[0][characterTypeMap['/']] = s + TK_DIV;
    input[0][characterTypeMap['*']] = s + TK_MUL;
    input[0][characterTypeMap['-']] = s + TK_MINUS;
    input[0][characterTypeMap['+']] = s + TK_PLUS;
    input[0][characterTypeMap['.']] = s + TK_DOT;
    input[0][characterTypeMap[',']] = s + TK_COMMA;
    input[0][characterTypeMap[':']] = s + TK_COLON;
    input[0][characterTypeMap[';']] = s + TK_SEM;
    input[0][characterTypeMap[')']] = s + TK_CL;
    input[0][characterTypeMap['(']] = s + TK_OP;
    input[0][characterTypeMap['[']] = s + TK_SQL;
    input[0][characterTypeMap[']']] = s + TK_SQR;

    input[0][characterTypeMap['\n']] = s + CARRIAGE_RETURN;
    input[s + CARRIAGE_RETURN][characterTypeMap['\n']] = s + CARRIAGE_RETURN;

    input[0][characterTypeMap[' ']] = 23;
    input[0][characterTypeMap['\t']] = 23;
    initialisetooneNumber(input, 23, s + DELIMITER);
    input[23][characterTypeMap[' ']] = 23;
    input[23][characterTypeMap['\t']] = 23;

    input[0][characterTypeMap['!']] = 25;
    input[25][characterTypeMap['=']] = s + TK_NE;

    input[0][characterTypeMap['#']] = 1;
    initialiseforLowerLetter(input, 1, 2);
    initialisetooneNumber(input, 2, s + TK_RUID);
    initialiseforLowerLetter(input, 2, 2);

    input[0][characterTypeMap['_']] = 20;

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
    initialisetooneNumber(input, 3, s + TK_NUM);
    initialiseforDigit(input, 3, 3);
    input[3][characterTypeMap['.']] = 4;
    initialisetooneNumber(input, 4, s + TK_NUM);
    initialiseforDigit(input, 4, 5);
    initialiseforDigit(input, 5, 6);

    initialisetooneNumber(input, 6, s + TK_RNUM);
    input[6][characterTypeMap['E']] = 7;

    initialiseforDigit(input, 7, 8);
    input[7][characterTypeMap['-']] = 9;
    input[7][characterTypeMap['+']] = 9;

    initialiseforDigit(input, 9, 8);
    initialiseforDigit(input, 8, TK_RNUM);

    // COMPARATIVE OPERATORS
    input[0][characterTypeMap['>']] = 26;
    input[0][characterTypeMap['<']] = 27;
    initialisetooneNumber(input, 26, s + TK_GT);
    input[26][characterTypeMap['=']] = s + TK_GE;
    initialisetooneNumber(input, 27, s + TK_LT);
    input[27][characterTypeMap['=']] = s + TK_LE;
    input[27][characterTypeMap['-']] = 28;
    initialisetooneNumber(input, 28, s + TK_LE);
    input[28][characterTypeMap['-']] = 29;
    input[29][characterTypeMap['-']] = s + TK_ASSIGNOP;

    // LOG. OPERATORS
    input[0][characterTypeMap['@']] = 17;
    input[0][characterTypeMap['&']] = 15;
    input[15][characterTypeMap['&']] = 16;
    input[16][characterTypeMap['&']] = s + TK_AND;
    input[17][characterTypeMap['@']] = 18;
    input[18][characterTypeMap['@']] = s + TK_OR;
    input[0][characterTypeMap['=']] = 19;
    input[19][characterTypeMap['=']] = s + TK_EQ;

    // TK_ID TK_FIELDID
    input[0][characterTypeMap['a']] = 14;
    initialiseforIdLetter(input, 0, 10);

    initialisetooneNumber(input, 10, s + TK_FIELDID);
    initialiseforLowerLetter(input, 10, 14);

    initialiseforIdDigit(input, 10, 11);

    initialisetooneNumber(input, 11, s + TK_ID);
    initialiseforIdLetter(input, 11, 12);
    initialiseforIdDigit(input, 11, 13);
    initialiseforIdDigit(input, 13, 13);
    initialisetooneNumber(input, 12, s + TK_ID);
    initialiseforIdLetter(input, 12, 12);
    initialiseforIdDigit(input, 12, 13);

    input[26][characterTypeMap['=']] = s + TK_GE;

    input[0][characterTypeMap['%']] = 24;
    initialisetooneNumber(input, 24, 24);
    input[24][characterTypeMap['\n']] = s + TK_COMMENT;
    input[19][characterTypeMap['=']] = s + TK_EQ;
    // printf("\n\nINPUT ARRAY: \n");
    // for (int t = 0; t < NUM_STATES; t += 1)
    // {
    //     printf("\n\nState : %d\n", t);
    //     for (int j = 0; j < INPUT_SECOND_DIMENSION; j += 1)
    //     {
    //         printf("%d, ", input[t][j]);
    //     }
    //     printf("\n");
    // }

    // this is the loop to find the base(offset), next and check arrays
    for (int row = 0; row < NUM_STATES; row++)
    {

        // printf("%d ROW", row);

        // printf("Check State array:\n");
        // for (int i = 0; i < sizeof(checkState) / sizeof(checkState[0]); ++i)
        // {
        //     printf("%d ", checkState[i]);
        // }
        // printf("\n");

        int j = 0;
        while (j + INPUT_SECOND_DIMENSION < STATE_ARRAY_SIZE)
        { // if there is a collision then we make the flag 1 and increment the value of j
            // then we break out of that checker loop
            int flag = 0;
            for (int k = 0; k < INPUT_SECOND_DIMENSION; k++)
            {
                // checker loop
                if (nextState[j + k] != -1 && input[row][k] != -1)
                {
                    flag = 1;
                    j++;
                    break;
                }
            }
            if (flag == 0)
            {
                for (int k = 0; k < INPUT_SECOND_DIMENSION; k++)
                {
                    // write into the ans array
                    if (input[row][k] != -1)
                    {
                        nextState[j + k] = input[row][k];
                        checkState[j + k] = row;
                    }
                }
                break;
            }
        }
        offset[row] = j;
    }

    printf("\nOffset array:\n");
    for (int i = 0; i < sizeof(offset) / sizeof(offset[0]); ++i)
    {
        printf("%d,", offset[i]);
    }
    printf("\n\nNext State array:\n");
    for (int i = 0; i < sizeof(nextState) / sizeof(nextState[0]); ++i)
    {
        printf("%d,", nextState[i]);
    }
    printf("\n");

    printf("Check State array:\n");
    for (int i = 0; i < sizeof(checkState) / sizeof(checkState[0]); ++i)
    {
        printf("%d,", checkState[i]);
    }
    printf("\n");

    printf("Default array:\n");
    for (int i = 0; i < sizeof(defaultArray) / sizeof(defaultArray[0]); ++i)
    {
        printf("%d,", defaultArray[i]);
    }
    printf("\n");

    return 0;
}

int main()
{
    char testString[] = "_main@";
    printf("String: %s\n", testString);
    printf("Character type map: \n");
    for (int i = 0; i < strlen(testString); i++)
    {
        printf("%d : %d\n", testString[i], characterTypeMap[testString[i]]);
    }
    printf("\n");
    int startstate = 0;
    int i = 0;
    while (startstate < NUM_NON_ACCEPT_STATES && i < strlen(testString))
    {
        int actualOffset = offset[startstate] + characterTypeMap[testString[i]];
        if (checkState[actualOffset] == startstate)
        {
            startstate = nextState[actualOffset];
            printf("Going to %d \n", startstate);
            i++; // Move to the next character
        }
        else
        {
            printf("Error\n");
            break;
        }
    }
    return 0;
}
