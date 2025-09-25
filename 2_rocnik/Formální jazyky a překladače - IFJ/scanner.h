#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char* type;
    char* value;
} Token;

char* append_char_to_string(char* original, char to_append);

char* append_char_to_string_times(char* original, char to_append, int times);

char* validate_string(char* string, bool *valid);

bool is_type(char *identifier);

bool is_keyword(char *identifier);

Token *create_token(char** type, char** lexeme);

void print_token(Token *token);

void free_token(Token* token);

int get_token(char** type, char **value);

typedef enum STATE{
    START,
    UNDERSCORE,
    IDENTIFIER,
    INT_LITERAL,
    DOUBLE_LITERAL,
    DOUBLE_LITERAL_WITH_E,
    DOUBLE_LITERAL_DOT,
    DOUBLE_LITERAL_E,
    STRING,
    STRING_SLASH,
    SLASH,
    LINE_COMMENT,
    FIRST_QUOTES,
    SECOND_QUOTES,
    THIRD_QUOTES,
    MULTILINE_STR,
    MULTILINE_STR_SAFE,
    MULTILINE_STR_NEWLINE,
    MULTILINE_STR_QMARK_ERR1,
    MULTILINE_STR_QMARK_ERR2,
    MULTILINE_STR_QMARK_ERR3,
    MULTILINE_STR_QMARK1,
    MULTILINE_STR_QMARK2,
    MULTILINE_STR_QMARK3,
    BLOCK_COMMENT_OUTER,
    BLOCK_COMMENT_OUTER_END,
    BLOCK_COMMENT_INNER_START,
    BLOCK_COMMENT_INNER,
    BLOCK_COMMENT_INNER_END,
    DASH,
    LESS_THAN,
    GREATER_THAN,
    QUEST_MARK,
    EXCL_MARK,
    EQUALS,
} STATE;