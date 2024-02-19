
// MAKE AN ENUM FOR NONTERMINALS#

#include "syntactical.h"

#include <stdbool.h>

// STRUCTURES FOR GRAMMAR

// Functions
bool appendVarToSet(bool *set, struct Variable element)
{
    if (element.val == CARRIAGE_RETURN){
        return false;
    }

    if (set[element.val] == true)
    {
        // Element already exists in the set
        // printf("ELE ALREADY EXISTS\n");
        return false;
    }

    // printf("ELE DID NOT EXIST?!");
    set[element.val] = true;
    return true;
}

bool containsEPS(bool *set)
{
    Tokentype value = TK_EPS;
    if (set[value] == true)
    {
        // printf("CONTANS EPS??");
        return true;
    }
    return false;
}

bool appendSetToSet(bool *destinationSet, bool *sourceSet)
{
    bool changed = false;
    for (int i = 0; i < NUM_TERMINALS; ++i)
    {

        if ((i != TK_EPS) && (destinationSet[i] == false) && (sourceSet[i] == true)){
            changed = true;
        }

        destinationSet[i] = (destinationSet[i] || sourceSet[i]);
    }
    return changed;
}

void printSetWithIndex(bool *set, size_t setIndex, int flag)
{
    if (flag == 1)
    {
        printf("Set %s: ", NonTerminalToString((enum NonTerminals)setIndex));
        for (size_t i = 0; i < NUM_TERMINALS; ++i)
        {
            // Assuming elements are integers for simplicity
            if (set[i])
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
            if (set[i])
            {
                printf("%s, ", TokenToString((enum Tokentype)i));
            }
        }
        printf("\n\n");
    }
}

void initSets(struct Sets *sets)
{
    memset(sets->firstSets, 0, sizeof(sets->firstSets));
    memset(sets->followSets, 0, sizeof(sets->followSets));
}


bool isEmpty(bool *destinationSet)
{
    for (size_t i = 0; i < NUM_TERMINALS; ++i)
    {
        if (destinationSet[i] == true)
        {
            return false;
        }
    }
    return true;
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
                    if (nonTerminal == NT_STMT)
                    {
                        printf("HEELLO WHY? %s %d %d\n", NonTerminalToString(production[index1].val), index2, length);
                    }

                    // EPSILON UPTO THE LAST SYMBOL AFTER INDEX1
                    if (index2 == length || (production[index2].val == 0 && production[index2].flag == 0))
                    {
                        res = appendSetToSet(sets_for_all[production[index1].val]->followSets, sets_for_all[nonTerminal]->followSets);
                        
                        changed = changed || res;
                    }

                    // REMOVE EPSILON FROM FOLLOW SET: NO FOLLOW SET CAN HAVE EPSILON
                    sets_for_all[production[index1].val]->followSets[TK_EPS] = false;
                }
            }
        }
    }

    // PRINT ALL THE FOLLOW SETS NON TERMINALS
    for (int i = 0; i < NUM_NON_TERMINALS; i += 1)
    {
        printSetWithIndex(sets_for_all[i]->followSets, i, 1);
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

        if (index != length)
        {
            // RECOMPUTE SET INDEX
            set_index = recomputeSetIndex(production[index]);

            if (nonTerminal == NT_FUN_CALL_STMT)
            {
                printf("%d\n", index);
            }

            // printf("INDEX %d APPENDED\n", index);
            appendSetToSet(sets_for_all[nonTerminal]->firstSets, sets_for_all[set_index]->firstSets);
            sets_for_all[nonTerminal]->firstSets[TK_EPS] = 0;
        }

        else
        {
            sets_for_all[nonTerminal]->firstSets[TK_EPS] = 1;
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
        sets_for_all[i]->firstSets[i - NUM_NON_TERMINALS] = true;
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
        printSetWithIndex(sets_for_all[i]->firstSets, i, 1);
    }
}

