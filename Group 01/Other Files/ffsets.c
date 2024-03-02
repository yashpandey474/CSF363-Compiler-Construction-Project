#include "../parser.h"
bool isDefault(struct Variable var)
{
    if (var.flag == 0 && var.val == 0)
    {
        return true;
    }
    return false;
}

struct Node *createLLNode(struct Variable data)
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

void insertAtBeginning(struct LinkedListArray *linkedList, struct Variable var)
{

    // Create a new node
    struct Node *newNode = createLLNode(var);

    // Set the next pointer of the new node to point to the current head
    newNode->next = linkedList->head;

    // Update the head to point to the new node
    linkedList->head = newNode;

    // INCREASE LENGTH
    linkedList->length += 1;
}

bool appendVarToSet(struct LinkedListSet *set, struct Variable element)
{
    if (element.val == CARRIAGE_RETURN)
    {
        return false;
    }

    if (set->booleanArr[element.val] == true)
    {
        // Element already exists in the set
        // printf("ELE ALREADY EXISTS\n");
        return false;
    }

    // INSERT INTO LINKED LIST
    insertAtBeginning(set->linkedList, element);

    // SET IN BOOLEAN ARRAY
    set->booleanArr[element.val] = true;
    return true;
}

bool containsEPS(struct LinkedListSet *set)
{
    return (set->booleanArr[TK_EPS] == true);
}

bool appendSetToSet(struct LinkedListSet *destinationSet, struct LinkedListSet *sourceSet)
{
    bool changed = false;
    struct Variable var;
    var.flag = 0;
    for (int i = 0; i < NUM_TERMINALS; ++i)
    {

        if ((i != TK_EPS) && (destinationSet->booleanArr[i] == false) && (sourceSet->booleanArr[i] == true))
        {
            changed = true;

            // INSERT INTO LINKED LIST
            var.val = i;
            insertAtBeginning(destinationSet->linkedList, var);

            // SET IN BOOLEAN ARRAY
            destinationSet->booleanArr[i] = true;
        }
    }
    return changed;
}

void fprintSetWithIndex(FILE *cfile, struct LinkedListSet *set, int setIndex, int flag, int firstorfollow)
{
    char str[10];
    if (firstorfollow)
    {
        strcpy(str, "First");
    }
    else
    {
        strcpy(str, "Follow");
    }

    if (flag == 1)
    {
        fprintf(cfile, "%s(%s): ", NonTerminalToString((enum NonTerminals)setIndex), str);
        for (int i = 0; i < NUM_TERMINALS; ++i)
        {
            // Assuming elements are integers for simplicity
            if (set->booleanArr[i] == true)
            {
                fprintf(cfile, "%s, ", TokenToString((enum Tokentype)i));
            }
        }
        fprintf(cfile, "\n");
    }
    else
    {
        fprintf(cfile, "%s(%s): ", TokenToString((enum Tokentype)setIndex), str);
        for (int i = 0; i < NUM_TERMINALS; ++i)
        {
            // Assuming elements are integers for simplicity
            if (set->booleanArr[i])
            {
                fprintf(cfile, "%s, ", TokenToString((enum Tokentype)i));
            }
        }
        fprintf(cfile, "\n");
    }
}

void printFFSetsTable(FILE *cfile, struct Sets **sets_for_all)
{
    for (int i = 0; i < NUM_NON_TERMINALS; i += 1)
    {
        fprintSetWithIndex(cfile, sets_for_all[i]->firstSets, i, 1, 1);
        fprintSetWithIndex(cfile, sets_for_all[i]->followSets, i, 1, 0);
        fprintf(cfile, "\n");
    }
    // for (int i = 0; i < NUM_TERMINALS; i += 1)
    // {
    //     fprintSetWithIndex(cfile, sets_for_all[i+NUM_NON_TERMINALS]->firstSets, i, 0, 1);
    //     fprintSetWithIndex(cfile, sets_for_all[i+NUM_NON_TERMINALS]->followSets, i, 0, 0);
    //     fprintf(cfile,"\n");
    // }
}

struct LinkedListSet *initialiseLLSet()
{
    struct LinkedListSet *set = (struct LinkedListSet *)malloc(sizeof(struct LinkedListSet));
    memset(set->booleanArr, false, sizeof(set->booleanArr));

    set->linkedList = (struct LinkedListArray *)malloc(sizeof(struct LinkedListArray));
    set->linkedList->head = NULL;
    set->linkedList->length = 0;

    return set;
}

struct Sets *initializeSets()
{
    struct Sets *newSets = (struct Sets *)malloc(sizeof(struct Sets));

    // Initialize firstSets
    newSets->firstSets = initialiseLLSet();

    // Initialize followSets
    newSets->followSets = initialiseLLSet();

    return newSets;
}

bool isEmpty(struct LinkedListSet *destinationSet)
{
    // DIRECTLY CHECK LINKED LIST INSTEAD OF ITERATING THROUGH BOOLEAN ARRAY
    if (destinationSet->linkedList->length == 0)
    {
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

void computeFollowSet(FirstAndFollow *sets_for_all, Productions productions)
{

    // ADD TK_EOF [END OF INPUT MARKER] to follow set of NT_PROGRAM (start symbol)
    struct Variable eof;
    eof.val = TK_EOF;
    eof.flag = 0;
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
    // removed, now printed to file
    // for (int i = 0; i < NUM_NON_TERMINALS; i += 1)
    // {
    //     printSetWithIndex(sets_for_all[i]->followSets, i, 1, 1);
    // }
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

void computeFirstSet(struct Sets **sets_for_all, Productions productions)
{
    // printf("FIRST SETS\n");
    // for all the terminal first sets, we add the terminal to the first set
    // I have created the sets_for_all as an array of Sets structures. Each structure holds the first and follow sets for that variable
    // I have kept the non terminals first and only then have I kept the terminals
    struct Variable var;
    var.flag = 0;

    for (int i = NUM_NON_TERMINALS; i < NUM_NON_TERMINALS + NUM_TERMINALS; i++)
    {
        // INSERT IN LINKED LIST
        var.val = i - NUM_NON_TERMINALS;
        insertAtBeginning(sets_for_all[i]->firstSets->linkedList, var);

        // SET IN BOOLEAN ARRAY
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
    // removed, now printed to file
    // for (int i = 0; i < NUM_NON_TERMINALS; i += 1)
    // {
    //     printSetWithIndex(sets_for_all[i]->firstSets, i, 1, 0);
    // }
}

struct Sets **initialiseSetsWhole()
{
    struct Sets **sets_for_all = (struct Sets **)malloc(sizeof(struct Sets *) * (NUM_NON_TERMINALS + NUM_TERMINALS));

    for (int i = 0; i < NUM_NON_TERMINALS + NUM_TERMINALS; i += 1)
    {
        sets_for_all[i] = initializeSets();
    }

    return sets_for_all;
}
