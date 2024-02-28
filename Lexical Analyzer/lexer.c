#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"

typedef struct SymbolTableEntry *tokenInfo;

FILE *getStream(FILE *fp);

tokenInfo getNextToken(twinBuffer B);

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