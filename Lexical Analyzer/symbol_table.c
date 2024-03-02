// MAKE A HASH TABLE AS THE SYMBOL TABLE
#include "lexer.h"
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
    // printf("LOOKING %s", lexeme);

    // GET THE NODE: SEARCH IN HASH TABLE
    // printf(" at Index: %d\n", index);

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
        // printf("Lexeme %s not found\n", lexeme);
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
const char *CharacterTypeToString(enum CharacterType type)
{
    switch (type)
    {
    case CT_UNDERSCORE:
        return "CT_UNDERSCORE";
    case CT_INVALID:
        return "CT_INVALID";
    case CT_LETTER_UPPER:
        return "CT_LETTER_UPPER";
    case CT_LETTER_LOWER_EXCEPT_ID_LETTER:
        return "CT_LETTER_LOWER_EXCEPT_ID_LETTER";
    case CT_DIGIT_EXCEPT_ID_DIGIT:
        return "CT_DIGIT_EXCEPT_ID_DIGIT";
    case CT_DELIMITER:
        return "CT_DELIMITER";
    case CT_GREATER:
        return "CT_GREATER";
    case CT_LESSER:
        return "CT_LESSER";
    case CT_EQUAL:
        return "CT_EQUAL";
    case CT_COMMA:
        return "CT_COMMA";
    case CT_SEM:
        return "CT_SEM";
    case CT_COLON:
        return "CT_COLON";
    case CT_DOT:
        return "CT_DOT";
    case CT_ROUND_OPEN:
        return "CT_ROUND_OPEN";
    case CT_ROUND_CLOSE:
        return "CT_ROUND_CLOSE";
    case CT_SQUARE_OPEN:
        return "CT_SQUARE_OPEN";
    case CT_SQUARE_CLOSE:
        return "CT_SQUARE_CLOSE";
    case CT_PLUS:
        return "CT_PLUS";
    case CT_MINUS:
        return "CT_MINUS";
    case CT_MUL:
        return "CT_MUL";
    case CT_DIV:
        return "CT_DIV";
    case CT_AND:
        return "CT_AND";
    case CT_OR:
        return "CT_OR";
    case CT_EXCLAMATION:
        return "CT_EXCLAMATION";
    case CT_TILDE:
        return "CT_TILDE";
    case CT_PERCENT:
        return "CT_PERCENT";
    case CT_AT_THE_RATE:
        return "CT_AT_THE_RATE";
    case CT_ID_LETTER:
        return "CT_ID_LETTER";
    case CT_ID_DIGIT:
        return "CT_ID_DIGIT";
    case CT_EXPONENT:
        return "CT_EXPONENT";
    case CT_EOF:
        return "CT_EOF";
    case CT_HASH:
        return "CT_HASH";

    case CT_CARRIAGE_RETURN:
        return "CT_CARRIAGE_RETURN";
    // Handle more types if needed
    default:
        return "Unknown CharacterType";
    }
}

const char *TokenToString(int token)
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
    case TK_LT2:
    case TK_LT:
        return "TK_LT";
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
        return "TK_EPS";
    case TK_EOF:
        return "TK_EOF";
    case LEXICAL_ERROR:
        return "LEXICAL_ERROR";
    case -1:
        return "SYN TOKEN";

    default:
        return "non accept state";
    }
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
    // printf("Inserting %s\n", symbolTableEntry->lexeme);
    char *lexeme = symbolTableEntry->lexeme;
    // printf("\nThe lexeme received in gettoken is ");
    // for(int i=0;i<strlen(lexeme);i++)
    // {
    //     printf("%d ",lexeme[i]);
    // }
    struct SymbolTableEntry *exists = lookup(lexeme);
    // printf(">> %s %p\n", lexeme, exists);
    if (exists == NULL)
    {
        // insert into the symbol table using the lexeme as the hash tabel
        // return the new entry
        int lexeme_hash = hash(lexeme);
        struct SymbolTableNode *next = symbolTable[lexeme_hash];
        symbolTable[lexeme_hash] = createNode(symbolTableEntry, next);
        return symbolTable[lexeme_hash]->entry;
    }
    return symbolTableEntry;
}

void printSymbolTable()
{
    printf("\n\n\nPrinting symbol table\n");
    int collisionCount = 0;
    int entriesCount = 0;
    for (int i = 0; i < HASH_MAP_SIZE; i++)
    {
        printf("%d. ", i);
        if (symbolTable[i] != NULL)
        {
            struct SymbolTableNode *node = symbolTable[i];
            while (node)
            {
                entriesCount++;
                printf("%s (%s)--->", node->entry->lexeme, TokenToString(node->entry->tokenType));
                node = node->next;
                if (node != NULL)
                {
                    collisionCount++;
                }
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