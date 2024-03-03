#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include <string.h>

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

void computeFollowSet(FirstAndFollow *sets_for_all, struct GrammarRule *productions)
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

void computeFirstSet(struct Sets **sets_for_all, struct GrammarRule *productions)
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

struct tree_node *create_tree_node(struct Variable *data)
{
    struct tree_node *new_tree_node = (struct tree_node *)malloc(sizeof(struct tree_node));
    new_tree_node->data = data;
    new_tree_node->next = NULL;
    new_tree_node->head = NULL;
    new_tree_node->parent = NULL;
    return new_tree_node;
}
void serialize_node(struct tree_node *node, FILE *output)
{
    if (node == NULL)
        return;

    // Serialize current node data and its relationships
    fprintf(output, "{\n");
    fprintf(output, "  \"current\": \"%p\",\n", (void *)node);

    if (node->data->flag == 1)
    {
        fprintf(output, "  \"data\": \"%s\",\n", NonTerminalToString(node->data->val)); // Assuming data can be represented as a string
    }
    else
    {
        fprintf(output, "  \"data\": \"%s\",\n", TokenToString(node->data->val)); // Assuming data can be represented as a string
    }
    if (node->parent != NULL)
    {
        fprintf(output, "  \"parent\": \"%p\",\n", (void *)node->parent);
    }
    if (node->next != NULL)
    {
        fprintf(output, "  \"next\": \"%p\",\n", (void *)node->next);
    }
    fprintf(output, "  \"children\": [\n");

    struct tree_node *child = node->head;
    while (child != NULL)
    {
        serialize_node(child, output);
        child = child->next;
        if (child != NULL)
            fprintf(output, ",\n");
    }

    fprintf(output, "  ]\n");
    fprintf(output, "}\n");
}

