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
    unsigned int hash = 0;
    while (*lexeme)
    {
        hash = hash * 31 + *(lexeme++);
    }
    return hash % HASH_MAP_SIZE;
}

struct SymbolTableEntry *lookup(const char *lexeme)
{
    // GET THE INDEX
    int index = hash(lexeme);

    // GET THE NODE
    struct SymbolTableNode *nodeFound = symbolTable[index];

    // CHECK IF EXISTS
    if (nodeFound == NULL)
    {
        return NULL;
    }

    else
    {
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

struct SymbolTableNode *createNode(char *lexeme, enum Tokentype token, struct SymbolTableNode *next)
{
    struct SymbolTableEntry *newEntry = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
    struct SymbolTableNode *newNode = (struct SymbolTableNode *)malloc(sizeof(struct SymbolTableNode));

    newEntry->tokenType = token;
    newNode->next = next;
    newEntry->lexeme = strdup(lexeme);
    newNode->entry = newEntry;
    return newNode;
}

struct SymbolTableEntry *insert(char *lexeme, enum Tokentype token)
{
    struct SymbolTableEntry *exists = lookup(lexeme);
    if (exists == NULL)
    {
        // insert into the symbol table using the lexeme as the hash tabel
        // return the new entry
        int lexeme_hash = hash(lexeme);

        struct SymbolTableNode *next = symbolTable[lexeme_hash];

        symbolTable[lexeme_hash] = createNode(lexeme, token, next);

        return symbolTable[lexeme_hash]->entry;
    }
    else
    {
        // ignore
        // return the existing symbol table entry
        return exists;
    }
}

void printSymbolTable()
{
    for (int i = 0; i < HASH_MAP_SIZE; i++)
    {
        printf("%d --->", i);
        if (symbolTable[i] != NULL)
        {
            struct SymbolTableNode *node = symbolTable[i];
            while (node)
            {
                printf("%s of enum tokentype %d --->", node->entry->lexeme, node->entry->tokenType);
                node = node->next;
            }
            printf("NULL\n");
        }
        else
        {
            printf("NULL\n");
        }
    }
}

void insertAllKeywords()
{

    insert("=", TK_ASSIGNOP);
    insert("%f%%", TK_COMMENT);
    insert("with", TK_WITH);
    insert("parameters", TK_PARAMETERS);
    insert("end", TK_END);
    insert("while", TK_WHILE);
    insert("union", TK_UNION);
    insert("endunion", TK_ENDUNION);
    insert("definetype", TK_DEFINETYPE);
    insert("as", TK_AS);
    insert("type", TK_TYPE);
    insert("_main", TK_MAIN);
    insert("global", TK_GLOBAL);
    insert("parameter", TK_PARAMETER);
    insert("list", TK_LIST);
    insert("input", TK_INPUT);
    insert("output", TK_OUTPUT);
    insert("int", TK_INT);
    insert("real", TK_REAL);
    insert("endwhile", TK_ENDWHILE);
    insert("if", TK_IF);
    insert("then", TK_THEN);
    insert("endif", TK_ENDIF);
    insert("read", TK_READ);
    insert("write", TK_WRITE);
    insert("return", TK_RETURN);
    insert("call", TK_CALL);
    insert("record", TK_RECORD);
    insert("endrecord", TK_ENDRECORD);
    insert("else", TK_ELSE);
    insert(",", TK_COMMA);
    insert(";", TK_SEM);
    insert(":", TK_COLON);
    insert(".", TK_DOT);
    insert("(", TK_OP);
    insert(")", TK_CL);
    insert("+", TK_PLUS);
    insert("-", TK_MINUS);
    insert("*", TK_MUL);
    insert("/", TK_DIV);
    insert("&&&", TK_AND);
    insert("@@@", TK_OR);
    insert("~", TK_NOT);
    insert("<", TK_LT);
    insert("<=", TK_LE);
    insert("==", TK_EQ);
    insert(">", TK_GT);
    insert(">=", TK_GE);
    insert("!=", TK_NE);
}

// TEST SYMBOL TABLE
int main()
{

    // INSERT ALL KEYWORDS
    insertAllKeywords();
    printSymbolTable();
}