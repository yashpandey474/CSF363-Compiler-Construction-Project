// MAKE A HASH TABLE AS THE SYMBOL TABL
#define hashMapSize 69;
struct SymbolTable{
    
}
//EACH ENTRY IN THE SYMBOL TABLE: HASH ON THE LEXEME
struct symbolTableEntry{
    char* lexeme;
    enum Tokentype token;
};

int hash(const char *lexeme) {
    unsigned int hash = 0;
    while (*lexeme) {
        hash = hash * 31 + *(lexeme++);
    }
    return hash % hashMapSize;
}    

int lookup(struct SymbolTable* symboltableonst char *lexeme) {
    int index = hash(key);
    while (hashMap[index].key != NULL) {
        if (strcmp(hashMap[index].key, key) == 0) {
            return hashMap[index].value;
        }
        index = (index + 1) % hashMapSize;
    }
    return -1;
}
// STORE LEXEMES IN A SEPARATE DATA STRUCTURE (?)

// ADD THE KEYWORDS <TOKEN, POINTER TO LEXEME>

// WRITE FUNCTIONS FOR INSERT (TOKEN, LEXEME) AND LOOKUP (LEXEME)

