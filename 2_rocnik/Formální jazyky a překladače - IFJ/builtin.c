#include "builtin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

char* readString() {
    const size_t bufferSize = 1024;
    char* buffer = malloc(bufferSize);
    if (buffer == NULL) {
        return NULL;
    }

    if (fgets(buffer, bufferSize, stdin) == NULL) {
        free(buffer);
        return NULL;
    }

    // Remove newline character if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    // Optionally resize the buffer to the actual string length
    char* line = realloc(buffer, len + 1);
    if (line == NULL) {
        free(buffer);
        return NULL;
    }

    return line;
}

// Reads an integer from standard input
int* readInt() {
    char* line = readString();
    if (!line) {
        return NULL;
    }

    int* value = (int*) malloc(sizeof(int));
    if (sscanf(line, "%d", value) != 1) {
        free(value);
        value = NULL;
    }

    free(line);
    return value;
}

// Reads a double from standard input
double* readDouble() {
    char* line = readString();
    if (!line) {
        return NULL;
    }

    double* value = (double*) malloc(sizeof(double));
    if (sscanf(line, "%lf", value) != 1) {
        free(value);
        value = NULL;
    }

    free(line);
    return value;
}

// Writes terms to standard output
void write(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

// Converts integer to double
double Int2Double(int term) {
    return (double)term;
}

// Converts double to integer
int Double2Int(double term) {
    return (int)term;
}

// Returns length of string
int length(const char* s) {
    return strlen(s);
}

// Returns a substring
char* substring(const char* s, int startingAt, int endingBefore) {
    if (startingAt < 0 || endingBefore < 0 || startingAt > endingBefore || startingAt >= length(s) || endingBefore > length(s)) {
        return NULL;
    }

    char* result = (char*) malloc(endingBefore - startingAt + 1);
    strncpy(result, &s[startingAt], endingBefore - startingAt);
    result[endingBefore - startingAt] = '\0';

    return result;
}

// Returns ASCII value of first character in string
int ord(const char* c) {
    if (!c || strlen(c) == 0) {
        return 0;
    }

    return (int)c[0];
}

// Returns a string from an ASCII value
char* chr(int i) {
    if (i < 0 || i > 255) {
        return NULL;
    }

    char* result = (char*) malloc(2);
    result[0] = (char)i;
    result[1] = '\0';

    return result;
}
