
// MAKE AN ENUM FOR NONTERMINALS#

#include "syntactical.h"

#include <stdbool.h>

void printParsingTable(struct ParsingTable *pt)
{
    if (pt == NULL)
    {
        printf("ParsingTable pointer is NULL\n");
        return;
    }
    for (int i = 0; i < NUM_NON_TERMINALS; i++)
    {
        for (int j = 0; j < NUM_TERMINALS; j++)
        {
            printf("Cell[%s][%s]:", NonTerminalToString(i), TokenToString(j));
            if (pt->table[i][j] != NULL)
            {
                struct Variable* arr = pt->table[i][j];
                
                for(int var=0;var < 9; var += 1){
                    if(arr[var].flag == 0){
                        printf("%s ", TokenToString(arr[var].val));
                    }
                    else
                    {
                        printf("%s ", NonTerminalToString(arr[var].val));
                    }
                }

                printf("\n");

            }
            else
            {
                printf("ERROR\n");
            }
        }
    }
}

void insert(enum NonTerminals nt, enum Tokentype terminal, struct Variable *rule, struct ParsingTable *PT)
{
    PT->table[(int)(nt)][(int)terminal] = rule;
}

void populate_parsing_table(struct ParsingTable *PT, struct GrammarRule *productions, struct Sets **sets_for_all)
{
    for (int nt = 0; nt < NUM_NON_TERMINALS; nt++)
    {
        struct GrammarRule a = productions[nt];
        for (int i = 0; i < a.numProductions; i++)
        {
            
            struct Variable *rule = a.rules[i];
            
            if (rule[0].val == TK_EPS)
            {
                struct Node *current = sets_for_all[nt]->followSets->linkedList->head;

                while (current != NULL)
                {
    
                    insert(nt, current->data.val, rule, PT);
                    current = current->next;
                }
            }
            else
            {
                int set_index = rule[0].val;
                if (rule[0].flag == 0)
                {
                    set_index += NUM_NON_TERMINALS;
                }

                struct Node *current = sets_for_all[set_index]->firstSets->linkedList->head;
                while (current != NULL)
                {
                    printf("INSERT CALLED");

                    insert(nt, current->data.val, rule, PT);
                    current = current->next;
                }
            }
        }
    }
    return;
}

// Functions
struct Node *createNode(struct Variable data)
{
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}


void insertAtBeginning(struct LinkedListArray *linkedList, int newData, int flag)
{

    struct Variable var;
    var.val = newData;
    var.flag = flag;
    // Create a new node
    struct Node *newNode = createNode(var);

    // Set the next pointer of the new node to point to the current head
    newNode->next = linkedList->head;

    // Update the head to point to the new node
    linkedList->head = newNode;

    // INCREASE LENGTH
    linkedList->length += 1;
}

bool isDefault(struct Variable var)
{
    if (var.flag == 0 && var.val == 0)
    {
        return true;
    }
    return false;
}


bool appendVarToSet(struct LinkedListSet *set, struct Variable element)
{
    if (element.val == CARRIAGE_RETURN){
        return false;
    }

    if (set->booleanArr[element.val] == true)
    {
        // Element already exists in the set
        // printf("ELE ALREADY EXISTS\n");
        return false;
    }

    //INSERT INTO LINKED LIST
    insertAtBeginning(set->linkedList, element.val, element.flag);

    //SET IN BOOLEAN ARRAY
    set->booleanArr[element.val] = true;
    return true;
}

bool containsEPS(struct LinkedListSet *set)
{
    Tokentype value = TK_EPS;
    if (set->booleanArr[TK_EPS] == true)
    {
        // printf("CONTANS EPS??");
        return true;
    }
    return false;
}

bool appendSetToSet(struct LinkedListSet *destinationSet, struct LinkedListSet *sourceSet)
{
    bool changed = false;
    for (int i = 0; i < NUM_TERMINALS; ++i)
    {

        if ((i != TK_EPS) && (destinationSet->booleanArr[i] == false) && (sourceSet->booleanArr[i] == true)){
            changed = true;

            //INSERT INTO LINKED LIST
            insertAtBeginning(destinationSet->linkedList, i, 0);

            //SET IN BOOLEAN ARRAY
            destinationSet->booleanArr[i] = true;   
        }
        
    }
    return changed;
}

