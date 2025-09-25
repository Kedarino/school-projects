#include "symtab.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void testInitAndDelete() {
    printf("Testing table initialization and deletion...\n");
    SymbolTable *table = initSymbolTable();
    assert(table != NULL && "Initialization failed.");
    
    deleteSymbolTable(table);
    printf("Initialization and deletion test passed.\n\n");
}

void testAddAndRetrieveSymbol() {
    printf("Testing adding and retrieving symbols...\n");
    SymbolTable *table = initSymbolTable();

    // Add a global symbol
    SymbolDetails globalSymbol = {STRING_TYPE, true, NULL, "globalSymbol", true};
    assert(addSymbol(table, "globalSymbol", globalSymbol) && "Failed to add global symbol.");
    
    // Retrieve the global symbol
    SymbolNode *retrieved = retrieveSymbol(table, "globalSymbol");
    assert(retrieved != NULL && "Retrieval of global symbol failed.");
    assert(strcmp(retrieved->details.symName, "globalSymbol") == 0 && "Retrieved global symbol has incorrect name.");

    deleteSymbolTable(table);
    printf("Adding and retrieving symbols test passed.\n\n");
}

void testDuplicateSymbols() {
    printf("Testing duplicate symbol handling...\n");
    SymbolTable *table = initSymbolTable();

    // Add a symbol
    SymbolDetails symbol = {STRING_TYPE, true, NULL, "symbol", false};
    assert(addSymbol(table, "symbol", symbol) && "Failed to add symbol.");

    // Attempt to add a duplicate symbol
    assert(!addSymbol(table, "symbol", symbol) && "Duplicate symbol was added incorrectly.");

    deleteSymbolTable(table);
    printf("Duplicate symbol test passed.\n\n");
}

void testGlobalSymbol() {
    printf("Testing global symbol functionality...\n");
    SymbolTable *table = initSymbolTable();

    // Add a global symbol
    SymbolDetails globalSymbol = {STRING_TYPE, true, NULL, "globalVar", true};
    addSymbol(table, "globalVar", globalSymbol);

    // Verify global flag
    SymbolNode *retrieved = retrieveSymbol(table, "globalVar");
    assert(retrieved->details.isGlobal && "Global symbol is not marked as global.");

    deleteSymbolTable(table);
    printf("Global symbol test passed.\n\n");
}

void testFunctionParameters() {
    printf("Testing function parameter handling...\n");
    SymbolTable *table = initSymbolTable();

    // Create an InfStr and set its value
    InfStr *params = infStrCreate();
    if (!params) {
        printf("Failed to create InfStr for parameters.\n");
        deleteSymbolTable(table);
        return;
    }
    if (!infStrCopyFromCStr(params, "int, double")) {
        printf("Failed to set InfStr value.\n");
        infStrFree(params);
        deleteSymbolTable(table);
        return;
    }
    

    // Add a function symbol with parameters
    SymbolDetails functionSymbol = {STRING_TYPE, true, params, "function", false};
    addSymbol(table, "function", functionSymbol);

    // Verify function parameters
    SymbolNode *retrieved = retrieveSymbol(table, "function");
    assert(retrieved != NULL && "Retrieval of function symbol failed.");
    assert(infStrCompareWithCStr(retrieved->details.functionParams, "int, double") == 0 && "Function parameters are incorrect.");

    // Clean up
    infStrFree(params);
    
    deleteSymbolTable(table);
    
    printf("Function parameter test passed.\n\n");
    
}

void testDefinedAndUndefinedSymbols() {
    printf("Testing defined and undefined symbols...\n");
    SymbolTable *table = initSymbolTable();

    // Add an undefined symbol
    SymbolDetails undefinedSymbol = {STRING_TYPE, false, NULL, "undefinedSymbol", false};
    addSymbol(table, "undefinedSymbol", undefinedSymbol);

    // Verify isDefined flag
    SymbolNode *retrieved = retrieveSymbol(table, "undefinedSymbol");
    assert(!retrieved->details.isDefined && "Undefined symbol is marked as defined.");

    deleteSymbolTable(table);
    printf("Defined and undefined symbols test passed.\n\n");
}

void testRemoveSymbol() {
    printf("Testing symbol removal...\n");
    SymbolTable *table = initSymbolTable();

    // Add and then remove a symbol
    SymbolDetails symbol = {STRING_TYPE, true, NULL, "tempSymbol", false};
    addSymbol(table, "tempSymbol", symbol);
    assert(removeSymbol(table, "tempSymbol") && "Failed to remove symbol.");

    // Verify removal
    assert(retrieveSymbol(table, "tempSymbol") == NULL && "Symbol was not removed correctly.");

    deleteSymbolTable(table);
    printf("Symbol removal test passed.\n\n");
}



int main() {
    testInitAndDelete();
    testAddAndRetrieveSymbol();
    testDuplicateSymbols();
    testGlobalSymbol();
    testFunctionParameters();
    testDefinedAndUndefinedSymbols();
    testRemoveSymbol();
    

    printf("All tests passed successfully.\n");
    return 0;
}