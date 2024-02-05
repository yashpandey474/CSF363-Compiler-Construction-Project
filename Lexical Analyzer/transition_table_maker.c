#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"

#define INPUT_FIRST_DIMENSION 60
#define INPUT_SECOND_DIMENSION 128
#define NUM_STATES 40
#define STATE_ARRAY_SIZE 2000


void initialisetooneNumber(int **input, int rownumber, int val)
{
    for (int i = 0; i < 128; i++)
    {
        input[rownumber][i] = val;
    }
}
void initialiseforDigit(int **input, int rownumber, int nextState)
{
    input[rownumber][characterTypeMap['1']]=nextState;
    input[rownumber][characterTypeMap['2']]=nextState;
}
void initialiseforIdLetter(int **input, int rownumber, int nextState){
    input[rownumber][characterTypeMap['b']] = nextState;
}
void initialiseforLowerLetter(int **input, int rownumber, int nextState){
    initialiseforIdLetter(input,rownumber,nextState);
    input[rownumber][characterTypeMap['a']] = nextState;
}
void initialiseforLetter(int **input, int rownumber, int nextState)
{
    input[rownumber][characterTypeMap['A']]=nextState;
    input[rownumber][characterTypeMap['E']]=nextState;
    initialiseforLowerLetter(input,rownumber,nextState);
}
void initialiseforIdDigit(int** input, int rownumber, int nextState){
    input[rownumber][characterTypeMap['2']] = nextState;
}

