///@file symtab.c
///@author Radek Libra (xlibra02)
///@author Kryštof Samek (xsamek10)
///@brief Symbol table source file


#include "symtab.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>


unsigned int computeIndex(const char *key) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash << 5) + *key++;  
    }
    return hash % TABLE_SIZE;
}


SymbolTable* initSymbolTable(int level) {
    SymbolTable* newTable = (SymbolTable*) malloc(sizeof(SymbolTable));
    if (!newTable) {
        return NULL;  
    }
    
    
    for (unsigned int i = 0; i < TABLE_SIZE; i++) {
        newTable->slots[i] = NULL;
    }
    
    newTable->parentTable = NULL;  
    newTable->level = level;
    return newTable;
}


void deleteSymbolTable(SymbolTable **table) {
    if (!table || !(*table)) {
        return;
    }
    
    SymbolTable *currentTable = *table;

    for (unsigned int i = 0; i < TABLE_SIZE; i++) {
        SymbolNode *currentNode = currentTable->slots[i];
        while (currentNode) {
            SymbolNode *toFree = currentNode;
            currentNode = currentNode->nextItem;
            free(toFree->symKey);
            free(toFree);
        }
    }
    free(*table);
    *table = NULL;
}


bool addSymbol(SymbolTable* table, const char* key, SymbolDetails details) {
    unsigned int index = computeIndex(key);
    while (table->parentTable != NULL)
    {
        table = table->parentTable;
    }
    
    
    SymbolNode* currentNode = table->slots[index];
    while (currentNode) {
        if (strcmp(currentNode->symKey, key) == 0) {
            
            return false;
        }
        currentNode = currentNode->nextItem;
    }

    
    SymbolNode* newNode = (SymbolNode*) malloc(sizeof(SymbolNode));
    if (!newNode) {
        return false;  
    }
    newNode->symKey = (char *)malloc(sizeof(char)*strlen(key) + 1);
    newNode->symKey = strcpy(newNode->symKey,key);  
    
    newNode->details = details;
    newNode->details.level = table->level;
    newNode->nextItem = table->slots[index];  
    newNode->details.functionParams = details.functionParams;
    
    table->slots[index] = newNode;

    return true;
}


SymbolNode* retrieveSymbol(SymbolTable* table, const char* key) {
    if (!table || !key) {
        return NULL;
    }
    SymbolNode* node = NULL;

    if (table->parentTable != NULL){
        node = retrieveSymbol(table->parentTable, key);
    }
    
    
    if (node != NULL)
    {
        return node;
    }

    unsigned int index = computeIndex(key);
    SymbolNode* currentNode = table->slots[index];

    
    while (currentNode) {
        if (strcmp(currentNode->symKey, key) == 0) {
            return currentNode;
        }
        currentNode = currentNode->nextItem;
    }

    

    
    return node;
}


bool removeSymbol(SymbolTable* table, const char* key) {
    if (!table || !key) {
        return false;
    }

    unsigned int index = computeIndex(key);
    SymbolNode* currentNode = table->slots[index];
    SymbolNode* prevNode = NULL;

    while (currentNode) {
        if (strcmp(currentNode->symKey, key) == 0) {
            
            if (prevNode == NULL) {
                table->slots[index] = currentNode->nextItem;
            } else {
                prevNode->nextItem = currentNode->nextItem;
            }

            free(currentNode->symKey);
            free(currentNode);
            return true;
        }
        prevNode = currentNode;
        currentNode = currentNode->nextItem;
    }

    return false;
}


void addLVLSymbolTable(SymbolTable* table){
    if (table == NULL)
    {
        return;
    }
    
    while (table->parentTable != NULL)
    {
        table = table->parentTable;
    }
    table->parentTable = initSymbolTable(table->level + 1);
}


void deleteLVLSymbolTable(SymbolTable* table){
    if (table == NULL)
    {
        return;
    }
    SymbolTable* prev = NULL;
    while (table->parentTable != NULL)
    {
        prev = table;
        table = table->parentTable;
    }
    if (prev)
    {
        prev->parentTable = NULL;
    }
    deleteSymbolTable(&table);
}


int getTopLVL(SymbolTable* table){
    if (table == NULL)
    {
        return -1;
    }
    while (table->parentTable != NULL)
    {

        table = table->parentTable;
    }
    return table->level;

}


void initDet(SymbolDetails *det){
    det->dataType = 0;
    det->fncparams = NULL;
    det->functionParams = NULL;
    det->isDefined = false;
    det->IsGlobal = false;
    det->level = 0;
    det->symName = NULL;
    det->varlet = VAR;
}