int main()
{
    // freopen("output.txt", "w", stdout);
    struct Sets **sets_for_all = (struct Sets **)malloc(sizeof(struct Sets *) * (NUM_NON_TERMINALS + NUM_TERMINALS));

    for (int i = 0; i < NUM_NON_TERMINALS + NUM_TERMINALS; i += 1)
    {
        sets_for_all[i] = (struct Sets *)malloc(sizeof(struct Sets));
        initSets(sets_for_all[i]);
    }

    struct GrammarRule productions[NUM_NON_TERMINALS] = {{1, {{{NT_OTHER_FUNCTIONS, 1}, {NT_MAIN_FUNCTION, 1}}}}, {1, {{{TK_MAIN, 0}, {NT_STMTS, 1}, {TK_END, 0}}}}, {2, {{{NT_FUNCTION, 1}, {NT_OTHER_FUNCTIONS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_FUNID, 0}, {NT_INPUT_PAR, 1}, {NT_OUTPUT_PAR, 1}, {TK_SEM, 0}, {NT_STMTS, 1}, {TK_END, 0}}}}, {1, {{{TK_INPUT, 0}, {TK_PARAMETER, 0}, {TK_LIST, 0}, {TK_SQL, 0}, {NT_PARAMETER_LIST, 1}, {TK_SQR, 0}}}}, {2, {{{TK_OUTPUT, 0}, {TK_PARAMETER, 0}, {TK_LIST, 0}, {TK_SQL, 0}, {NT_PARAMETER_LIST, 1}, {TK_SQR, 0}}, {{TK_EPS, 0}}}}, {1, {{{NT_DATA_TYPE, 1}, {TK_ID, 0}, {NT_REMAINING_LIST, 1}}}}, {2, {{{NT_PRIMITIVE_DATA_TYPE, 1}}, {{NT_CONSTRUCTED_DATA_TYPE, 1}}}}, {2, {{{TK_INT, 0}}, {{TK_REAL, 0}}}}, {2, {{{NT_A, 1}, {TK_RUID, 0}}, {{TK_RUID, 0}}}}, {2, {{{TK_COMMA, 0}, {NT_PARAMETER_LIST, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_TYPE_DEFINITIONS, 1}, {NT_DECLARATIONS, 1}, {NT_OTHER_STMTS, 1}, {NT_RETURN_STMT, 1}}}}, {3, {{{NT_TYPE_DEFINITION, 1}, {NT_TYPE_DEFINITIONS, 1}}, {{NT_DEFINETYPE_STMT, 1}, {NT_TYPE_DEFINITIONS, 1}}, {{TK_EPS, 0}}}}, {2, {{{TK_RECORD, 0}, {TK_RUID, 0}, {NT_FIELD_DEFINITIONS, 1}, {TK_ENDRECORD, 0}}, {{TK_UNION, 0}, {TK_RUID, 0}, {NT_FIELD_DEFINITIONS, 1}, {TK_ENDUNION, 0}}}}, {1, {{{NT_FIELD_DEFINITION, 1}, {NT_FIELD_DEFINITIONS, 1}, {NT_MORE_FIELDS, 1}}}}, {1, {{{TK_TYPE, 0}, {NT_DATA_TYPE, 1}, {TK_COLON, 0}, {TK_FIELDID, 0}, {TK_SEM, 0}}}}, {2, {{{NT_FIELD_DEFINITION, 1}, {NT_MORE_FIELDS, 1}}, {{TK_EPS, 0}}}}, {2, {{{NT_DECLARATION, 1}, {NT_DECLARATIONS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_TYPE, 0}, {NT_DATA_TYPE, 1}, {TK_COLON, 0}, {TK_ID, 0}, {NT_GLOBAL_OR_NOT, 1}, {TK_SEM, 0}}}}, {2, {{{TK_COLON, 0}, {TK_GLOBAL, 0}}, {{TK_EPS, 0}}}}, {2, {{{NT_STMT, 1}, {NT_OTHER_STMTS, 1}}, {{TK_EPS, 0}}}}, {5, {{{NT_ASSIGNMENT_STMT, 1}}, {{NT_ITERATIVE_STMT, 1}}, {{NT_CONDITIONAL_STMT, 1}}, {{NT_IO_STMT, 1}}, {{NT_FUN_CALL_STMT, 1}}}}, {1, {{{NT_SINGLE_OR_REC_ID, 1}, {TK_ASSIGNOP, 0}, {NT_ARITHMETIC_EXPRESSION, 1}, {TK_SEM, 0}}}}, {1, {{{TK_ID, 0}, {NT_REC_ID, 1}}}}, {2, {{{TK_DOT, 0}, {TK_FIELDID, 0}, {NT_REC_ID, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_OUTPUT_PARAMETERS, 1}, {TK_CALL, 0}, {TK_FUNID, 0}, {TK_WITH, 0}, {TK_PARAMETERS, 0}, {NT_INPUT_PARAMETERS, 1}, {TK_SEM, 0}}}}, {2, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}, {TK_ASSIGNOP, 0}}, {{TK_EPS, 0}}}}, {1, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}}}}, {1, {{{TK_WHILE, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}, {TK_ENDWHILE, 0}}}}, {1, {{{TK_IF, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {TK_THEN, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}, {NT_NEW3, 1}, {TK_ENDIF, 0}}}}, {2, {{{TK_ELSE, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}}, {{TK_EPS, 0}}}}, {2, {{{TK_READ, 0}, {TK_OP, 0}, {NT_VAR, 1}, {TK_CL, 0}, {TK_SEM, 0}}, {{TK_WRITE, 0}, {TK_OP, 0}, {NT_VAR, 1}, {TK_CL, 0}, {TK_SEM, 0}}}}, {1, {{{NT_TERM, 1}, {NT_NEW5, 1}}}}, {2, {{{NT_OPERATOR, 1}, {NT_TERM, 1}, {NT_NEW5, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_FACTOR, 1}, {NT_NEW6, 1}}}}, {2, {{{NT_OP_H, 1}, {NT_FACTOR, 1}, {NT_NEW6, 1}}, {{TK_EPS, 0}}}}, {2, {{{NT_VAR, 1}}, {{TK_OP, 0}, {NT_ARITHMETIC_EXPRESSION, 1}, {TK_CL, 0}}}}, {2, {{{TK_PLUS, 0}}, {{TK_MINUS, 0}}}}, {2, {{{TK_MUL, 0}}, {{TK_DIV, 0}}}}, {3, {{{TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {NT_LOGICAL_OP, 1}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}}, {{NT_VAR, 1}, {NT_RELATIONAL_OP, 1}, {NT_VAR, 1}}, {{TK_NOT, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}}}}, {3, {{{NT_SINGLE_OR_REC_ID, 1}}, {{TK_NUM, 0}}, {{TK_RNUM, 0}}}}, {2, {{{TK_AND, 0}}, {{TK_OR, 0}}}}, {6, {{{TK_LT, 0}}, {{TK_LE, 0}}, {{TK_EQ, 0}}, {{TK_GT, 0}}, {{TK_GE, 0}}, {{TK_NE, 0}}}}, {1, {{{TK_RETURN, 0}, {NT_OPTIONAL_RETURN, 1}, {TK_SEM, 0}}}}, {2, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}}, {{TK_EPS, 0}}}}, {1, {{{TK_ID, 0}, {NT_MORE_IDS, 1}}}}, {2, {{{TK_COMMA, 0}, {TK_ID, 0}, {NT_MORE_IDS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_DEFINETYPE, 0}, {NT_A, 1}, {TK_RUID, 0}, {TK_AS, 0}, {TK_RUID, 0}}}}, {2, {{{TK_RECORD, 0}}, {{TK_UNION, 0}}}}};
    computeFirstSet(sets_for_all, productions);
    computeFollowSet(sets_for_all, productions);
}