void serialize_tree(struct tree_node *root)
{
    printf("%d", root->data->val);
    printf("Serializing tree\n");
    FILE *output = fopen("tree_structure.json", "w");
    if (output == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    serialize_node(root, output);

    fclose(output);
}

// function to add a tree_node to the linkedlist which functions as the children of the parse tree
// the tree_nodes are added to the head (they are in reverse order)
void add_tree_node(struct tree_node *parent, struct tree_node *child)
{
    child->next = parent->head;
    parent->head = child;
    child->parent = parent;
}

// function to find the next non terminal for a given node
struct tree_node *nextNonTerminal(struct tree_node *current)
{
    if (current->data->flag == 1)
    {
        printf("FINDING NEXT NT OF: %s\n", NonTerminalToString(current->data->val));
    }
    else
    {
        printf("FINDING NEXT NT OF: %s\n", TokenToString(current->data->val));
    }
    while (current->next != NULL && current->data->flag == 0)
    {
        current = current->next;
    }

    if (current->data->flag == 1)
    {
        printf("RETURNED: %s\n", NonTerminalToString(current->data->val));
        return current;
    }
    while (current->parent != NULL && current->parent->next == NULL)
    {
        current = current->parent;
    }
    if (current->parent == NULL)
    {
        return NULL;
    }
    return nextNonTerminal(current->parent->next);
}

// function which takes the structure described in the first few comments as input and the tree_node where the rule has to be added
struct tree_node *repeated_add(struct tree_node *parent, struct input_structure input, struct Variable **rule)
{

    if (parent->data->val != input.nonterminal->val)
    {
        printf("Error: The input does not match the first non-terminal found\n");
        printf("Non-terminal entered: %s\n", NonTerminalToString(input.nonterminal->val));
        printf("Non-terminal found: %s\n", NonTerminalToString(parent->data->val));
        return nextNonTerminal;
    }

    for (int var = 8; var >= 0; var -= 1)
    {
        if (rule[var] == NULL)
        {
            continue;
        }

        input.length += 1;

        add_tree_node(parent, create_tree_node(rule[var]));
    }

    return nextNonTerminal(parent->head);
}
// parent = add_to_tree(topStack, a, pt, parent);
struct tree_node *add_to_tree(struct Variable *nt, struct Variable **rule, struct tree_node *parent)
{
    struct input_structure inp;

    printf("NONTERMINAL IN TREE: %s\n", NonTerminalToString(nt->val));

    inp.nonterminal = nt;

    parent = repeated_add(parent, inp, rule);

    if (parent != NULL)
    {
        printf("PARENT POINTER; %s\n", NonTerminalToString(parent->data->val));
    }
    // printf("TREE RETURNED NEXT NONTERMINAL: %s\n", NonTerminalToString(parent->data->val));
    return parent;
}

void printNodeDetails(struct tree_node *node, FILE *outfile)
{
    char *parentNodeSymbol = node->parent == NULL ? "ROOT" : node->parent->data->token->lexeme;
    struct SymbolTableEntry *token = node->data->token;
    char *lexeme = node->data->flag == 0 ? token->lexeme : "----";
    int lineNo = token->lineNo;
    const char *tokenName = TokenToString(token->tokenType);
    char *valueIfNumber;
    int isLeaf = node->data->flag == 0;
    const char *nodeSymbol = isLeaf ? "LEAF" : NonTerminalToString(node->data->val);

    if (token->tokenType == TK_RNUM)
    {
        fprintf(outfile, "%-20s %-5d %-20s %-20lf %-20s %-3s %-20s\n",
                lexeme, lineNo, tokenName, token->doubleValue, parentNodeSymbol, isLeaf ? "yes" : "no", "RNUM");
    }
    else if (token->tokenType == TK_NUM)
    {
        fprintf(outfile, "%-20s %-5d %-20s %-20d %-20s %-3s %-20s\n",
                lexeme, lineNo, tokenName, token->intValue, parentNodeSymbol, isLeaf ? "yes" : "no", "NUM");
    }
    else
    {
        fprintf(outfile, "%-20s %-5d %-20s %-20s %-20s %-3s %-20s\n",
                lexeme, lineNo, tokenName, "-----", parentNodeSymbol, isLeaf ? "yes" : "no", "NON-NUM");
    }
}

void inorderTraversal(struct tree_node *node, FILE *outfile)
{
    if (node == NULL)
        return;

    int isLeaf = node->data->flag == 0;
    if (node->head != NULL)
    {
        inorderTraversal(node->head, outfile);
    }

    // Print the current (parent) node
    printNodeDetails(node, outfile);

    // For the remaining children
    struct tree_node *sibling = node->head ? node->head->next : NULL;
    while (sibling != NULL)
    {
        inorderTraversal(sibling, outfile); // Assuming lexeme as the non-terminal symbol.
        sibling = sibling->next;
    }
}

void printParseTree(parseTree *PT, char *outfile)
{
    FILE *file = fopen(outfile, "w");
    if (!file)
    {
        printf("Failed to open the file %s for writing.\n", outfile);
        return;
    }

    if (PT && PT->root)
    {
        inorderTraversal(PT->root, file);
    }

    fclose(file);
}

void printRule(enum NonTerminals nt, struct Variable *ruleArray)
{

    printf("%s ===> ", NonTerminalToString(nt));

    for (int i = 0; i < 9; i++)
    {
        if (ruleArray[i].val == 0 && ruleArray[i].flag == 0)
        {
            break;
        }
        if (ruleArray[i].flag == 0)
        {
            printf("%s ", TokenToString(ruleArray[i].val));
        }
        else
        {
            printf("%s ", NonTerminalToString(ruleArray[i].val));
        }
    }

    printf("\n");
}
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
                printRule(i, pt->table[i][j]);
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

void addSyn(struct ParsingTable *PT, struct Sets **sets_for_all, int nonTerminal, int *synchSet, int defaultSynchSetSize)
{
    // SYN INDICATING RULE
    struct Variable *rule = (struct Variable *)malloc(sizeof(struct Variable));
    rule[0].val = -1;

    struct Node *current = sets_for_all[nonTerminal]->followSets->linkedList->head;

    // FOR ALL TERMINALS IN FOLLOW OF NONTERMINAL
    while (current != NULL)
    {
        // INSERT INTO PARSING TABLE
        insert(nonTerminal, current->data.val, rule, PT);
        current = current->next;
    }

    // FOR ALL IN DEFAULT SYNC SET
    for (int i = 0; i < defaultSynchSetSize; i += 1)
    {
        insert(nonTerminal, synchSet[i], rule, PT);
    }
}

void createParseTable(struct ParsingTable *PT, struct GrammarRule *productions, struct Sets **sets_for_all, int *synchSet, int defaultSynchSetSize)
{
    for (int nt = 0; nt < NUM_NON_TERMINALS; nt++)
    {
        // ADD THE SYN INDICATOR FOR ALL TERMINALS IN FOLLOW OF NT
        addSyn(PT, sets_for_all, nt, synchSet, defaultSynchSetSize);

        for (int i = 0; i < productions[nt].numProductions; i++)
        {
            // GET CURRENT RULE
            struct Variable *rule = productions[nt].rules[i];

            // EPSILON RULE
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

                int var = 0;
                for (; var < 9 && !(isDefault(rule[var])); var += 1)
                {

                    int set_index = rule[var].val;
                    if (rule[var].flag == 0)
                    {
                        set_index += NUM_NON_TERMINALS;
                    }
                    struct Node *current = sets_for_all[set_index]->firstSets->linkedList->head;

                    while (current != NULL)
                    {
                        // DONT ADD TK_EPS
                        if (current->data.val == TK_EPS)
                        {
                            current = current->next;
                            continue;
                        }
                        insert(nt, current->data.val, rule, PT);
                        current = current->next;
                    }

                    if (!(containsEPS(sets_for_all[set_index]->firstSets)))
                    {
                        break;
                    }
                }

                if (var == 9 || isDefault(rule[var]))
                {
                    // ADD THE RULE IN FOLLOW OF THE VAR [EPSILON RULE]
                    struct Node *current = sets_for_all[nt]->followSets->linkedList->head;

                    while (current != NULL)
                    {
                        insert(nt, current->data.val, rule, PT);
                        current = current->next;
                    }
                }
            }
        }
    }

    return;
}