void printSetWithIndex(struct LinkedListSet *set, size_t setIndex, int flag, int firstorfollow)
{
    char str[10]; 
    if (firstorfollow) {
        strcpy(str, "Follow");
    } else {
        strcpy(str, "First");
    }

    if (flag == 1)
    {
        printf("%s Set %s: ", str, NonTerminalToString((enum NonTerminals)setIndex));
        for (size_t i = 0; i < NUM_TERMINALS; ++i)
        {
            // Assuming elements are integers for simplicity
            if (set->booleanArr[i] == true)
            {
                printf("%s, ", TokenToString((enum Tokentype)i));
            }
        }
        printf("\n\n");
    }
    else
    {
        printf("Set %s: ", TokenToString((enum Tokentype)setIndex));
        for (size_t i = 0; i < NUM_TERMINALS; ++i)
        {
            // Assuming elements are integers for simplicity
            if (set->booleanArr[i])
            {
                printf("%s, ", TokenToString((enum Tokentype)i));
            }
        }
        printf("\n\n");
    }
}

struct LinkedListSet* initialiseLLSet(){
    struct LinkedListSet* set = (struct LinkedListSet*)malloc (sizeof(struct LinkedListSet));
    memset(set->booleanArr, false, sizeof(set->booleanArr));

    set->linkedList =(struct LinkedListArray*) malloc(sizeof(struct LinkedListArray));
    set->linkedList->head = NULL;
    set->linkedList->length = 0;

    return set;
}

struct Sets *initializeSets()
{
    struct Sets *newSets = (struct Sets*) malloc(sizeof(struct Sets));

    // Initialize firstSets
    newSets->firstSets = initialiseLLSet();

    // Initialize followSets
    newSets->followSets = initialiseLLSet();

    return newSets;
}

bool isEmpty(struct LinkedListSet *destinationSet)
{
    //DIRECTLY CHECK LINKED LIST INSTEAD OF ITERATING THROUGH BOOLEAN ARRAY
    if (destinationSet->linkedList->length == 0){
        return true;
    }

    return false;
}

int recomputeSetIndex(struct Variable var)
{
    int set_index = var.val;
    if (var.flag == 0)
    {
        set_index += NUM_NON_TERMINALS;
    }
    return set_index;
}

void computeFollowSet(struct Sets **sets_for_all, struct GrammarRule *productions)
{

    // ADD TK_EOF [END OF INPUT MARKER] to follow set of NT_PROGRAM (start symbol)
    struct Variable eof; eof.val = TK_EOF; eof.flag = 0;
    appendVarToSet(sets_for_all[NT_PROGRAM]->followSets, eof);

    // WHILE THERE IS A CHANGE IN SOMEONE'S FOLLOW SET
    bool changed = true;
    bool res = false;
    while (changed == true)
    {
        changed = false;

        for (int nonTerminal = 0; nonTerminal < NUM_NON_TERMINALS; nonTerminal += 1)
        {
            int numberOfRules = productions[nonTerminal].numProductions;
            for (int j = 0; j < numberOfRules; j++)
            {
                // GET THE PRODUCTION
                struct Variable *production = productions[nonTerminal].rules[j];

                // GET THE LENGTH OF THE PRODUCTION
                int length = sizeof(productions[nonTerminal].rules[j]) / sizeof(productions[nonTerminal].rules[j][0]);
                for (int index1 = 0; index1 < length; index1++)
                {
                    // IF IT IS A TERMINAL: NO FOLLOW SET: SKIP
                    if (production[index1].flag == 0)
                    {
                        continue;
                    }

                    // If A -> αBβ, then Follow(B) = Follow(B) ∪ (First(β) - {ε})
                    int index2 = index1 + 1;
                    for (index2 = index1 + 1; index2 < length; index2++)
                    {
                        // printf("%d %d %d\n", nonTerminal, index1, index2);
                        // TERMINAL: INSTEAD OF ITERATING THROUGH WHOLE SET; ADD JUST THE TERMINAL
                        if (production[index2].flag == 0)
                        {
                            res = appendVarToSet(sets_for_all[production[index1].val]->followSets, production[index2]);
                            changed = changed || res;
                            // CANNOT CONTAIN EPSILON: NO FURTHER FIRST SETS ARE ADDED
                            break;
                        }

                        // NON TERMINAL: ADD ITS FIRST SET
                        res = appendSetToSet(sets_for_all[production[index1].val]->followSets, sets_for_all[production[index2].val]->firstSets);
                        changed = changed || res;


                        // NO EPSILON: NO MORE FIRST SETS ADDED FROM THIS PRODUCTION
                        if (!containsEPS(sets_for_all[production[index2].val]->firstSets))
                        {
                            break;
                        }
                    }

                    // EPSILON UPTO THE LAST SYMBOL AFTER INDEX1
                    if (index2 == length || isDefault(production[index2]))
                    {
                        res = appendSetToSet(sets_for_all[production[index1].val]->followSets, sets_for_all[nonTerminal]->followSets);
                        
                        changed = changed || res;
                    }

                    // REMOVE EPSILON FROM FOLLOW SET: NO FOLLOW SET CAN HAVE EPSILON
                    sets_for_all[production[index1].val]->followSets->booleanArr[TK_EPS] = false;
                }
            }
        }
    }

    // PRINT ALL THE FOLLOW SETS NON TERMINALS
    for (int i = 0; i < NUM_NON_TERMINALS; i += 1)
    {
        printSetWithIndex(sets_for_all[i]->followSets, i, 1, 1);
    }
}


