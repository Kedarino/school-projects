#include "infStr.h"
#include <stdlib.h>
#include <string.h>

InfStr* infStrCreate() {
    InfStr *s = (InfStr *)malloc(sizeof(InfStr));
    if (!s) return NULL;
    
    s->length = 0;
    s->data = (char *)calloc(1, sizeof(char)); // Allocating and initializing with null terminator
    if (!s->data) {
        free(s);
        return NULL;
    }
    
    return s;
}

void infStrFree(InfStr *s) {
    if (s) {
        free(s->data);
        free(s);
    }
}

void infStrClear(InfStr *s) {
    if (s && s->data) {
        s->length = 0;
        s->data[0] = '\0';
    }
}

bool infStrAdd(InfStr *s, char c) {
    if (!s) return false;
    
    char *newData = (char *)realloc(s->data, (s->length + 2) * sizeof(char));
    if (!newData) return false;

    s->data = newData;
    s->data[s->length++] = c;
    s->data[s->length] = '\0';

    return true;
}

bool infStrCopy(InfStr *dest, const InfStr *src) {
    if (!dest || !src) return false;

    char *newData = (char *)realloc(dest->data, (src->length + 1) * sizeof(char));
    if (!newData) return false;

    dest->data = newData;
    strcpy(dest->data, src->data);
    dest->length = src->length;

    return true;
}

int infStrCompare(const InfStr *s1, const InfStr *s2) {
    if (!s1 || !s2) return -1;
    return strcmp(s1->data, s2->data);
}

int infStrCompareWithCStr(const InfStr *s, const char *cstr) {
    if (!s || !cstr) return -1;
    return strcmp(s->data, cstr);
}

bool infStrCopyFromCStr(InfStr *s, const char *cstr) {
    if (!s || !cstr) return false;
    
    int cstrLen = strlen(cstr);
    char *newData = (char *)realloc(s->data, (cstrLen + 1) * sizeof(char));
    if (!newData) return false;

    s->data = newData;
    strcpy(s->data, cstr);
    s->length = cstrLen;

    return true;
}

const char *infStrGetCStr(const InfStr *s) {
    return s ? s->data : NULL;
}

int infStrGetLength(const InfStr *s) {
    return s ? s->length : -1;
}

bool infStrConcat(InfStr *s1, const InfStr *s2) {
    if (!s1 || !s2) return false;

    int newLen = s1->length + s2->length;
    char *newData = (char *)realloc(s1->data, (newLen + 1) * sizeof(char));
    if (!newData) return false;

    s1->data = newData;
    strcat(s1->data, s2->data);
    s1->length = newLen;

    return true;
}

bool infStrInsert(InfStr *s, int index, char c) {
    if (!s || index < 0 || index > s->length) return false;

    char *newData = (char *)realloc(s->data, (s->length + 2) * sizeof(char));
    if (!newData) return false;

    s->data = newData;
    memmove(&s->data[index + 1], &s->data[index], s->length - index + 1);  // +1 to include null terminator
    s->data[index] = c;
    s->length++;

    return true;
}

char infStrCharAt(const InfStr *s, int index) {
    if (!s || index < 0 || index >= s->length) return '\0';
    return s->data[index];
}