const char *NonTerminalToString(enum NonTerminals nonTerminal)
{
    switch (nonTerminal)
    {
    case NT_PROGRAM:
        return "NT_PROGRAM";
    case NT_MAIN_FUNCTION:
        return "NT_MAIN_FUNCTION";
    case NT_OTHER_FUNCTIONS:
        return "NT_OTHER_FUNCTIONS";
    case NT_FUNCTION:
        return "NT_FUNCTION";
    case NT_INPUT_PAR:
        return "NT_INPUT_PAR";
    case NT_OUTPUT_PAR:
        return "NT_OUTPUT_PAR";
    case NT_PARAMETER_LIST:
        return "NT_PARAMETER_LIST";
    case NT_DATA_TYPE:
        return "NT_DATA_TYPE";
    case NT_PRIMITIVE_DATA_TYPE:
        return "NT_PRIMITIVE_DATA_TYPE";
    case NT_CONSTRUCTED_DATA_TYPE:
        return "NT_CONSTRUCTED_DATA_TYPE";
    case NT_REMAINING_LIST:
        return "NT_REMAINING_LIST";
    case NT_STMTS:
        return "NT_STMTS";
    case NT_TYPE_DEFINITIONS:
        return "NT_TYPE_DEFINITIONS";
    case NT_TYPE_DEFINITION:
        return "NT_TYPE_DEFINITION";
    case NT_FIELD_DEFINITIONS:
        return "NT_FIELD_DEFINITIONS";
    case NT_FIELD_DEFINITION:
        return "NT_FIELD_DEFINITION";
    case NT_MORE_FIELDS:
        return "NT_MORE_FIELDS";
    case NT_DECLARATIONS:
        return "NT_DECLARATIONS";
    case NT_DECLARATION:
        return "NT_DECLARATION";
    case NT_GLOBAL_OR_NOT:
        return "NT_GLOBAL_OR_NOT";
    case NT_OTHER_STMTS:
        return "NT_OTHER_STMTS";
    case NT_STMT:
        return "NT_STMT";
    case NT_ASSIGNMENT_STMT:
        return "NT_ASSIGNMENT_STMT";
    case NT_SINGLE_OR_REC_ID:
        return "NT_SINGLE_OR_REC_ID";
    case NT_REC_ID:
        return "NT_REC_ID";
    case NT_FUN_CALL_STMT:
        return "NT_FUN_CALL_STMT";
    case NT_OUTPUT_PARAMETERS:
        return "NT_OUTPUT_PARAMETERS";
    case NT_INPUT_PARAMETERS:
        return "NT_INPUT_PARAMETERS";
    case NT_ITERATIVE_STMT:
        return "NT_ITERATIVE_STMT";
    case NT_CONDITIONAL_STMT:
        return "NT_CONDITIONAL_STMT";
    case NT_NEW3:
        return "NT_NEW3";
    case NT_IO_STMT:
        return "NT_IO_STMT";
    case NT_ARITHMETIC_EXPRESSION:
        return "NT_ARITHMETIC_EXPRESSION";
    case NT_NEW5:
        return "NT_NEW5";
    case NT_TERM:
        return "NT_TERM";
    case NT_NEW6:
        return "NT_NEW6";
    case NT_FACTOR:
        return "NT_FACTOR";
    case NT_OPERATOR:
        return "NT_OPERATOR";
    case NT_OP_H:
        return "NT_OP_H";
    case NT_BOOLEAN_EXPRESSION:
        return "NT_BOOLEAN_EXPRESSION";
    case NT_VAR:
        return "NT_VAR";
    case NT_LOGICAL_OP:
        return "NT_LOGICAL_OP";
    case NT_RELATIONAL_OP:
        return "NT_RELATIONAL_OP";
    case NT_RETURN_STMT:
        return "NT_RETURN_STMT";
    case NT_OPTIONAL_RETURN:
        return "NT_OPTIONAL_RETURN";
    case NT_ID_LIST:
        return "NT_ID_LIST";
    case NT_MORE_IDS:
        return "NT_MORE_IDS";
    case NT_DEFINETYPE_STMT:
        return "NT_DEFINETYPE_STMT";
    case NT_A:
        return "NT_A";

    default:
    {
        char *result = (char *)malloc(50);
        snprintf(result, 50, "Unknown NonTerminal - %s", TokenToString(nonTerminal));
        return result;
    }
    }
}

