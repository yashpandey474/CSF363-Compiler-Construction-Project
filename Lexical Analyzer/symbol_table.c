// MAKE A HASH TABLE AS THE SYMBOL TABLE
#include "lexical.h"
#include <stdio.h>
#include <string.h>
struct SymbolTableNode *symbolTable[HASH_MAP_SIZE] = {NULL};

// STORE LEXEMES IN A SEPARATE DATA STRUCTURE (?)

// ADD THE KEYWORDS <TOKEN, POINTER TO LEXEME>

// WRITE FUNCTIONS FOR INSERT (TOKEN, LEXEME) AND LOOKUP (LEXEME)

int hash(const char *lexeme)
{
    // can write functions to rehash when reached 70% of the size
    int hash = 0;
    while (*lexeme)
    {
        hash = hash * 31 + *(lexeme++);
        hash = hash % HASH_MAP_SIZE;
    }
    return hash % HASH_MAP_SIZE;
}

struct SymbolTableEntry *lookup(const char *lexeme)
{
    // GET THE INDEX
    int index = hash(lexeme);
    printf("LOOKING %s", lexeme);

    // GET THE NODE: SEARCH IN HASH TABLE
    printf(" at Index: %d\n", index);

    // for (int ii = 0; ii < HASH_MAP_SIZE; ii++)
    // {
    //     printf("%x ", symbolTable[ii]->entry);
    // }
    // printf("\n");

    struct SymbolTableNode *nodeFound = symbolTable[index];
    // printf("Symbol table index: %d\n Symbol table node: %p Symbol table entry lexeme %s\n", index, nodeFound, nodeFound->entry->lexeme);

    // CHECK IF EXISTS
    if (nodeFound == NULL)
    {
        printf("Lexeme %s not found\n", lexeme);
        return NULL;
    }

    else
    {
        // LINEAR SEARCH IN LINKED LIST
        while (nodeFound)
        {
            if (strcmp(lexeme, nodeFound->entry->lexeme) == 0)
                return nodeFound->entry;

            else
                nodeFound = nodeFound->next;
        }
        return NULL;
    }
}

struct SymbolTableEntry *installId(const char *lexeme)
{
    return lookup(lexeme);
}

struct SymbolTableNode *createNode(struct SymbolTableEntry *symbolTableEntry, struct SymbolTableNode *next)
{
    struct SymbolTableNode *newNode = (struct SymbolTableNode *)malloc(sizeof(struct SymbolTableNode));
    newNode->next = next;
    newNode->entry = symbolTableEntry;
    return newNode;
}
struct SymbolTableEntry *insertTester(char *lexeme, enum Tokentype token)
{
    struct SymbolTableEntry *symbolTableEntry = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
    symbolTableEntry->lexeme = lexeme;
    symbolTableEntry->tokenType = token;
    symbolTableEntry = getToken(symbolTableEntry);

    return symbolTableEntry;
}

struct SymbolTableEntry *getToken(struct SymbolTableEntry *symbolTableEntry)
{
    printf("Inserting %s\n", symbolTableEntry->lexeme);
    char *lexeme = symbolTableEntry->lexeme;
    struct SymbolTableEntry *exists = lookup(lexeme);
    printf(">> %s %p\n", lexeme, exists);
    if (exists == NULL)
    {
        // insert into the symbol table using the lexeme as the hash tabel
        // return the new entry
        int lexeme_hash = hash(lexeme);
        struct SymbolTableNode *next = symbolTable[lexeme_hash];
        symbolTable[lexeme_hash] = createNode(symbolTableEntry, next);
        return symbolTable[lexeme_hash]->entry;
    }
    else
    {
        // ignore
        // return the existing symbol table entry

        printf("Lexeme %s already exists\n", lexeme);
        printf("MAXVERSTAPPEN");
        printf("DUM DUM%d\n", exists->tokenType);
        return exists;
    }
}

void printSymbolTable()
{
    printf("\nPrinting symbol table\n");
    int collisionCount = 0;
    int entriesCount = 0;
    for (int i = 0; i < HASH_MAP_SIZE; i++)
    {
        printf("%d --->", i);
        if (symbolTable[i] != NULL)
        {
            struct SymbolTableNode *node = symbolTable[i];
            while (node)
            {
                entriesCount++;
                printf("%s of enum tokentype %d --->", node->entry->lexeme, node->entry->tokenType);
                node = node->next;
                if (node != NULL)
                    collisionCount++;
            }
            printf("NULL\n");
        }
        else
        {
            printf("NULL\n");
        }
    }
    printf("Total entries: %d\n", entriesCount);
    printf("Total collisions: %d\n", collisionCount);
    printf("Hash map size: %d\n", HASH_MAP_SIZE);
}

void insertAllKeywords()
{
    insertTester("with", TK_WITH);
    insertTester("parameters", TK_PARAMETERS);
    insertTester("end", TK_END);
    insertTester("while", TK_WHILE);
    insertTester("union", TK_UNION);
    insertTester("endunion", TK_ENDUNION);
    insertTester("definetype", TK_DEFINETYPE);
    insertTester("as", TK_AS);
    insertTester("type", TK_TYPE);
    insertTester("_main", TK_MAIN);
    insertTester("global", TK_GLOBAL);
    insertTester("parameter", TK_PARAMETER);
    insertTester("list", TK_LIST);
    insertTester("input", TK_INPUT);
    insertTester("output", TK_OUTPUT);
    insertTester("int", TK_INT);
    insertTester("real", TK_REAL);
    insertTester("endwhile", TK_ENDWHILE);
    insertTester("if", TK_IF);
    insertTester("then", TK_THEN);
    insertTester("endif", TK_ENDIF);
    insertTester("read", TK_READ);
    insertTester("write", TK_WRITE);
    insertTester("return", TK_RETURN);
    insertTester("call", TK_CALL);
    insertTester("record", TK_RECORD);
    insertTester("endrecord", TK_ENDRECORD);
    insertTester("else", TK_ELSE);
}

// // TEST SYMBOL TABLE
// int main()
// {

//     // INSERT ALL KEYWORDS
//     insertAllKeywords();
//     insertTester("!=", TK_NE);
//     printSymbolTable();
// }