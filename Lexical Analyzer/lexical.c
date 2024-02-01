#include "lexical.h"
#include <stdio.h>

CharacterType characterTypeMap[128] = {6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 30, 6, 6, 6, 24, 20, 6, 28, 29, 18, 16, 10, 17, 13, 19, 4, 4, 1, 1, 1, 1, 1, 1, 4, 4, 12, 11, 8, 9, 7, 6, 25, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 26, 6, 27, 6, 6, 6, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 21, 6, 31, 6};
// void initializeCharacterTypeMap()
// {
//     for (int i = 0; i < 128; ++i)
//     {
//         if (i >= 'a' && i <= 'z')
//         {
//             if (i >= 'b' && i <= 'd')
//             {
//                 characterTypeMap[i] = CT_ID_LETTER;
//             }
//             else
//             {
//                 characterTypeMap[i] = CT_LETTER_LOWER_EXCEPT_ID_LETTER;
//             }
//         }
//         else if (i >= 'A' && i <= 'Z')
//         {
//             characterTypeMap[i] = CT_LETTER_UPPER;
//         }
//         else if (i >= '0' && i <= '9')
//         {
//             if (i >= '2' && i <= '7')
//             {
//                 characterTypeMap[i] = CT_ID_DIGIT;
//             }
//             else
//             {
//                 characterTypeMap[i] = CT_DIGIT_EXCEPT_ID_DIGIT;
//             }
//         }
//         else
//         {
//             switch (i)
//             {
//             case '\t':
//             case ' ':
//             case '\n':
//                 characterTypeMap[i] = CT_DELIMITER;
//                 break;
//             case '>':
//                 characterTypeMap[i] = CT_GREATER;
//                 break;
//             case '<':
//                 characterTypeMap[i] = CT_LESSER;
//                 break;
//             case '=':
//                 characterTypeMap[i] = CT_EQUAL;
//                 break;
//             case ',':
//                 characterTypeMap[i] = CT_COMMA;
//                 break;
//             case ';':
//                 characterTypeMap[i] = CT_SEM;
//                 break;
//             case ':':
//                 characterTypeMap[i] = CT_COLON;
//                 break;
//             case '.':
//                 characterTypeMap[i] = CT_DOT;
//                 break;
//             case '(':
//                 characterTypeMap[i] = CT_ROUND_OPEN;
//                 break;
//             case ')':
//                 characterTypeMap[i] = CT_ROUND_CLOSE;
//                 break;
//             case '[':
//                 characterTypeMap[i] = CT_SQUARE_OPEN;
//                 break;
//             case ']':
//                 characterTypeMap[i] = CT_SQUARE_CLOSE;
//                 break;
//             case '+':
//                 characterTypeMap[i] = CT_PLUS;
//                 break;
//             case '-':
//                 characterTypeMap[i] = CT_MINUS;
//                 break;
//             case '*':
//                 characterTypeMap[i] = CT_MUL;
//                 break;
//             case '/':
//                 characterTypeMap[i] = CT_DIV;
//                 break;
//             case '&':
//                 characterTypeMap[i] = CT_AND;
//                 break;
//             case '|':
//                 characterTypeMap[i] = CT_OR;
//                 break;
//             case '!':
//                 characterTypeMap[i] = CT_EXCLAMATION;
//                 break;
//             case '~':
//                 characterTypeMap[i] = CT_TILDE;
//                 break;
//             case '%':
//                 characterTypeMap[i] = CT_PERCENT;
//                 break;
//             case '@':
//                 characterTypeMap[i] = CT_AT_THE_RATE;
//                 break;
//             // Any other character is considered invalid
//             default:
//                 characterTypeMap[i] = CT_INVALID;
//             }
//         }
//     }
// }

void main()
{
    initializeCharacterTypeMap();
    for (int i = 0; i < 128; ++i)
    {
        printf("%d,", characterTypeMap[i]);
    }
}