#define STACK_INITIAL_SIZE 128
struct stack
{
    struct Variable **stack;
    int top;
    int MAX;
};
void printStack(struct stack *st)
{
    for (int i = 0; i <= st->top; i++)
    {

        if ((st->stack[i])->flag == 1)
        {
            printf("%s ", NonTerminalToString((st->stack[i])->val));
        }

        else
        {
            printf("%s ", TokenToString((st->stack[i])->val));
        }
    }
    printf("\n");
}
bool isEmptyStack(struct stack *st)
{
    return st->top == -1;
}

struct Variable *peek(struct stack *st)
{

    if (!isEmptyStack(st))
    {
        return st->stack[st->top];
    }
    else
    {
        printf("Stack is empty\n");
        return NULL;
    }
}

bool onlyContainsEOF(struct stack *st)
{
    struct Variable *topStack = peek(st);
    return st->top == 0 && topStack->val == TK_EOF && topStack->flag == 0;
}

bool isFull(struct stack *st)
{
    if (st->top == st->MAX - 1)
    {
        st->MAX += 128;
        st->stack = (struct Variable **)realloc(st->stack, 2 * st->MAX * sizeof(struct Variable *));
        return true;
    }
    return false;
}

void push(struct stack *st, struct Variable *data)
{
    // CHECKS IF FULL AND REALLOCATES
    isFull(st);

    st->stack[++st->top] = data;
}

struct Variable *pop(struct stack *st)
{

    if (isEmptyStack(st))
    {
        printf("Stack is empty\n");
        return NULL;
    }

    return st->stack[st->top--];
}

struct Variable *createCopy(struct Variable var)
{
    struct Variable *copy = (struct Variable *)malloc(sizeof(struct Variable));
    copy->flag = var.flag;
    copy->val = var.val;
    // no need ot token
    return copy;
}

