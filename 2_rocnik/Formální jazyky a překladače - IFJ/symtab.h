#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdbool.h>
#include "infStr.h"
#include "node.h"


#define TABLE_SIZE 101  

// Possible data categories for symbols
typedef struct SymbolTableStruct SymbolTable;

// Holds details related to a specific symbol
typedef struct
{
    Type dataType;      // Category of the data
    bool isDefined;               // True if the symbol is defined
    Node *functionParams;      // Function parameters if symbol is a function, using infStr type
    char *symName;                // Name associated with the symbol
    int level;                    // Level for scoping, inherits from table
    Var_Let varlet;
    SymbolTable *fncparams;
    bool IsGlobal;
} SymbolDetails;

// Node structure for symbol entries
typedef struct symbol_node
{
    char *symKey;                  // Key for the symbol (typically the name)
    SymbolDetails details;         // Symbol details
    struct symbol_node *nextItem;  // Next item in case of collisions
} SymbolNode;


// Definition of the struct
struct SymbolTableStruct
{
    int level;                      // Level of scoping
    SymbolNode* slots[TABLE_SIZE];  // Array of pointers for the symbols
    SymbolTable* parentTable;       // Pointer to the parent table (for scoping)
};



// Function prototypes for symbol table operations
unsigned int computeIndex(const char *key);   // Computes table index based on key
SymbolTable* initSymbolTable(int level);               // Initializes an empty symbol table
void deleteSymbolTable(SymbolTable** table);  // Deallocates memory for the symbol table
bool removeSymbol(SymbolTable* table, const char* key); //Removes symbol based on its key
bool addSymbol(SymbolTable* table, const char* key, SymbolDetails details);  // Adds a symbol to the table
SymbolNode* retrieveSymbol(SymbolTable* table, const char* key);    // Retrieves a symbol based on its key
void addLVLSymbolTable(SymbolTable* table);
void deleteLVLSymbolTable(SymbolTable* table);
int getTopLVL(SymbolTable* table);
#endif // SYMBOL_TABLE_H
