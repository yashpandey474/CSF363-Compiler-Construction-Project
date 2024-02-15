
// MAKE AN ENUM FOR NONTERMINALS#

#include "syntactical.h"

#include <stdbool.h>

// STRUCTURES FOR GRAMMAR


// Functions
int appendVarToSet(bool *set, struct Variable element)
{
    if (set[element.val] == true)
    {
        // Element already exists in the set
        return 0;
    }
    set[element.val] = true;
    return 1;
}

bool containsEPS(bool *set)
{
    Tokentype value = TK_EPS;
    if (set[value] == true)
    {
        return 1;
    }
    return 0;
}

void appendSetToSet(bool *destinationSet, bool *sourceSet)
{
    for (size_t i = 0; i < NUM_TERMINALS; ++i)
    {

        destinationSet[i] = destinationSet[i] || sourceSet[i];
    }
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
    case NT_NEW1:
        return "NT_NEW1";
    case NT_MORE_FIELDS:
        return "NT_MORE_FIELDS";
    case NT_DECLARATIONS:
        return "NT_DECLARATIONS";
    case NT_DECLARATION:
        return "NT_DECLARATION";
    case NT_OPTIONAL_GLOBAL:
        return "NT_OPTIONAL_GLOBAL";
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
        return "Unknown NonTerminal";
    }
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
    case TK_NUM2:
    case TK_NUM:
        return "TK_NUM";
    case TK_RNUM1:
    case TK_RNUM2:
    case TK_RNUM:
        return "TK_RNUM";
    case TK_FUNID:
        return "TK_FUNID";
    case TK_RUID:
        return "TK_RUID";
    case DELIMITER:
        return "DELIMITER";
    case TK_EPS:
        return "EPSILON";

    default:
        return "non accept state";
    }
}

bool isEmpty (bool *destinationSet){
    for (size_t i = 0; i < NUM_TERMINALS; ++i){
        if (destinationSet[i] == true){
            return false;
        }
    }
    return true;
}
void computeFirstSetNT(struct GrammarRule *productions, struct Sets **sets_for_all, int nonTerminal)
{
    //FOR EVERY PRODUCTION
    int numberOfRules = productions[nonTerminal].numProductions;
    for (int j = 0; j < numberOfRules; j++)
    {
        //GET THE PRODUCTION
        struct Variable *production = productions[nonTerminal].rules[j];

        // first we check if the rule is an epsilon rule for that non terminal
        if (production[0].val == TK_EPS)
        {
            appendVarToSet(sets_for_all[nonTerminal]->firstSets, production[0]);
            continue;
        }

        // if it is not an epsilon grammar rule, we simply check for the first non-terminal which cannot be epsilon
        // or the first terminal
        else
        {
            
            // index keeps track of how many non-terminals we have looked at so far
            int index = 0;
            // length is supposed to keep the max of non-terminals we could have to check for
            int length = sizeof(productions[nonTerminal].rules[j]) / sizeof(productions[nonTerminal].rules[j][0]);

            printf("length=%d\n", length);
            while (index < length && production[index].flag == 1 && containsEPS(sets_for_all[production[index].val]->firstSets))
            {
                
                //CHECK WHETHER THE SET IS EMPTY
                if (isEmpty(sets_for_all[production[index].val]->firstSets)){
                    //COMPUTE ITS FIRST SET
                    computeFirstSetNT(productions, sets_for_all, production[index].val);
                }

                //APPEND THE SET
                appendSetToSet(sets_for_all[nonTerminal]->firstSets, sets_for_all[production[index].val]->firstSets);
                index++;
                sets_for_all[nonTerminal]->firstSets[TK_EPS] = 0;

                printf("INDEX %d NONTERMINAL %s %s\n", index, NonTerminalToString(nonTerminal), NonTerminalToString(index));
            }
            // if all the productions have not had epsilon, then remove the epsilon value
            if (index != length)
            {
                // remove epsilon from sets_for_all->firstSets[nonTerminal]
                // if the flag was 0, and there was a terminal
                // or there was a non terminal whose first set did not contain epsilon
                // then we simply add the first set for that variable
                appendSetToSet(sets_for_all[nonTerminal]->firstSets, sets_for_all[production[index].val]->firstSets);

                if (production[index].flag == 1){
                    printf("NT INDEX %d FINAL %s %s\n", index, NonTerminalToString(nonTerminal), NonTerminalToString(production[index].val));
                }
                else{
                    printf("T INDEX %d FINAL %s %s %d\n", index, NonTerminalToString(nonTerminal), TokenToString(production[index].val));
                }
            }
            else
            {
                sets_for_all[nonTerminal]->firstSets[TK_EPS] = 1;
            }
        }
    }
    printSetWithIndex(sets_for_all[nonTerminal]->firstSets, nonTerminal, 1);
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


    // for all the non-terminals
    // while (changed){
        // changed = false
    for (int i = 0; i < NUM_NON_TERMINALS; i++)
    {
                // i holds the index of the non terminal whose grammar rules we are going through
                // productions[i] are the grammar rules for the ith non terminal
            
        computeFirstSetNT(productions, sets_for_all, i);
        
    }
    
}

