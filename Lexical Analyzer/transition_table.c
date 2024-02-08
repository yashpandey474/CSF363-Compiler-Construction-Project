// // 4 ARRAYS: DEFAULT, BASE, CHECK, NEXT
#include "lexical.h"

#define STATE_ARRAY_SIZE 89
int nextState[STATE_ARRAY_SIZE] = {14, 3, 10, 13, 7, 3, 23, 20, 26, 27, 19, 60, 61, 62, 63, 13, 12, 68, 69, 70, 71, 15, 2, 16, 2, 24, 17, 67, 66, 64, 65, 25, 2, 3, 2, 5, 6, 3, 8, 5, 6, 55, 8, 8, 72, 55, 4, 8, 14, 11, 14, 13, 12, 18, 9, 9, 73, 77, 21, 23, 21, 21, 21, 21, 22, 21, 21, 21, 22, 22, 31, 80, 79, 22, 76, 29, 29, -1, -1, -1, -1, -1, 28, -1, -1, -1, -1, -1, 30};
int checkState[STATE_ARRAY_SIZE] = {0, 0, 0, 13, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 11, 0, 0, 0, 0, 0, 1, 15, 1, 0, 0, 0, 0, 0, 0, 0, 2, 3, 2, 4, 5, 3, 7, 4, 5, 8, 7, 9, 16, 8, 3, 9, 10, 10, 10, 12, 12, 17, 7, 7, 18, 19, 20, 23, 20, 20, 20, 21, 21, 21, 21, 21, 21, 22, 24, 25, 26, 22, 27, 28, 29, -1, -1, -1, -1, -1, 27, -1, -1, -1, -1, -1, 29};
int defaultArray[NUM_STATES] = {-1, -1, 86, 83, 83, -1, 84, -1, -1, -1, 81, 82, 82, -1, -1, -1, -1, -1, -1, -1, -1, 85, 85, 87, 24, -1, 78, 75, 76, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int offset[NUM_STATES] = {0, 22, 32, 32, 34, 35, 0, 37, 40, 42, 48, 14, 50, 2, 0, 2, 23, 27, 30, 47, 58, 63, 68, 53, 64, 61, 62, 64, 57, 70, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
CharacterType characterTypeMap[128] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 6, 31, 7, 7, 7, 25, 21, 7, 29, 30, 19, 17, 11, 18, 14, 20, 5, 5, 1, 1, 1, 1, 1, 1, 5, 5, 13, 12, 9, 10, 8, 7, 26, 3, 3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 27, 7, 28, 7, 7, 7, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 22, 7, 32, 7};

// tester
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
    while (startstate < NUM_NON_ACCEPT_STATES && i < strlen(testString) && startstate >= 0)
    {
        printf("Reading %c. Currently at %d. Going to -->", testString[i], startstate);

        int actualOffset = offset[startstate] + characterTypeMap[testString[i]];
        if (actualOffset >= STATE_ARRAY_SIZE)
        {
            printf("%d\n", defaultArray[21]);
            startstate = defaultArray[startstate];
            break;
        }
        if (checkState[actualOffset] == startstate)
        {
            startstate = nextState[actualOffset];
        }
        else
        {
            startstate = defaultArray[startstate];
        }
        printf("%d\n", startstate);
        i++; // Move to the next character
    }
    printf("Done");

    return 0;
}

// struct TransitionTableEntry{
//     int checkState;
//     int nextState;
// } TransitionTableEntry;

// struct TransitionTable{
//     //STORE BASE ADRESS POINTING TO NEXT & CHECK
//     int BaseEntry[NUM_STATES];

//     //STORE THE DEFAUT STATES FOR EACH STATE
//     int DefaultStates[NUM_STATES];

//     //STORE THE NEXT

// } TransitionTable;
