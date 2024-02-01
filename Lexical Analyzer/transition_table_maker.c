#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"

void main()
{
    initializeCharacterTypeMap();
    // the length of ans and the number of columns of input are randomly chosen
    int nextState[200];
    int checkState[200];
    for (int i = 0; i < 200; i++)
    {
        nextState[i] = -1;
        checkState[i] = -1;
    }
    int input[40][128];
    int s;

    input[0][characterTypeMap['#']] = 1;
    input[0][characterTypeMap['_']] = 20;
    input[0][characterTypeMap['\n']] = s + CARRIAGE_RETURN;
    input[0][characterTypeMap['\t']] = 23;
    input[0][characterTypeMap[' ']] = 23;
    input[0][characterTypeMap['_']] = 20;
    input[0][characterTypeMap['%']] = 24;
    input[0][characterTypeMap['!']] = 25;
    input[0][characterTypeMap['>']] = 26;
    input[0][characterTypeMap['<']] = 27;
    input[0][characterTypeMap['=']] = 19;
    input[0][characterTypeMap['@']] = 17;
    input[0][characterTypeMap['&']] = 15;
    input[0][characterTypeMap['~']] = s + TK_NOT;
    input[0][characterTypeMap['/']] = s + TK_DIV;
    input[0][characterTypeMap['*']] = s + TK_MUL;
    input[0][characterTypeMap['-']] = s + TK_MINUS;
    input[0][characterTypeMap['+']] = s + TK_PLUS;
    input[0][characterTypeMap[')']] = s + TK_CL;
    input[0][characterTypeMap['(']] = s + TK_OP;
    input[0][characterTypeMap['.']] = s + TK_DOT;
    input[0][characterTypeMap[',']] = s + TK_COMMA;
    input[0][characterTypeMap[':']] = s + TK_COLON;
    input[0][characterTypeMap[';']] = s + TK_SEM;
    input[0][characterTypeMap['[']] = s + TK_SQR;
    input[0][characterTypeMap[']']] = s + TK_SQR;
    input[0][characterTypeMap['b']] = 10;
    input[0][characterTypeMap['a']] = 14;
    input[0][characterTypeMap['1']] = 3;
    input[0][characterTypeMap['2']] = 3;

    for (int row = 0; row < len(input); row++)
    {
        int j = 0;
        while (j + len(input[0]) < len(nextState))
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
                    if (n[j + k] != -1)
                    {
                        checkState[j + k] = input[row][k];
                    }
                }
            }
        }
    }
}