int parseInputSourceCode(struct SymbolTableEntry *token, struct ParsingTable *pt, struct stack *st, twinBuffer LA, struct tree_node *parent, bool skipError, struct tree_node **parentpointer)
{

    enum Tokentype a = token->tokenType;

    struct Variable *X = st->stack[st->top];

    // BOTH ARE TERMINALS
    if (X->val == a && X->flag == 0)
    {
        printf("POPPED TERMINAL: %s\n", TokenToString(X->val));
        X->token = token;
        pop(st);
        return 1;
    }

    else if (X->flag == 0)
    {
        // CALL ERROR FUNCTION
        printf("Line no. %-5d Error: The token %s for lexeme %s  does not match with the expected token %s\n", LA->lineNo, TokenToString(a), token->lexeme, TokenToString(X->val));
        pop(st);
        return 0;
    }

    // ERROR
    else if (pt->table[X->val][a] == NULL)
    {
        if (!skipError)
            printf("Line %-5d Error: Invalid token %s encountered with value %s stack top %s\n", LA->lineNo, TokenToString(a), token->lexeme, NonTerminalToString(X->val));
        // go and get the next token

        // DISCARD UNTIL SYN OR VALID
        return -1;
    }
    // SYN
    else if (pt->table[X->val][a][0].val == -1)
    {
        // SYN TOKEN; POP THE NONTERMINAL
        // printf("Line %-5d Error: Invalid token %s encountered with value %s stack top %s\n", LA->lineNo, TokenToString(a), token->lexeme, NonTerminalToString(X.val));
        printf("synch encountered");
        pop(st);

        // CONTINUE FROM  SYN TOKEN
        return 0;
    }
    else
    {
        // GET THE RULE
        // struct Variable *arr = pt->table[X.val][a];
        struct Variable *topStack = pop(st);

        // ASSIGN THE TOKEN
        topStack->token = token;

        // printf("Pushing rule ");
        // printRule(topStack.val, arr);

        struct Variable **copyRule = (struct Variable **)malloc(sizeof(struct Variable *) * 9);
        int var = 8;
        for (; var >= 0; var -= 1)
        {
            if (isDefault(pt->table[X->val][a][var]))
            {
                continue;
            }

            if (pt->table[X->val][a][var].val == TK_EPS && pt->table[X->val][a][var].flag == 0)
            {
                break;
            }
            struct Variable *copy = createCopy(pt->table[X->val][a][var]);
            push(st, copy);
        }

        printf("While passing to add to tree stack was\n");
        printStack(st);

        // PASS TO TREE & GET CURRENT NODE
        parent = add_to_tree(topStack, copyRule, parent);

        printf("IN PARSING, GOT PARENT: %s\n", NonTerminalToString(parent->data->val));
        *parentpointer = parent;

        if (parent != NULL)
        {
            printf("RETURNED PARENT: %s\n", NonTerminalToString(parent->data->val));
        }

        return 0;
    }

    // TODO: WHAT TO RETURN IF ERROR
}

struct stack *initialiseStack()
{
    struct stack *stack = (struct stack *)malloc(sizeof(struct stack));

    stack->MAX = STACK_INITIAL_SIZE;
    stack->top = -1;
    stack->stack = (struct Variable **)malloc(sizeof(struct Variable *) * STACK_INITIAL_SIZE);
    return stack;
}

FirstAndFollow *computeFirstAndFollow(struct GrammarRule *productions)
{
    FirstAndFollow *sets = initialiseSetsWhole();
    computeFirstSet(sets, productions);
    // printf("FIRST SET COMPUTED\n");

    computeFollowSet(sets, productions);
    // printf("FOLLOW SET COMPUTED\n");

    return sets;
}

// int main()
// {

//     int synchSet[] = {
//         TK_ENDRECORD, TK_ENDUNION, TK_SEM, TK_DOT, TK_CL, TK_OP};

//     struct Variable *init = createCopy((struct Variable){NT_PROGRAM, 1});

//     struct tree_node *parent = create_tree_node(init);
//     struct tree_node *root_for_later = parent;
//     struct tree_node *node_to_add_to = parent;
//     struct tree_node **parentpointer = (struct tree_node **)malloc(sizeof(struct tree_node *));
//     *parentpointer = node_to_add_to;
//     struct stack *stack = initialiseStack();
//     insertAllKeywords();
//     FILE *file = readTestFile("t2.txt");

//     twinBufferArray bufferArray = initialiseTwinBuffer(file);
//     twinBuffer LA = initialiseLA(bufferArray);
//     struct SymbolTableEntry *token;