void computeFirstSetNT(struct GrammarRule *productions, struct Sets **sets_for_all, int nonTerminal)
{
    // FOR EVERY PRODUCTION
    int numberOfRules = productions[nonTerminal].numProductions;
    for (int j = 0; j < numberOfRules; j++)
    {
        // GET THE PRODUCTION
        struct Variable *production = productions[nonTerminal].rules[j];

        // first we check if the rule is an epsilon rule for that non terminal
        if (production[0].val == TK_EPS)
        {
            appendVarToSet(sets_for_all[nonTerminal]->firstSets, production[0]);
            continue;
        }

        // index keeps track of how many non-terminals we have looked at so far
        int index = 0;

        // length is supposed to keep the max of non-terminals we could have to check for
        int length = sizeof(productions[nonTerminal].rules[j]) / sizeof(productions[nonTerminal].rules[j][0]);

        int set_index = recomputeSetIndex(production[index]);

        while (index < length && production[index].flag == 1)
        {
            // RECOMPUTE SET INDEX
            set_index = production[index].val;

            // CHECK WHETHER THE SET IS EMPTY
            if (isEmpty(sets_for_all[set_index]->firstSets))
            {
                // COMPUTE ITS FIRST SET
                computeFirstSetNT(productions, sets_for_all, production[index].val);
            }

            // ADD THE SET
            appendSetToSet(sets_for_all[nonTerminal]->firstSets, sets_for_all[set_index]->firstSets);

            // I ONLY KNOW IF THE SET HAS EPSILON AFTER COMPUTING IT IF IT WAS EMPTY
            if (!containsEPS(sets_for_all[set_index]->firstSets))
            {
                break;
            }

            index++;
        }

        if (index != length || isDefault(production[index]))
        {
            // RECOMPUTE SET INDEX
            set_index = recomputeSetIndex(production[index]);

            if (nonTerminal == NT_FUN_CALL_STMT)
            {
                printf("%d\n", index);
            }

            // printf("INDEX %d APPENDED\n", index);
            appendSetToSet(sets_for_all[nonTerminal]->firstSets, sets_for_all[set_index]->firstSets);
            sets_for_all[nonTerminal]->firstSets->booleanArr[TK_EPS] = 0;
        }

        else
        {
            sets_for_all[nonTerminal]->firstSets->booleanArr[TK_EPS] = 1;
        }
    }
}
void computeFirstSet(struct Sets **sets_for_all, struct GrammarRule *productions)
{
    // for all the terminal first sets, we add the terminal to the first set
    // I have created the sets_for_all as an array of Sets structures. Each structure holds the first and follow sets for that variable
    // I have kept the non terminals first and only then have I kept the terminals
    for (int i = NUM_NON_TERMINALS; i < NUM_NON_TERMINALS + NUM_TERMINALS; i++)
    {
        sets_for_all[i]->firstSets->booleanArr[i - NUM_NON_TERMINALS] = true;
    }

    // for (int hardik = 0; hardik < 3; hardik += 1)
    // {
    for (int i = 0; i < NUM_NON_TERMINALS; i++)
    {
        // ONLY COMPUTE THHE SETS ONCE
        if (isEmpty(sets_for_all[i]->firstSets))
        {
            computeFirstSetNT(productions, sets_for_all, i);
        }
    }
    // }

    // PRINT ALL THE FIRST SETS NON TERMINALS
    for (int i = 0; i < NUM_NON_TERMINALS; i += 1)
    {
        printSetWithIndex(sets_for_all[i]->firstSets, i, 1, 0);
    }
}


