// // 4 ARRAYS: DEFAULT, BASE, CHECK, NEXT
#include "lexical.h"

// copied portion from
#define STATE_ARRAY_SIZE 87
int nextState[STATE_ARRAY_SIZE] = {14, 3, 10, 13, 7, 3, 23, 1, 26, 27, 19, 61, 62, 63, 64, 13, 12, 69, 70, 71, 72, 15, 2, 16, 2, 24, 17, 68, 67, 65, 66, 25, 75, 20, 2, 3, 2, 5, 6, 3, 8, 5, 6, 87, 8, 8, 73, 87, 4, 8, 14, 11, 14, 13, 12, 18, 9, 9, 14, 74, 14, 79, 21, 23, 21, 21, 21, 21, 22, 21, 21, 21, 22, 22, 32, 82, 81, 22, 78, 29, 31, 30, -1, -1, -1, -1, 28};
int checkState[STATE_ARRAY_SIZE] = {0, 0, 0, 13, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 11, 0, 0, 0, 0, 0, 1, 15, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 2, 4, 5, 3, 7, 4, 5, 8, 7, 9, 16, 8, 3, 9, 10, 10, 10, 12, 12, 17, 7, 7, 14, 18, 14, 19, 20, 23, 20, 20, 20, 21, 21, 21, 21, 21, 21, 22, 24, 25, 26, 22, 27, 28, 29, 30, -1, -1, -1, -1, 27};
int defaultArray[NUM_STATES] = {-1, -1, 90, 85, 86, -1, 88, -1, -1, -1, 83, 84, 84, -1, 83, -1, -1, -1, -1, -1, -1, 89, 89, 91, 24, -1, 80, 76, 77, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int offset[NUM_STATES] = {0, 22, 34, 34, 36, 37, 0, 39, 42, 44, 50, 14, 52, 2, 58, 2, 25, 29, 33, 51, 62, 67, 72, 57, 68, 65, 66, 68, 61, 62, 75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
CharacterType characterTypeMap[128] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 31, 7, 7, 7, 25, 21, 7, 29, 30, 19, 17, 11, 18, 14, 20, 5, 5, 1, 1, 1, 1, 1, 1, 5, 5, 13, 12, 9, 10, 8, 7, 26, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 27, 7, 28, 7, 33, 7, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 22, 7, 32, 7};
// copied till

const char *CharacterTypeToString(enum CharacterType type)
{
    switch (type)
    {
    case CT_UNDERSCORE:
        return "CT_UNDERSCORE";
    case CT_INVALID:
        return "CT_INVALID";
    case CT_LETTER_UPPER:
        return "CT_LETTER_UPPER";
    case CT_LETTER_LOWER_EXCEPT_ID_LETTER:
        return "CT_LETTER_LOWER_EXCEPT_ID_LETTER";
    case CT_DIGIT_EXCEPT_ID_DIGIT:
        return "CT_DIGIT_EXCEPT_ID_DIGIT";
    case CT_DELIMITER:
        return "CT_DELIMITER";
    case CT_GREATER:
        return "CT_GREATER";
    case CT_LESSER:
        return "CT_LESSER";
    case CT_EQUAL:
        return "CT_EQUAL";
    case CT_COMMA:
        return "CT_COMMA";
    case CT_SEM:
        return "CT_SEM";
    case CT_COLON:
        return "CT_COLON";
    case CT_DOT:
        return "CT_DOT";
    case CT_ROUND_OPEN:
        return "CT_ROUND_OPEN";
    case CT_ROUND_CLOSE:
        return "CT_ROUND_CLOSE";
    case CT_SQUARE_OPEN:
        return "CT_SQUARE_OPEN";
    case CT_SQUARE_CLOSE:
        return "CT_SQUARE_CLOSE";
    case CT_PLUS:
        return "CT_PLUS";
    case CT_MINUS:
        return "CT_MINUS";
    case CT_MUL:
        return "CT_MUL";
    case CT_DIV:
        return "CT_DIV";
    case CT_AND:
        return "CT_AND";
    case CT_OR:
        return "CT_OR";
    case CT_EXCLAMATION:
        return "CT_EXCLAMATION";
    case CT_TILDE:
        return "CT_TILDE";
    case CT_PERCENT:
        return "CT_PERCENT";
    case CT_AT_THE_RATE:
        return "CT_AT_THE_RATE";
    case CT_ID_LETTER:
        return "CT_ID_LETTER";
    case CT_ID_DIGIT:
        return "CT_ID_DIGIT";
    case CT_EXPONENT:
        return "CT_EXPONENT";
    // Handle more types if needed
    default:
        return "Unknown CharacterType";
    }
}

const char *TokenToString(enum Tokentype token)
{
    switch (token)
    {
    case CARRIAGE_RETURN:
        return "CARRIAGE_RETURN";
    case TK_ASSIGNOP:
        return "TK_ASSIGNOP";
    case TK_COMMENT:
        return "TK_COMMENT";
    case TK_WITH:
        return "TK_WITH";
    case TK_PARAMETERS:
        return "TK_PARAMETERS";
    case TK_END:
        return "TK_END";
    case TK_WHILE:
        return "TK_WHILE";
    case TK_UNION:
        return "TK_UNION";
    case TK_ENDUNION:
        return "TK_ENDUNION";
    case TK_DEFINETYPE:
        return "TK_DEFINETYPE";
    case TK_AS:
        return "TK_AS";
    case TK_TYPE:
        return "TK_TYPE";
    case TK_MAIN:
        return "TK_MAIN";
    case TK_GLOBAL:
        return "TK_GLOBAL";
    case TK_PARAMETER:
        return "TK_PARAMETER";
    case TK_LIST:
        return "TK_LIST";
    case TK_INPUT:
        return "TK_INPUT";
    case TK_OUTPUT:
        return "TK_OUTPUT";
    case TK_INT:
        return "TK_INT";
    case TK_REAL:
        return "TK_REAL";
    case TK_ENDWHILE:
        return "TK_ENDWHILE";
    case TK_IF:
        return "TK_IF";
    case TK_THEN:
        return "TK_THEN";
    case TK_ENDIF:
        return "TK_ENDIF";
    case TK_READ:
        return "TK_READ";
    case TK_WRITE:
        return "TK_WRITE";
    case TK_RETURN:
        return "TK_RETURN";
    case TK_CALL:
        return "TK_CALL";
    case TK_RECORD:
        return "TK_RECORD";
    case TK_ENDRECORD:
        return "TK_ENDRECORD";
    case TK_ELSE:
        return "TK_ELSE";
    case TK_COMMA:
        return "TK_COMMA";
    case TK_SEM:
        return "TK_SEM";
    case TK_COLON:
        return "TK_COLON";
    case TK_DOT:
        return "TK_DOT";
    case TK_OP:
        return "TK_OP";
    case TK_CL:
        return "TK_CL";
    case TK_SQR:
        return "TK_SQR";
    case TK_SQL:
        return "TK_SQL";
    case TK_PLUS:
        return "TK_PLUS";
    case TK_MINUS:
        return "TK_MINUS";
    case TK_MUL:
        return "TK_MUL";
    case TK_DIV:
        return "TK_DIV";
    case TK_AND:
        return "TK_AND";
    case TK_OR:
        return "TK_OR";
    case TK_NOT:
        return "TK_NOT";
    case TK_LT1:
        return "TK_LT1";
    case TK_LT2:
        return "TK_LT2";
    case TK_LE:
        return "TK_LE";
    case TK_EQ:
        return "TK_EQ";
    case TK_GT:
        return "TK_GT";
    case TK_GE:
        return "TK_GE";
    case TK_NE:
        return "TK_NE";
    case TK_FIELDID:
        return "TK_FIELDID";
    case TK_ID:
        return "TK_ID";
    case TK_NUM1:
        return "TK_NUM1";
    case TK_NUM2:
        return "TK_NUM2";
    case TK_RNUM1:
        return "TK_RNUM1";
    case TK_RNUM2:
        return "TK_RNUM2";
    case TK_FUNID:
        return "TK_FUNID";
    case TK_RUID:
        return "TK_RUID";
    case DELIMITER:
        return "DELIMITER";
    default:
        return "non accept state";
    }
}

typedef struct
{
    char input[40];         // Input string for the test case
    char stopreadingAt[40]; // Expected lexeme output (simplified for illustration)
    enum Tokentype t;       // Expected token type output (simplified for illustration)
} TestCase;

TestCase testCases[] = {
    {"_main@", "_main@", TK_FIELDID},
    {"@@@@", "@@@", TK_AND},
    {"_main", "_main", TK_FIELDID},
    // Add more test cases as needed
};

int getNextState(int currentState, char character)
{
    int actualOffset = offset[currentState] + characterTypeMap[(int)(character)];
    int state;
    if (actualOffset >= STATE_ARRAY_SIZE)
    {
        state = defaultArray[currentState];
        return state;
    }
    if (checkState[actualOffset] == currentState)
    {
        state = nextState[actualOffset];
    }
    else
    {
        state = defaultArray[currentState];
    }
    printf("state: %d, %s\n", state, TokenToString(state - 30));

    return state;
}
// // tester
// int main()
// {
//     int numberOfTestCases = sizeof(testCases) / sizeof(TestCase);

//     for (int testCaseIndex = 0; testCaseIndex < numberOfTestCases; ++testCaseIndex)
//     {
//         char *testString = testCases[testCaseIndex].input;
//         int currentState = 0;
//         int charIndex = 0;

//         printf("Test Case %d: \"%s\"\n", testCaseIndex + 1, testString);

//         while (currentState <= NUM_NON_ACCEPT_STATES && charIndex < strlen(testString))
//         {
//             currentState = getNextState(currentState, testString[charIndex]);
//             if (currentState < 0)
//             {
//                 printf("Error\n");
//             };           // Error state
//             charIndex++; // Move to the next character
//         }

//         testString[charIndex] = '\0';
//         if (strcmp(testString, testCases[testCaseIndex].stopreadingAt) == 0 && currentState > NUM_NON_ACCEPT_STATES)
//         {
//             printf("PASS: Reached expected state and lexeme\n");
//         }
//         else
//         {
//             printf("FAIL: Did not reach expected state or lexeme\n");
//             printf("%s", testCases[testCaseIndex].stopreadingAt);
//         }
//     }

//     return 0;
// }

// // struct TransitionTableEntry{
// //     int checkState;
// //     int nextState;
// // } TransitionTableEntry;

// // struct TransitionTable{
// //     //STORE BASE ADRESS POINTING TO NEXT & CHECK
// //     int BaseEntry[NUM_STATES];

// //     //STORE THE DEFAUT STATES FOR EACH STATE
// //     int DefaultStates[NUM_STATES];

// //     //STORE THE NEXT

// // } TransitionTable;
