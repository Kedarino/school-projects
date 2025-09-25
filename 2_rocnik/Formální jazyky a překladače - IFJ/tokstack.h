
#ifndef TOKSTACK_H
#define TOKSTACK_H

#include <stdbool.h>
#include "scanner.h" // Assuming scanner.h contains the Token structure definition
#include "infStr.h"  // For operations with infinite strings

// Structure for token node in the stack
typedef struct TokNode {
    Token token;
    struct TokNode* next;
} TokNode;

// Structure for token stack
typedef struct {
    TokNode* top;
    int size;
} TokStack;

// Function declarations
/**
 * @brief Describe the purpose of the initTokStack function here.
 * @return Describe what is returned.
 */
TokStack* initTokStack();
/**
 * @brief Describe the purpose of the pushTok function here.
 * @param stack Describe the purpose of stack.
 * @param token Describe the purpose of token.
 */
void pushTok(TokStack* stack, Token token);
/**
 * @brief Describe the purpose of the popTok function here.
 * @param stack Describe the purpose of stack.
 * @return Describe what is returned.
 */
Token popTok(TokStack* stack);
/**
 * @brief Describe the purpose of the peekTok function here.
 * @param stack Describe the purpose of stack.
 * @return Describe what is returned.
 */
Token peekTok(const TokStack* stack);
/**
 * @brief Describe the purpose of the isEmptyTokStack function here.
 * @param stack Describe the purpose of stack.
 * @return Describe what is returned.
 */
bool isEmptyTokStack(const TokStack* stack);
/**
 * @brief Describe the purpose of the clearTokStack function here.
 * @param stack Describe the purpose of stack.
 */
void clearTokStack(TokStack* stack);
/**
 * @brief Describe the purpose of the destroyTokStack function here.
 * @param stack Describe the purpose of stack.
 */
void destroyTokStack(TokStack* stack);
/**
 * @brief Describe the purpose of the printTokStack function here.
 * @param stack Describe the purpose of stack.
 */
void printTokStack(const TokStack* stack);

#endif // TOKSTACK_H

// Additional function declarations
/**
 * @brief Describe the purpose of the copyTokStack function here.
 * @param stack Describe the purpose of stack.
 * @return Describe what is returned.
 */
TokStack* copyTokStack(const TokStack* stack);
/**
 * @brief Describe the purpose of the reverseTokStack function here.
 * @param stack Describe the purpose of stack.
 */
void reverseTokStack(TokStack* stack);
/**
 * @brief Describe the purpose of the searchTokInStack function here.
 * @param stack Describe the purpose of stack.
 * @param type Describe the purpose of type.
 * @param value Describe the purpose of value.
 * @return Describe what is returned.
 */
bool searchTokInStack(const TokStack* stack, const char* type, const char* value);
/**
 * @brief Describe the purpose of the countTokTypeInStack function here.
 * @param stack Describe the purpose of stack.
 * @param type Describe the purpose of type.
 * @return Describe what is returned.
 */
int countTokTypeInStack(const TokStack* stack, const char* type);
/**
 * @brief Describe the purpose of the mergeTokStacks function here.
 * @param dest Describe the purpose of dest.
 * @param src Describe the purpose of src.
 */
void mergeTokStacks(TokStack* dest, const TokStack* src);