struct Sets** initialiseSetsWhole(){
    struct Sets** sets_for_all = (struct Sets **)malloc(sizeof(struct Sets *) * (NUM_NON_TERMINALS + NUM_TERMINALS));

    for (int i = 0; i < NUM_NON_TERMINALS + NUM_TERMINALS; i += 1)
    {         sets_for_all[i] = initializeSets();
    }

    return sets_for_all;
}


int main()
{
    freopen("output.txt", "w", stdout);
    struct Sets **sets_for_all = initialiseSetsWhole();

    struct GrammarRule productions[NUM_NON_TERMINALS] = {{1, {{{NT_OTHER_FUNCTIONS, 1}, {NT_MAIN_FUNCTION, 1}}}}, {1, {{{TK_MAIN, 0}, {NT_STMTS, 1}, {TK_END, 0}}}}, {2, {{{NT_FUNCTION, 1}, {NT_OTHER_FUNCTIONS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_FUNID, 0}, {NT_INPUT_PAR, 1}, {NT_OUTPUT_PAR, 1}, {TK_SEM, 0}, {NT_STMTS, 1}, {TK_END, 0}}}}, {1, {{{TK_INPUT, 0}, {TK_PARAMETER, 0}, {TK_LIST, 0}, {TK_SQL, 0}, {NT_PARAMETER_LIST, 1}, {TK_SQR, 0}}}}, {2, {{{TK_OUTPUT, 0}, {TK_PARAMETER, 0}, {TK_LIST, 0}, {TK_SQL, 0}, {NT_PARAMETER_LIST, 1}, {TK_SQR, 0}}, {{TK_EPS, 0}}}}, {1, {{{NT_DATA_TYPE, 1}, {TK_ID, 0}, {NT_REMAINING_LIST, 1}}}}, {2, {{{NT_PRIMITIVE_DATA_TYPE, 1}}, {{NT_CONSTRUCTED_DATA_TYPE, 1}}}}, {2, {{{TK_INT, 0}}, {{TK_REAL, 0}}}}, {2, {{{NT_A, 1}, {TK_RUID, 0}}, {{TK_RUID, 0}}}}, {2, {{{TK_COMMA, 0}, {NT_PARAMETER_LIST, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_TYPE_DEFINITIONS, 1}, {NT_DECLARATIONS, 1}, {NT_OTHER_STMTS, 1}, {NT_RETURN_STMT, 1}}}}, {3, {{{NT_TYPE_DEFINITION, 1}, {NT_TYPE_DEFINITIONS, 1}}, {{NT_DEFINETYPE_STMT, 1}, {NT_TYPE_DEFINITIONS, 1}}, {{TK_EPS, 0}}}}, {2, {{{TK_RECORD, 0}, {TK_RUID, 0}, {NT_FIELD_DEFINITIONS, 1}, {TK_ENDRECORD, 0}}, {{TK_UNION, 0}, {TK_RUID, 0}, {NT_FIELD_DEFINITIONS, 1}, {TK_ENDUNION, 0}}}}, {1, {{{NT_FIELD_DEFINITION, 1}, {NT_FIELD_DEFINITIONS, 1}, {NT_MORE_FIELDS, 1}}}}, {1, {{{TK_TYPE, 0}, {NT_DATA_TYPE, 1}, {TK_COLON, 0}, {TK_FIELDID, 0}, {TK_SEM, 0}}}}, {2, {{{NT_FIELD_DEFINITION, 1}, {NT_MORE_FIELDS, 1}}, {{TK_EPS, 0}}}}, {2, {{{NT_DECLARATION, 1}, {NT_DECLARATIONS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_TYPE, 0}, {NT_DATA_TYPE, 1}, {TK_COLON, 0}, {TK_ID, 0}, {NT_GLOBAL_OR_NOT, 1}, {TK_SEM, 0}}}}, {2, {{{TK_COLON, 0}, {TK_GLOBAL, 0}}, {{TK_EPS, 0}}}}, {2, {{{NT_STMT, 1}, {NT_OTHER_STMTS, 1}}, {{TK_EPS, 0}}}}, {5, {{{NT_ASSIGNMENT_STMT, 1}}, {{NT_ITERATIVE_STMT, 1}}, {{NT_CONDITIONAL_STMT, 1}}, {{NT_IO_STMT, 1}}, {{NT_FUN_CALL_STMT, 1}}}}, {1, {{{NT_SINGLE_OR_REC_ID, 1}, {TK_ASSIGNOP, 0}, {NT_ARITHMETIC_EXPRESSION, 1}, {TK_SEM, 0}}}}, {1, {{{TK_ID, 0}, {NT_REC_ID, 1}}}}, {2, {{{TK_DOT, 0}, {TK_FIELDID, 0}, {NT_REC_ID, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_OUTPUT_PARAMETERS, 1}, {TK_CALL, 0}, {TK_FUNID, 0}, {TK_WITH, 0}, {TK_PARAMETERS, 0}, {NT_INPUT_PARAMETERS, 1}, {TK_SEM, 0}}}}, {2, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}, {TK_ASSIGNOP, 0}}, {{TK_EPS, 0}}}}, {1, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}}}}, {1, {{{TK_WHILE, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}, {TK_ENDWHILE, 0}}}}, {1, {{{TK_IF, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {TK_THEN, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}, {NT_NEW3, 1}, {TK_ENDIF, 0}}}}, {2, {{{TK_ELSE, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}}, {{TK_EPS, 0}}}}, {2, {{{TK_READ, 0}, {TK_OP, 0}, {NT_VAR, 1}, {TK_CL, 0}, {TK_SEM, 0}}, {{TK_WRITE, 0}, {TK_OP, 0}, {NT_VAR, 1}, {TK_CL, 0}, {TK_SEM, 0}}}}, {1, {{{NT_TERM, 1}, {NT_NEW5, 1}}}}, {2, {{{NT_OPERATOR, 1}, {NT_TERM, 1}, {NT_NEW5, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_FACTOR, 1}, {NT_NEW6, 1}}}}, {2, {{{NT_OP_H, 1}, {NT_FACTOR, 1}, {NT_NEW6, 1}}, {{TK_EPS, 0}}}}, {2, {{{NT_VAR, 1}}, {{TK_OP, 0}, {NT_ARITHMETIC_EXPRESSION, 1}, {TK_CL, 0}}}}, {2, {{{TK_PLUS, 0}}, {{TK_MINUS, 0}}}}, {2, {{{TK_MUL, 0}}, {{TK_DIV, 0}}}}, {3, {{{TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {NT_LOGICAL_OP, 1}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}}, {{NT_VAR, 1}, {NT_RELATIONAL_OP, 1}, {NT_VAR, 1}}, {{TK_NOT, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}}}}, {3, {{{NT_SINGLE_OR_REC_ID, 1}}, {{TK_NUM, 0}}, {{TK_RNUM, 0}}}}, {2, {{{TK_AND, 0}}, {{TK_OR, 0}}}}, {6, {{{TK_LT, 0}}, {{TK_LE, 0}}, {{TK_EQ, 0}}, {{TK_GT, 0}}, {{TK_GE, 0}}, {{TK_NE, 0}}}}, {1, {{{TK_RETURN, 0}, {NT_OPTIONAL_RETURN, 1}, {TK_SEM, 0}}}}, {2, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}}, {{TK_EPS, 0}}}}, {1, {{{TK_ID, 0}, {NT_MORE_IDS, 1}}}}, {2, {{{TK_COMMA, 0}, {TK_ID, 0}, {NT_MORE_IDS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_DEFINETYPE, 0}, {NT_A, 1}, {TK_RUID, 0}, {TK_AS, 0}, {TK_RUID, 0}}}}, {2, {{{TK_RECORD, 0}}, {{TK_UNION, 0}}}}};
    
    computeFirstSet(sets_for_all, productions);
    computeFollowSet(sets_for_all, productions);

    struct ParsingTable *PT = (struct ParsingTable *)malloc(sizeof(struct ParsingTable));

    // fill up parsing table
    populate_parsing_table(PT, productions, sets_for_all);

    printParsingTable(PT);

    
}