CharacterType characterTypeMap[128] = {6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 30, 6, 6, 6, 24, 20, 6, 28, 29, 18, 16, 10, 17, 13, 19, 4, 4, 1, 1, 1, 1, 1, 1, 4, 4, 12, 11, 8, 9, 7, 6, 25, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
 3, 3, 3, 3, 3, 3, 26, 6, 27, 6, 6, 6, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 21, 6, 31, 6};
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
int main()
{
    initializeCharacterTypeMap();

    for (int i = 0; i < 128; ++i)
    {
        printf("%d,", characterTypeMap[i]);
    }
    // the length of ans and the number of columns of input are randomly chosen
    int nextState[2000];
    int checkState[2000];
    int defaultArray[NUM_STATES];
    int offset[NUM_STATES];

    for (int i = 0; i<NUM_STATES; i += 1){
        offset[i] = 0;
        defaultArray[i] = -1;
    }

    //SET DEFAULT STATE FOR SIGMA - TO DESTINATION
    defaultArray[26] = s + TK_GE;
    defaultArray[27] = s + TK_LE;

    for (int i = 0; i < 2000; i++)
    {
        nextState[i] = -1;
        checkState[i] = -1;
    }
    
    int **input = (int **)malloc(INPUT_FIRST_DIMENSION * sizeof(int *));
    for (int i = 0; i < INPUT_FIRST_DIMENSION; ++i)
    {
        input[i] = (int *)malloc(INPUT_SECOND_DIMENSION * sizeof(int));
    }

    printf("\n\nINPUT FIRST ROW\n\n");
    for (int i = 0; i <INPUT_FIRST_DIMENSION; i ++){
        for (int j = 0; j<INPUT_SECOND_DIMENSION; j ++){
               input[i][j] = -1;
        }
    }
    printf("\n\nINPUT FIRST ROW\n\n");
    int s=40;

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
    input[s+CARRIAGE_RETURN][characterTypeMap['\n']] = s + CARRIAGE_RETURN;
    
    input[0][characterTypeMap[' ']] = 23;
    input[0][characterTypeMap['\t']] = 23;
    initialisetooneNumber(input, 23, s+DELIMITER);
    input[23][characterTypeMap[' ']] = 23;
    input[23][characterTypeMap['\t']] = 23;

    printf("FIRST SET DONE");

    input[0][characterTypeMap['!']] = 25;
    input[25][characterTypeMap['=']] = s + TK_NE;

    

    input[0][characterTypeMap['#']] = 1;
    initialiseforLowerLetter(input, 1, 2);
    initialisetooneNumber(input, 2, s + TK_RUID);
    initialiseforLowerLetter(input, 2, 2);


    input[0][characterTypeMap['_']] = 20;
    
    //STATE 20 TRANSITIONS
    initialiseforLetter(input,20,21);         

    //TK_FUNID
    initialisetooneNumber(input, 21, s + TK_FUNID);
    initialiseforLetter(input,21,21);
    initialiseforDigit(input,21,22);
    initialisetooneNumber(input, 22, s + TK_FUNID);
    initialiseforDigit(input,22,22);

    printf("SECOND SET DONE");

    //NUMBERS TK_NUM & TK_RNUM
    initialiseforDigit(input, 0, 3);
    initialisetooneNumber(input, 3, s+TK_NUM);
    initialiseforDigit(input, 3, 3);
    input[3][characterTypeMap['.']] = 4;
    initialisetooneNumber(input, 4, s+TK_NUM);
    initialiseforDigit(input, 4, 5);
    initialiseforDigit(input, 5, 6);

    initialisetooneNumber(input, 6, s+TK_RNUM);
    input[6][characterTypeMap['E']] = 7;

    initialiseforDigit(input, 7, 8);
    input[7][characterTypeMap['-']] = 9; 
    input[7][characterTypeMap['+']] = 9; 

    initialiseforDigit(input, 9, 8);
    initialiseforDigit(input, 8, TK_RNUM);

    printf("THIRD SET DONE");
    //COMPARATIVE OPERATORS
    input[0][characterTypeMap['>']] = 26;
    input[0][characterTypeMap['<']] = 27;
    initialisetooneNumber(input, 26, s+TK_GT);
    input[26][characterTypeMap['=']] = s+TK_GE; 
    initialisetooneNumber(input, 27, s+TK_LT);
    input[27][characterTypeMap['=']] = s+TK_LE; 
    input[27][characterTypeMap['-']] = 28;
    initialisetooneNumber(input, 28, s+TK_LE);
    input[28][characterTypeMap['-']] = 29;
    input[29][characterTypeMap['-']] = s+TK_ASSIGNOP;


    //LOG. OPERATORS
    input[0][characterTypeMap['@']] = 17;
    input[0][characterTypeMap['&']] = 15;
    input[15][characterTypeMap['&']] = 16;
    input[16][characterTypeMap['&']] = s+TK_AND;
    input[17][characterTypeMap['@']] = 18;
    input[18][characterTypeMap['@']] = s + TK_OR;
    input[0][characterTypeMap['=']] = 19;
    input[19][characterTypeMap['=']] = s+TK_EQ;

    printf("FOURTH SET DONE");

    //TK_ID TK_FIELDID
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
    initialiseforIdDigit(input, 12,  13);



    input[26][characterTypeMap['=']] = s+TK_GE;
    

    input[0][characterTypeMap['%']] = 24;
    initialisetooneNumber(input, 24, 24);
    input[24][characterTypeMap['\n']] = s + TK_COMMENT;
    input[19][characterTypeMap['=']] = s + TK_EQ;

    //change len to size everywhere



    //this is the loop to find the base(offset), next and check arrays
    for (int row = 0; row < INPUT_FIRST_DIMENSION; row++)
    {

        // printf("%d ROW", row);

        // printf("Check State array:\n");
        // for (int i = 0; i < sizeof(checkState) / sizeof(checkState[0]); ++i)
        // {
        //     printf("%d ", checkState[i]);
        // }
        // printf("\n");

        int j = 0;
        while (j + INPUT_SECOND_DIMENSION < sizeof(nextState) / sizeof(nextState[0]))
        { // if there is a collision then we make the flag 1 and increment the value of j
            // then we break out of that checker loop
            int flag = 0;
            for (int k = 0; k < 128; k++)
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
                for (int k = 0; k < 128; k++)
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
        offset[row]=j;
    }

    printf("\n\nINPUT ARRAY: \n");
    for (int t = 0; t < INPUT_FIRST_DIMENSION; t += 1){
        for (int j  = 0; j < INPUT_SECOND_DIMENSION; j += 1){
            printf("%d, ", input[t][j]);
        }
        printf("\n");
    }
    printf("Offset array:\n");
    for (int i = 0; i < sizeof(offset) / sizeof(offset[0]); ++i)
    {
        printf("%d ", offset[i]);
    }
    printf("\n");

    printf("Next State array:\n");
    for (int i = 0; i < sizeof(nextState) / sizeof(nextState[0]); ++i)
    {
        printf("%d ", nextState[i]);
    }
    printf("\n");

    printf("Check State array:\n");
    for (int i = 0; i < sizeof(checkState) / sizeof(checkState[0]); ++i)
    {
        printf("%d ", checkState[i]);
    }
    printf("\n");

    return 0;
}