//     Grammar G = {{1, {{{NT_OTHER_FUNCTIONS, 1}, {NT_MAIN_FUNCTION, 1}}}}, {1, {{{TK_MAIN, 0}, {NT_STMTS, 1}, {TK_END, 0}}}}, {2, {{{NT_FUNCTION, 1}, {NT_OTHER_FUNCTIONS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_FUNID, 0}, {NT_INPUT_PAR, 1}, {NT_OUTPUT_PAR, 1}, {TK_SEM, 0}, {NT_STMTS, 1}, {TK_END, 0}}}}, {1, {{{TK_INPUT, 0}, {TK_PARAMETER, 0}, {TK_LIST, 0}, {TK_SQL, 0}, {NT_PARAMETER_LIST, 1}, {TK_SQR, 0}}}}, {2, {{{TK_OUTPUT, 0}, {TK_PARAMETER, 0}, {TK_LIST, 0}, {TK_SQL, 0}, {NT_PARAMETER_LIST, 1}, {TK_SQR, 0}}, {{TK_EPS, 0}}}}, {1, {{{NT_DATA_TYPE, 1}, {TK_ID, 0}, {NT_REMAINING_LIST, 1}}}}, {2, {{{NT_PRIMITIVE_DATA_TYPE, 1}}, {{NT_CONSTRUCTED_DATA_TYPE, 1}}}}, {2, {{{TK_INT, 0}}, {{TK_REAL, 0}}}}, {2, {{{NT_A, 1}, {TK_RUID, 0}}, {{TK_RUID, 0}}}}, {2, {{{TK_COMMA, 0}, {NT_PARAMETER_LIST, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_TYPE_DEFINITIONS, 1}, {NT_DECLARATIONS, 1}, {NT_OTHER_STMTS, 1}, {NT_RETURN_STMT, 1}}}}, {3, {{{NT_TYPE_DEFINITION, 1}, {NT_TYPE_DEFINITIONS, 1}}, {{NT_DEFINETYPE_STMT, 1}, {NT_TYPE_DEFINITIONS, 1}}, {{TK_EPS, 0}}}}, {2, {{{TK_RECORD, 0}, {TK_RUID, 0}, {NT_FIELD_DEFINITIONS, 1}, {TK_ENDRECORD, 0}}, {{TK_UNION, 0}, {TK_RUID, 0}, {NT_FIELD_DEFINITIONS, 1}, {TK_ENDUNION, 0}}}}, {1, {{{NT_FIELD_DEFINITION, 1}, {NT_FIELD_DEFINITION, 1}, {NT_MORE_FIELDS, 1}}}}, {1, {{{TK_TYPE, 0}, {NT_DATA_TYPE, 1}, {TK_COLON, 0}, {TK_FIELDID, 0}, {TK_SEM, 0}}}}, {2, {{{NT_FIELD_DEFINITION, 1}, {NT_MORE_FIELDS, 1}}, {{TK_EPS, 0}}}}, {2, {{{NT_DECLARATION, 1}, {NT_DECLARATIONS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_TYPE, 0}, {NT_DATA_TYPE, 1}, {TK_COLON, 0}, {TK_ID, 0}, {NT_GLOBAL_OR_NOT, 1}, {TK_SEM, 0}}}}, {2, {{{TK_COLON, 0}, {TK_GLOBAL, 0}}, {{TK_EPS, 0}}}}, {2, {{{NT_STMT, 1}, {NT_OTHER_STMTS, 1}}, {{TK_EPS, 0}}}}, {5, {{{NT_ASSIGNMENT_STMT, 1}}, {{NT_ITERATIVE_STMT, 1}}, {{NT_CONDITIONAL_STMT, 1}}, {{NT_IO_STMT, 1}}, {{NT_FUN_CALL_STMT, 1}}}}, {1, {{{NT_SINGLE_OR_REC_ID, 1}, {TK_ASSIGNOP, 0}, {NT_ARITHMETIC_EXPRESSION, 1}, {TK_SEM, 0}}}}, {1, {{{TK_ID, 0}, {NT_REC_ID, 1}}}}, {2, {{{TK_DOT, 0}, {TK_FIELDID, 0}, {NT_REC_ID, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_OUTPUT_PARAMETERS, 1}, {TK_CALL, 0}, {TK_FUNID, 0}, {TK_WITH, 0}, {TK_PARAMETERS, 0}, {NT_INPUT_PARAMETERS, 1}, {TK_SEM, 0}}}}, {2, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}, {TK_ASSIGNOP, 0}}, {{TK_EPS, 0}}}}, {1, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}}}}, {1, {{{TK_WHILE, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}, {TK_ENDWHILE, 0}}}}, {1, {{{TK_IF, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {TK_THEN, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}, {NT_NEW3, 1}, {TK_ENDIF, 0}}}}, {2, {{{TK_ELSE, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}}, {{TK_EPS, 0}}}}, {2, {{{TK_READ, 0}, {TK_OP, 0}, {NT_VAR, 1}, {TK_CL, 0}, {TK_SEM, 0}}, {{TK_WRITE, 0}, {TK_OP, 0}, {NT_VAR, 1}, {TK_CL, 0}, {TK_SEM, 0}}}}, {1, {{{NT_TERM, 1}, {NT_NEW5, 1}}}}, {2, {{{NT_OPERATOR, 1}, {NT_TERM, 1}, {NT_NEW5, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_FACTOR, 1}, {NT_NEW6, 1}}}}, {2, {{{NT_OP_H, 1}, {NT_FACTOR, 1}, {NT_NEW6, 1}}, {{TK_EPS, 0}}}}, {2, {{{NT_VAR, 1}}, {{TK_OP, 0}, {NT_ARITHMETIC_EXPRESSION, 1}, {TK_CL, 0}}}}, {2, {{{TK_PLUS, 0}}, {{TK_MINUS, 0}}}}, {2, {{{TK_MUL, 0}}, {{TK_DIV, 0}}}}, {3, {{{TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {NT_LOGICAL_OP, 1}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}}, {{NT_VAR, 1}, {NT_RELATIONAL_OP, 1}, {NT_VAR, 1}}, {{TK_NOT, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}}}}, {3, {{{NT_SINGLE_OR_REC_ID, 1}}, {{TK_NUM, 0}}, {{TK_RNUM, 0}}}}, {2, {{{TK_AND, 0}}, {{TK_OR, 0}}}}, {6, {{{TK_LT, 0}}, {{TK_LE, 0}}, {{TK_EQ, 0}}, {{TK_GT, 0}}, {{TK_GE, 0}}, {{TK_NE, 0}}}}, {1, {{{TK_RETURN, 0}, {NT_OPTIONAL_RETURN, 1}, {TK_SEM, 0}}}}, {2, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}}, {{TK_EPS, 0}}}}, {1, {{{TK_ID, 0}, {NT_MORE_IDS, 1}}}}, {2, {{{TK_COMMA, 0}, {NT_ID_LIST, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_DEFINETYPE, 0}, {NT_A, 1}, {TK_RUID, 0}, {TK_AS, 0}, {TK_RUID, 0}}}}, {2, {{{TK_RECORD, 0}}, {{TK_UNION, 0}}}}};
//     // for (int i=0;i<NUM_NON_TERMINALS;i++)
//     //     G.productions[i] = productions[i];

