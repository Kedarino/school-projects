#ifndef BUILTIN_FUNCTIONS_H
#define BUILTIN_FUNCTIONS_H

#include <stdbool.h>

// Built-in functions for reading literals and outputting terms
char* readString();
int* readInt();
double* readDouble();
void write(const char* format, ...); // Using variadic function for multiple terms

// Built-in functions for converting number types
double Int2Double(int term);
int Double2Int(double term);

// Built-in functions for working with strings
int length(const char* s);
char* substring(const char* s, int startingAt, int endingBefore);
int ord(const char* c);
char* chr(int i);

#endif // BUILTIN_FUNCTIONS_H