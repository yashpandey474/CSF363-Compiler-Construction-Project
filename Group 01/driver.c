
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

void remove_comments(char *testcasefile, char *cleanFile);
void print_token_list(char *filename);
void print_timing_info(clock_t start_time, clock_t end_time);

void remove_comments(char *testcasefile, char *cleanFile)
{
    FILE *file = fopen(testcasefile, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Open an additional file for writing the output
    FILE *outputFile = fopen(cleanFile, "w");
    if (outputFile == NULL)
    {
        perror("Error opening output file");
        fclose(file);
        return;
    }

    char buffer[50];
    int prev_char = '\0'; // Store the previous character read

    while (fgets(buffer, 50, file) != NULL)
    {
        for (int i = 0; buffer[i] != '\0'; i++)
        {
            if (prev_char == '%' && buffer[i] != '\n')
            {
                continue;
            }

            if (prev_char == '%' && buffer[i] == '\n')
            {
                printf("%c", buffer[i]);
                fprintf(outputFile, "%c", buffer[i]);
            }
            else if (buffer[i] == '\n')
            {
                printf("%c", buffer[i]);
                fprintf(outputFile, "%c", buffer[i]);
            }
            else if (buffer[i] != '%')
            {
                printf("%c", buffer[i]);
                fprintf(outputFile, "%c", buffer[i]);
            }
            prev_char = buffer[i];
        }
    }

    fclose(file);
    fclose(outputFile);
}

void print_token_list(char *filename)
{

    // printf("KEYWORDS\n");

    // TEST THE TWIN BUFFER
    FILE *file = readTestFile(filename);

    // INITIALISE A TWIN BUFFER
    twinBufferArray bufferArray = initialiseTwinBuffer(file);

    // INITIALISE LA
    twinBuffer LA = initialiseLA(bufferArray);

    // printf("LA INITIALISED\n");

    // START SCANNING
    getStream(bufferArray);

    // printf("READ INPUT\n");

    // THE TOKEN
    struct SymbolTableEntry *token;

    // printf("STARTING SCANNING\n");
    FILE *outputFile = fopen("lexicaltest.txt", "w");

    if (outputFile == NULL)
    {
        perror("Error opening output file");
        fclose(file);
        return;
    }

    while ((token = getNextToken(LA)))
    {
        if (token->tokenType == LEXICAL_ERROR)
        {
            printf("Line no. %-5d Error: %s\n", LA->lineNo, token->lexeme);
            fprintf(outputFile, "Line no. %-5d Error: %s\n", LA->lineNo, token->lexeme);
        }
        else
        {
            printf("Line no. %-5d Lexeme %-30s Token %-s\n", LA->lineNo, token->lexeme, TokenToString(token->tokenType));
            fprintf(outputFile, "Line no. %-5d Lexeme %-30s Token %-s\n", LA->lineNo, token->lexeme, TokenToString(token->tokenType));
        }
    }
    fclose(outputFile);
}

void print_timing_info(clock_t start_time, clock_t end_time)
{
    double total_CPU_time, total_CPU_time_in_seconds;

    total_CPU_time = (double)(end_time - start_time);
    total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

    printf("Total CPU time: %f μs\n", total_CPU_time);
    printf("Time in seconds: %f s\n", total_CPU_time_in_seconds);
}

int main(int argc, char *argv[])
{
    int option;
    clock_t start_time, end_time;
    printf("FIRST and FOLLOW set automated\n");
    printf("Lexical,Syntax analyzer module developed\n");
    printf("Parse Tree module developed\n");
    
    if (argc < 3)
    {
        printf("Please provide in format %s <inputfile> <outputfile>\n", argv[0]);
        return 1;
    }
    insertAllKeywords();
    int synchSet[] = {
        TK_ENDRECORD, TK_ENDUNION, TK_SEM, TK_DOT, TK_CL, TK_OP};
    Grammar G = {{{1, {{{NT_OTHER_FUNCTIONS, 1}, {NT_MAIN_FUNCTION, 1}}}}, {1, {{{TK_MAIN, 0}, {NT_STMTS, 1}, {TK_END, 0}}}}, {2, {{{NT_FUNCTION, 1}, {NT_OTHER_FUNCTIONS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_FUNID, 0}, {NT_INPUT_PAR, 1}, {NT_OUTPUT_PAR, 1}, {TK_SEM, 0}, {NT_STMTS, 1}, {TK_END, 0}}}}, {1, {{{TK_INPUT, 0}, {TK_PARAMETER, 0}, {TK_LIST, 0}, {TK_SQL, 0}, {NT_PARAMETER_LIST, 1}, {TK_SQR, 0}}}}, {2, {{{TK_OUTPUT, 0}, {TK_PARAMETER, 0}, {TK_LIST, 0}, {TK_SQL, 0}, {NT_PARAMETER_LIST, 1}, {TK_SQR, 0}}, {{TK_EPS, 0}}}}, {1, {{{NT_DATA_TYPE, 1}, {TK_ID, 0}, {NT_REMAINING_LIST, 1}}}}, {2, {{{NT_PRIMITIVE_DATA_TYPE, 1}}, {{NT_CONSTRUCTED_DATA_TYPE, 1}}}}, {2, {{{TK_INT, 0}}, {{TK_REAL, 0}}}}, {2, {{{NT_A, 1}, {TK_RUID, 0}}, {{TK_RUID, 0}}}}, {2, {{{TK_COMMA, 0}, {NT_PARAMETER_LIST, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_TYPE_DEFINITIONS, 1}, {NT_DECLARATIONS, 1}, {NT_OTHER_STMTS, 1}, {NT_RETURN_STMT, 1}}}}, {3, {{{NT_TYPE_DEFINITION, 1}, {NT_TYPE_DEFINITIONS, 1}}, {{NT_DEFINETYPE_STMT, 1}, {NT_TYPE_DEFINITIONS, 1}}, {{TK_EPS, 0}}}}, {2, {{{TK_RECORD, 0}, {TK_RUID, 0}, {NT_FIELD_DEFINITIONS, 1}, {TK_ENDRECORD, 0}}, {{TK_UNION, 0}, {TK_RUID, 0}, {NT_FIELD_DEFINITIONS, 1}, {TK_ENDUNION, 0}}}}, {1, {{{NT_FIELD_DEFINITION, 1}, {NT_FIELD_DEFINITION, 1}, {NT_MORE_FIELDS, 1}}}}, {1, {{{TK_TYPE, 0}, {NT_DATA_TYPE, 1}, {TK_COLON, 0}, {TK_FIELDID, 0}, {TK_SEM, 0}}}}, {2, {{{NT_FIELD_DEFINITION, 1}, {NT_MORE_FIELDS, 1}}, {{TK_EPS, 0}}}}, {2, {{{NT_DECLARATION, 1}, {NT_DECLARATIONS, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_TYPE, 0}, {NT_DATA_TYPE, 1}, {TK_COLON, 0}, {TK_ID, 0}, {NT_GLOBAL_OR_NOT, 1}, {TK_SEM, 0}}}}, {2, {{{TK_COLON, 0}, {TK_GLOBAL, 0}}, {{TK_EPS, 0}}}}, {2, {{{NT_STMT, 1}, {NT_OTHER_STMTS, 1}}, {{TK_EPS, 0}}}}, {5, {{{NT_ASSIGNMENT_STMT, 1}}, {{NT_ITERATIVE_STMT, 1}}, {{NT_CONDITIONAL_STMT, 1}}, {{NT_IO_STMT, 1}}, {{NT_FUN_CALL_STMT, 1}}}}, {1, {{{NT_SINGLE_OR_REC_ID, 1}, {TK_ASSIGNOP, 0}, {NT_ARITHMETIC_EXPRESSION, 1}, {TK_SEM, 0}}}}, {1, {{{TK_ID, 0}, {NT_REC_ID, 1}}}}, {2, {{{TK_DOT, 0}, {TK_FIELDID, 0}, {NT_REC_ID, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_OUTPUT_PARAMETERS, 1}, {TK_CALL, 0}, {TK_FUNID, 0}, {TK_WITH, 0}, {TK_PARAMETERS, 0}, {NT_INPUT_PARAMETERS, 1}, {TK_SEM, 0}}}}, {2, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}, {TK_ASSIGNOP, 0}}, {{TK_EPS, 0}}}}, {1, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}}}}, {1, {{{TK_WHILE, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}, {TK_ENDWHILE, 0}}}}, {1, {{{TK_IF, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {TK_THEN, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}, {NT_NEW3, 1}, {TK_ENDIF, 0}}}}, {2, {{{TK_ELSE, 0}, {NT_STMT, 1}, {NT_OTHER_STMTS, 1}}, {{TK_EPS, 0}}}}, {2, {{{TK_READ, 0}, {TK_OP, 0}, {NT_VAR, 1}, {TK_CL, 0}, {TK_SEM, 0}}, {{TK_WRITE, 0}, {TK_OP, 0}, {NT_VAR, 1}, {TK_CL, 0}, {TK_SEM, 0}}}}, {1, {{{NT_TERM, 1}, {NT_NEW5, 1}}}}, {2, {{{NT_OPERATOR, 1}, {NT_TERM, 1}, {NT_NEW5, 1}}, {{TK_EPS, 0}}}}, {1, {{{NT_FACTOR, 1}, {NT_NEW6, 1}}}}, {2, {{{NT_OP_H, 1}, {NT_FACTOR, 1}, {NT_NEW6, 1}}, {{TK_EPS, 0}}}}, {2, {{{NT_VAR, 1}}, {{TK_OP, 0}, {NT_ARITHMETIC_EXPRESSION, 1}, {TK_CL, 0}}}}, {2, {{{TK_PLUS, 0}}, {{TK_MINUS, 0}}}}, {2, {{{TK_MUL, 0}}, {{TK_DIV, 0}}}}, {3, {{{TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}, {NT_LOGICAL_OP, 1}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}}, {{NT_VAR, 1}, {NT_RELATIONAL_OP, 1}, {NT_VAR, 1}}, {{TK_NOT, 0}, {TK_OP, 0}, {NT_BOOLEAN_EXPRESSION, 1}, {TK_CL, 0}}}}, {3, {{{NT_SINGLE_OR_REC_ID, 1}}, {{TK_NUM, 0}}, {{TK_RNUM, 0}}}}, {2, {{{TK_AND, 0}}, {{TK_OR, 0}}}}, {6, {{{TK_LT, 0}}, {{TK_LE, 0}}, {{TK_EQ, 0}}, {{TK_GT, 0}}, {{TK_GE, 0}}, {{TK_NE, 0}}}}, {1, {{{TK_RETURN, 0}, {NT_OPTIONAL_RETURN, 1}, {TK_SEM, 0}}}}, {2, {{{TK_SQL, 0}, {NT_ID_LIST, 1}, {TK_SQR, 0}}, {{TK_EPS, 0}}}}, {1, {{{TK_ID, 0}, {NT_MORE_IDS, 1}}}}, {2, {{{TK_COMMA, 0}, {NT_ID_LIST, 1}}, {{TK_EPS, 0}}}}, {1, {{{TK_DEFINETYPE, 0}, {NT_A, 1}, {TK_RUID, 0}, {TK_AS, 0}, {TK_RUID, 0}}}}, {2, {{{TK_RECORD, 0}}, {{TK_UNION, 0}}}}}};
    FirstAndFollow *sets = computeFirstAndFollow(G.productions);
    printf("FIRST and FOLLOW set automated\n");
    printf("Lexical analyzer module developed\n");
    struct ParsingTable *PT = (struct ParsingTable *)malloc(sizeof(struct ParsingTable));
    createParseTable(PT, G.productions, sets, synchSet, sizeof(synchSet) / sizeof(synchSet[0]));

    // printParsingTable(PT);

    do
    {
        printf("\nOptions:\n");
        printf("0 : Exit\n");
        printf("1 : Remove comments\n");
        printf("2 : Print token list\n");
        printf("3 : Parse and print parse tree\n");
        printf("4 : Print timing information\n");

        printf("Selected option: ");
        scanf("%d", &option);

        switch (option)
        {
        case 0:
            printf("Exiting...\n");
            break;
        case 1:
            remove_comments(argv[1], "comment_free_code.txt");
            break;
        case 2:
            print_token_list(argv[1]);
            break;
        case 3:
            print_and_parse_tree(argv[1], argv[2], sets, PT, G);
            break;
        case 4:
            start_time = clock();
            print_token_list(argv[1]);
            print_and_parse_tree(argv[1], argv[2], sets, PT, G);
            end_time = clock();
            print_timing_info(start_time, end_time);
            break;
        default:
            printf("Invalid option. Please try again.\n");
        }
    } while (option != 0);

    return 0;
}