//     FirstAndFollow* sets = computeFirstAndFollow(G.productions);
//     struct ParsingTable *PT = (struct ParsingTable *)malloc(sizeof(struct ParsingTable));

//     getStream(bufferArray);
//     printf("READ INPUT\n");

//     createParseTable(PT, G.productions, sets, synchSet);
//     printf("PARSING TABLE POPULATED\n");
//     // printParsingTable(PT);

//     push(stack, createCopy((struct Variable){TK_EOF, 0}));
//     push(stack, init);

//     // write to computed_sets.txt
//     FILE *cfile = fopen("computed_sets.txt", "w");
//     if (cfile == NULL)
//     {
//         printf("Error opening file!\n");
//         return 1;
//     }
//     printFFSetsTable(cfile, sets);
//     fclose(cfile);

//     int res = 0;
//     bool skip_error = false;
//     while ((token = getNextToken(LA)))
//     {
//         if (token->tokenType == LEXICAL_ERROR)
//         {
//             printf("Line no. %-5d Error: %-30s\n", LA->lineNo, token->lexeme);
//         }
//         if (!(token->tokenType == LEXICAL_ERROR || token->tokenType == TK_COMMENT))
//         {
//             // printf("Stack before:\n");
//             // printStack(stack);
//             printf("PASSED PARENT TO PARSING: %s\n", NonTerminalToString(node_to_add_to->data->val));
//             while ((res = parseInputSourceCode(token, PT, stack, LA, node_to_add_to, skip_error, parentpointer)) == 0)
//             {
//                 if (*parentpointer != NULL)
//                 {
//                     node_to_add_to = *parentpointer;
//                 }
//                 // printf("Stack after:\n");

//                 // printStack(stack);

//                 // keep doing it basically. youll only go to the next token if there's a valid accepting thing( in which case it returns something)
//             }

//             if (res == -1)
//             {
//                 skip_error = true;
//             }
//             if (res == 1)
//             {

//                 skip_error = false;
//             }
//             if (*parentpointer != NULL)
//             {
//                 node_to_add_to = *parentpointer;
//             }
//         }
//     }
//     printf("After parsing\n");
//     printStack(stack);

//     printTree(parent, 0);

//     if (onlyContainsEOF(stack))
//     {
//         printf("TUTUTUDUUU MAX VERSTAPPEN: SYNTAX ANALYSIS COMPLETE\\n");
//     }
//     serialize_tree(root_for_later);

//     return 0;
// }