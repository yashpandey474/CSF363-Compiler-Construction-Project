// GROUP 01
// NAMES	                 BITS ID
// ------------------------------------------
// Yash Pandey	             2021A7PS0661P
// Achinthya Hebbar S	     2021A7PS1457P
// Ujjwal Aggarwal	         2021A7PS2427P
// Hardik Gupta 	         2021A7PS2421P
// Agrawal Vansh Anil        2021A7PS2215P

// // 4 ARRAYS: DEFAULT, BASE, CHECK, NEXT
#include "lexer.h"

// copied portion from
#define STATE_ARRAY_SIZE 89
int nextState[STATE_ARRAY_SIZE] = {14, 3, 10, 3, 7, 3, 23, 3, 26, 27, 19, 61, 62, 63, 64, 13, 4, 69, 70, 71, 72, 15, 2, 16, 2, 24, 17, 68, 67, 65, 66, 25, 75, 20, 30, 2, 1, 2, 5, 6, 8, 87, 5, 6, 8, 87, 8, 14, 11, 14, 8, 13, 12, 13, 12, 73, 9, 9, 14, 18, 14, 74, 79, 21, 23, 21, 21, 21, 21, 22, 21, 21, 21, 22, 22, 32, 32, 82, 22, 81, 78, 29, 31, 30, -1, -1, -1, -1, 28};
int checkState[STATE_ARRAY_SIZE] = {0, 0, 0, 3, 6, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 13, 3, 0, 0, 0, 0, 0, 1, 15, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 4, 5, 7, 8, 4, 5, 7, 8, 9, 10, 10, 10, 9, 11, 11, 12, 12, 16, 7, 7, 14, 17, 14, 18, 19, 20, 23, 20, 20, 20, 21, 21, 21, 21, 21, 21, 22, 24, 24, 25, 22, 26, 27, 28, 29, 30, -1, -1, -1, -1, 27};
int defaultArray[NUM_STATES] = {-1, -1, 90, 85, 86, -1, 88, -1, -1, -1, 83, 84, 84, 84, 83, -1, -1, -1, -1, -1, -1, 89, 89, 91, 24, -1, 80, 76, 77, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int offset[NUM_STATES] = {0, 22, 35, 2, 37, 38, 0, 39, 40, 45, 47, 50, 52, 14, 58, 2, 34, 33, 35, 52, 63, 68, 73, 58, 41, 67, 69, 70, 63, 64, 49, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
CharacterType characterTypeMap[129] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 34, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 31, 7, 36, 7, 25, 21, 7, 29, 30, 19, 17, 11, 18, 14, 20, 5, 5, 1, 1, 1, 1, 1, 1, 5, 5, 13, 12, 9, 10, 8, 7, 26, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 27, 7, 28, 7, 33, 7, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 22, 7, 32, 7, 35};

int getNextState(int currentState, int character)
{

    // printf("Reading character: %c of type %s\n", character, CharacterTypeToString(characterTypeMap[character]));
    int actualOffset = offset[currentState] + characterTypeMap[character];
    int state;
    if (actualOffset >= STATE_ARRAY_SIZE)
    {
        // printf("INVALID INPUT\n");
        state = defaultArray[currentState];
        return state;
    }
    if (checkState[actualOffset] == currentState)
    {
        // printf("VALID INPUT; NEXT RETURNED\n");
        state = nextState[actualOffset];
    }
    else
    {
        // printf("DEFAULT RETURNED\n");
        state = defaultArray[currentState];
    }
    // printf("state: %d, %s\n", state, TokenToString(state - 30));

    // printf("RETURNED STATE %d\n", state);
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
