// 4 ARRAYS: DEFAULT, BASE, CHECK, NEXT
#include "lexical.h"
#define NUM_STATES 40



struct TransitionTableEntry{
    int checkState;
    int nextState;
} TransitionTableEntry;


struct TransitionTable{
    //STORE BASE ADRESS POINTING TO NEXT & CHECK
    int BaseEntry[NUM_STATES];

    //STORE THE DEFAUT STATES FOR EACH STATE
    int DefaultStates[NUM_STATES];

    //STORE THE NEXT

} TransitionTable;