int main()
{
    freopen("output.txt", "w", stdout);
    struct Sets** sets_for_all = (struct Sets**)malloc(sizeof(struct Sets*)*(NUM_NON_TERMINALS + NUM_TERMINALS));
    
    for (int i = 0; i < NUM_NON_TERMINALS + NUM_TERMINALS; i += 1)
    {
        sets_for_all[i] = (struct Sets *)malloc(sizeof(struct Sets));
        initSets(sets_for_all[i]);
    }

    struct GrammarRule productions[NUM_NON_TERMINALS] =
        {{1, {{{NT_OTHER_FUNCTIONS, 1}, {NT_MAIN_FUNCTION, 1}}}}, {1, {{{TK_MAIN, 0}, {NT_STMTS, 1}, {TK_END, 0}}}}, {2, {{{NT_FUNCTION, 1}, {NT_OTHER_FUNCTIONS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_FUNID, 0}, {NT_INPUT_PAR, 1}, {NT_OUTPUT_PAR, 1}, {TK_SEM, 0}, {NT_STMTS, 1}, {TK_END, 0}}}}, {1, {{{TK_INPUT, 0}, {TK_PARAMETER, 0}, {TK_LIST, 0}, {TK_SQL, 0}, {NT_PARAMETER_LIST, 1}, {TK_SQR, 0}}}}, {2, {{{TK_OUTPUT, 0}, {TK_PARAMETER, 0}, {TK_LIST, 0}, {TK_SQL, 0}, {NT_PARAMETER_LIST, 1}, {TK_SQR, 0}}, {{TK_EPS, 0}}}}, {1, {{{NT_DATA_TYPE, 1}, {TK_ID, 0}, {NT_REMAINING_LIST, 1}}}}, {2, {{{NT_PRIMITIVE_DATA_TYPE, 1}}, {{NT_CONSTRUCTED_DATA_TYPE, 1}}}}, {2, {{{TK_INT, 0}}, {{TK_REAL, 0}}}}, {1, {{{NT_A, 1}, {TK_RUID, 0}}}}, {2, {{{TK_COMMA, 0}, {NT_PARAMETER_LIST, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_TYPE_DEFINITIONS, 1}, {NT_DECLARATIONS, 1}, {NT_OTHER_STMTS, 1}, {NT_RETURN_STMT, 1}}}}, {3, {{{NT_TYPE_DEFINITION, 1}, {NT_TYPE_DEFINITIONS, 1}}, {{NT_DEFINETYPE_STMT, 1}, {NT_TYPE_DEFINITION, 1}}, {{TK_EPS, 0}}}}, {2, {{{TK_RECORD, 0}, {TK_RUID, 0}, {NT_FIELD_DEFINITIONS, 1}, {TK_ENDRECORD, 0}}, {{TK_UNION, 0}, {TK_RUID, 0}, {NT_FIELD_DEFINITIONS, 1}, {TK_ENDUNION, 0}}}}, {1, {{{NT_FIELD_DEFINITION, 1}, {NT_FIELD_DEFINITION, 1}, {NT_MORE_FIELDS, 1}}}}, {1, {{{TK_TYPE, 0}, {NT_NEW1, 1}}}}, {2, {{{NT_PRIMITIVE_DATA_TYPE, 1}, {TK_COLON, 0}, {TK_FIELDID, 0}, {TK_SEM, 0}}, {{TK_FIELDID, 0}, {TK_COLON, 0}, {TK_FIELDID, 0}, {TK_SEM, 0}}}}, {2, {{{NT_FIELD_DEFINITION, 1}, {NT_MORE_FIELDS, 1}}, {{TK_EPS, 0}}}}, {2, {{{NT_DECLARATION, 1}, {NT_DECLARATIONS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_TYPE, 0}, {NT_DATA_TYPE, 1}, {TK_COLON, 0}, {TK_ID, 0}, {NT_OPTIONAL_GLOBAL, 1}, {TK_SEM, 0}}}}, {2, {{{TK_COLON, 0}, {TK_GLOBAL, 0}}, {{TK_EPS, 0}}}}, {2, {{{NT_STMT, 1}, {NT_OTHER_STMTS, 1}}, {{TK_EPS, 0}}}}, {5, {{{NT_ASSIGNMENT_STMT, 1}}, {{NT_ITERATIVE_STMT, 1}}, {{NT_CONDITIONAL_STMT, 1}}, {{NT_IO_STMT, 1}}, {{NT_FUN_CALL_STMT, 1}}}}, {1, {{{NT_SINGLE_OR_REC_ID, 1}, {TK_ASSIGNOP, 0}, {NT_ARITHMETIC_EXPRESSION, 1}, {TK_SEM, 0}}}}, {1, {{{TK_ID, 0}, {NT_REC_ID, 1}}}}, {2, {{{TK_DOT, 0}, {TK_FIELDID, 0}, {NT_REC_ID, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_OUTPUT_PARAMETERS, 1}, {TK_CALL, 0}, {TK_FUNID, 0}, {TK_WITH, 0}, {TK_PARAMETERS, 0}, {NT_INPUT_PARAMETERS, 1}, {TK_SEM, 0}}}}, {2, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}, {TK_ASSIGNOP, 0}}, {{TK_EPS, 0}}}}, {1, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}}}}, {1, {{{TK_WHILE, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}, {TK_ENDWHILE, 0}}}}, {1, {{{TK_IF, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {TK_THEN, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}, {NT_NEW3, 1}, {TK_ENDIF, 0}}}}, {2, {{{TK_ELSE, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}}, {{TK_EPS, 0}}}}, {2, {{{TK_READ, 0}, {TK_OP, 0}, {NT_VAR, 1}, {TK_CL, 0}, {TK_SEM, 0}}, {{TK_WRITE, 0}, {TK_OP, 0}, {NT_VAR, 1}, {TK_CL, 0}, {TK_SEM, 0}}}}, {1, {{{NT_TERM, 1}, {NT_NEW5, 1}}}}, {2, {{{NT_OPERATOR, 1}, {NT_TERM, 1}, {NT_NEW5, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_FACTOR, 1}, {NT_NEW6, 1}}}}, {2, {{{NT_OP_H, 1}, {NT_FACTOR, 1}, {NT_NEW6, 1}}, {{TK_EPS, 0}}}}, {2, {{{NT_VAR, 1}}, {{TK_OP, 0}, {NT_ARITHMETIC_EXPRESSION, 1}, {TK_CL, 0}}}}, {2, {{{TK_PLUS, 0}}, {{TK_MINUS, 0}}}}, {2, {{{TK_MUL, 0}}, {{TK_DIV, 0}}}}, {3, {{{TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {NT_LOGICAL_OP, 1}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}}, {{NT_VAR, 1}, {NT_RELATIONAL_OP, 1}, {NT_VAR, 1}}, {{TK_NOT, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}}}}, {3, {{{NT_SINGLE_OR_REC_ID, 1}}, {{TK_NUM, 0}}, {{TK_RNUM, 0}}}}, {2, {{{TK_AND, 0}}, {{TK_OR, 0}}}}, {6, {{{TK_LT, 0}}, {{TK_LE, 0}}, {{TK_EQ, 0}}, {{TK_GT, 0}}, {{TK_GE, 0}}, {{TK_NE, 0}}}}, {1, {{{TK_RETURN, 0}, {NT_OPTIONAL_RETURN, 1}, {TK_SEM, 0}}}}, {2, {{{TK_SQL, 0}, {NT_ID_LIST, 1},
         {TK_SQR, 0}}, {{TK_EPS, 0}}}}, {1, {{{TK_ID, 0}, {NT_MORE_IDS, 1}}}}, {2, {{{TK_COMMA, 0}, {TK_ID, 0}, {NT_MORE_IDS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_DEFINETYPE, 0}, {NT_A, 1}, {TK_RUID, 0}, {TK_AS, 0}, {TK_RUID, 0}}}}, {2, {{{TK_RECORD, 0}}, {{TK_UNION, 0}}}}};

    computeFirstSet(sets_for_all, productions);
}
