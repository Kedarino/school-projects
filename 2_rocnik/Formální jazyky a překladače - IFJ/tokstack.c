
#include <stdio.h>
#include <stdlib.h>
#include "tokstack.h"
#include <string.h>

// Initialize token stack
TokStack* initTokStack() {
    TokStack* stack = (TokStack*)malloc(sizeof(TokStack));
    if (stack == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

// Push a token onto the stack
void pushTok(TokStack* stack, Token token) {
    TokNode* newNode = (TokNode*)malloc(sizeof(TokNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->token = token; 
    newNode->next = stack->top;
    stack->top = newNode;
    stack->size++;
}

// Pop a token from the stack
Token popTok(TokStack* stack) {
    if (isEmptyTokStack(stack)) {
        fprintf(stderr, "Stack underflow\n");
        exit(EXIT_FAILURE);
    }
    TokNode* temp = stack->top;
    Token poppedToken = temp->token;
    stack->top = stack->top->next;
    free(temp);
    stack->size--;
    return poppedToken;
}

// Peek at the top token without popping it
Token peekTok(const TokStack* stack) {
    if (isEmptyTokStack(stack)) {
        fprintf(stderr, "Stack is empty\n");
        exit(EXIT_FAILURE);
    }
    return stack->top->token;
}

// Check if the stack is empty
bool isEmptyTokStack(const TokStack* stack) {
    return stack->top == NULL;
}

// Clear all elements from the stack
void clearTokStack(TokStack* stack) {
    while (!isEmptyTokStack(stack)) {
        popTok(stack);
    }
}

// Free the memory allocated for the stack
void destroyTokStack(TokStack* stack) {
    clearTokStack(stack);
    free(stack);
}

// Print all tokens in the stack for debugging purposes
void printTokStack(const TokStack* stack) {
    TokNode* temp = stack->top;
    while (temp != NULL) {
        printf("Type: %s, Value: %s\n", temp->token.type, temp->token.value);
        temp = temp->next;
    }
}

// Create a copy of the token stack
TokStack* copyTokStack(const TokStack* stack) {
    TokStack* newStack = initTokStack();
    TokNode* temp = stack->top;
    TokNode* prev = NULL, *current = NULL;
    while (temp != NULL) {
        TokNode* node = (TokNode*)malloc(sizeof(TokNode));
        if (node == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        node->token = temp->token;
        node->next = NULL;
        if (prev == NULL) {
            newStack->top = node;
            current = node;
        } else {
            current->next = node;
            current = node;
        }
        prev = node;
        temp = temp->next;
    }
    newStack->size = stack->size;
    return newStack;
}

// Reverse the order of tokens in the stack
void reverseTokStack(TokStack* stack) {
    TokNode* prev = NULL;
    TokNode* current = stack->top;
    TokNode* next = NULL;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    stack->top = prev;
}

// Search for a specific token in the stack
bool searchTokInStack(const TokStack* stack, const char* type, const char* value) {
    TokNode* temp = stack->top;
    while (temp != NULL) {
        if (strcmp(temp->token.type, type) == 0 && strcmp(temp->token.value, value) == 0) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

// Count the number of tokens of a specific type in the stack
int countTokTypeInStack(const TokStack* stack, const char* type) {
    int count = 0;
    TokNode* temp = stack->top;
    while (temp != NULL) {
        if (strcmp(temp->token.type, type) == 0) {
            count++;
        }
        temp = temp->next;
    }
    return count;
}

// Merge two token stacks
void mergeTokStacks(TokStack* dest, const TokStack* src) {
    if (src->top == NULL) return;
    TokStack* reversedSrc = copyTokStack(src);
    reverseTokStack(reversedSrc);
    TokNode* temp = reversedSrc->top;
    while (temp != NULL) {
        pushTok(dest, temp->token);
        temp = temp->next;
    }
    destroyTokStack(reversedSrc);
}

// Returns a new stack containing only tokens of a specified type
TokStack* filterTokStackByType(const TokStack* stack, const char* type) {
    TokStack* filteredStack = initTokStack();
    TokNode* temp = stack->top;
    while (temp != NULL) {
        if (strcmp(temp->token.type, type) == 0) {
            pushTok(filteredStack, temp->token);
        }
        temp = temp->next;
    }
    return filteredStack;
}

// Applies a given function to all tokens in the stack and creates a new stack with the results
TokStack* mapTokStack(const TokStack* stack, Token (*func)(const Token*)) {
    TokStack* mappedStack = initTokStack();
    TokNode* temp = stack->top;
    while (temp != NULL) {
        pushTok(mappedStack, func(&(temp->token)));
        temp = temp->next;
    }
    return mappedStack;
}

// Reduces the stack to a single value using a given reduction function
Token reduceTokStack(const TokStack* stack, Token (*func)(const Token*, const Token*)) {
    if (isEmptyTokStack(stack)) {
        fprintf(stderr, "Cannot reduce an empty stack\n");
        exit(EXIT_FAILURE);
    }
    TokNode* temp = stack->top;
    Token result = temp->token;
    temp = temp->next;
    while (temp != NULL) {
        result = func(&result, &(temp->token));
        temp = temp->next;
    }
    return result;
}

// Compares two token stacks for equality
bool equalsTokStacks(const TokStack* stack1, const TokStack* stack2) {
    TokNode* temp1 = stack1->top;
    TokNode* temp2 = stack2->top;
    while (temp1 != NULL && temp2 != NULL) {
        if (strcmp(temp1->token.type, temp2->token.type) != 0 || 
            strcmp(temp1->token.value, temp2->token.value) != 0) {
            return false;
        }
        temp1 = temp1->next;
        temp2 = temp2->next;
    }
    return temp1 == NULL && temp2 == NULL;  // Both stacks should be fully traversed to be equal
}

// Returns the number of elements in the stack with more error handling and checks
int sizeOfTokStack(const TokStack* stack) {
    if (stack == NULL) {
        fprintf(stderr, "Stack is not initialized\n");
        exit(EXIT_FAILURE);
    }
    return stack->size;
}
