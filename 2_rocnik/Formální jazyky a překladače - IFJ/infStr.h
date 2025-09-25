#ifndef INFSTR_H
#define INFSTR_H

#include <stdbool.h>

typedef struct {
    char *data;
    int length;
} InfStr;

/**
 * @brief Creates a new infinite string.
 * @return A pointer to a newly allocated InfStr structure, or NULL if the allocation fails.
 */
InfStr* infStrCreate();

/**
 * @brief Frees the memory occupied by the infinite string.
 * @param s The pointer to the InfStr structure to be freed.
 */
void infStrFree(InfStr *s);

/**
 * @brief Clears the content of the infinite string, making its length zero.
 * @param s The pointer to the InfStr structure to be cleared.
 */
void infStrClear(InfStr *s);

/**
 * @brief Adds a character to the end of the infinite string.
 * @param s The pointer to the InfStr where the character will be added.
 * @param c The character to add.
 * @return true if the character is successfully added, false otherwise.
 */
bool infStrAdd(InfStr *s, char c);

/**
 * @brief Copies the content of one infinite string to another.
 * @param dest The pointer to the destination InfStr.
 * @param src The pointer to the source InfStr.
 * @return true if the string is successfully copied, false otherwise.
 */
bool infStrCopy(InfStr *dest, const InfStr *src);

/**
 * @brief Compares two infinite strings lexicographically.
 * @param s1 The first string to compare.
 * @param s2 The second string to compare.
 * @return <0 if s1<s2, 0 if s1==s2, >0 if s1>s2, -1 if either string is NULL.
 */
int infStrCompare(const InfStr *s1, const InfStr *s2);

/**
 * @brief Compares an infinite string with a C string lexicographically.
 * @param s The pointer to the InfStr.
 * @param cstr The pointer to the C string.
 * @return <0 if s<cstr, 0 if s==cstr, >0 if s>cstr, -1 if either string is NULL.
 */
int infStrCompareWithCStr(const InfStr *s, const char *cstr);

/**
 * @brief Copies a C string into an infinite string.
 * @param s The pointer to the InfStr where the C string will be copied.
 * @param cstr The pointer to the C string to copy.
 * @return true if the C string is successfully copied, false otherwise.
 */
bool infStrCopyFromCStr(InfStr *s, const char *cstr);

/**
 * @brief Retrieves the C string from an infinite string.
 * @param s The pointer to the InfStr to convert.
 * @return A pointer to the C string, or NULL if the InfStr is NULL.
 */
const char *infStrGetCStr(const InfStr *s);

/**
 * @brief Retrieves the length of an infinite string.
 * @param s The pointer to the InfStr to get the length.
 * @return The length of the string, or -1 if the InfStr is NULL.
 */
int infStrGetLength(const InfStr *s);

/**
 * @brief Concatenates one infinite string to another.
 * @param s1 The pointer to the InfStr where the second string will be appended.
 * @param s2 The pointer to the InfStr to append.
 * @return true if the string is successfully concatenated, false otherwise.
 */
bool infStrConcat(InfStr *s1, const InfStr *s2);

/**
 * @brief Inserts a character at a specific position in the infinite string.
 * @param s The pointer to the InfStr where the character will be inserted.
 * @param index The position where the character will be inserted.
 * @param c The character to insert.
 * @return true if the character is successfully inserted, false otherwise.
 */
bool infStrInsert(InfStr *s, int index, char c);

/**
 * @brief Retrieves the character at a specific position in the infinite string.
 * @param s The pointer to the InfStr to retrieve the character from.
 * @param index The position of the character to retrieve.
 * @return The character at the specified position, or '\0' if the index is out of bounds or InfStr is NULL.
 */
char infStrCharAt(const InfStr *s, int index);

#endif // INFSTR_H