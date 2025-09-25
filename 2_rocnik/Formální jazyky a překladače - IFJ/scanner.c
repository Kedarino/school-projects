#include "scanner.h"

char* append_char_to_string(char* original, char to_append){
    size_t original_len = (original == NULL) ? 0 : strlen(original);
    char* new_string = (char*)malloc((original_len + 2) * sizeof(char));
    if (original != NULL) {
        strcpy(new_string, original);
        free(original);
    }
    new_string[original_len] = to_append;
    new_string[original_len + 1] = '\0';

    return new_string;
}

char* append_char_to_string_times(char* original, char to_append, int times) {
    char* new = original;

    for (int i = 0; i < times; ++i) {
        new = append_char_to_string(new, to_append);
    }

    return new;
}


bool is_type(char *identifier){
   if (!strcmp(identifier,"Double")){
      return true;
   } else if (!strcmp(identifier,"String")){
      return true;
   } else if (!strcmp(identifier,"Int")) {
      return true;
   }
   return false;
}

bool is_keyword(char *identifier){
   if (!strcmp(identifier,"if")){
      return true;
   } else if (!strcmp(identifier,"else")){
      return true;
   } else if (!strcmp(identifier,"func")) {
      return true;
   } else if (!strcmp(identifier,"let")) {
      return true;
   } else if (!strcmp(identifier,"nil")) {
      return true;
   } else if (!strcmp(identifier,"var")) {
      return true;
   } else if (!strcmp(identifier,"while")) {
      return true;
   }
   return false;
}

Token* create_token(char** type, char** lexeme) {
    Token* token = (Token*)malloc(sizeof(Token));
    if (token == NULL) {
        exit(99);
    }

    token->type = *type;

    token->value = (char*)malloc(strlen(*lexeme) + 1);
    if (token->value == NULL) {
        free(token);
        exit(99);
    }
    strcpy(token->value, *lexeme);

    free(*lexeme);

    return token;
}


void free_token(Token* token) {
    if (token != NULL) {
         if (token->value!=NULL){
            free(token->value);
         }
         free(token);
    }
}

void print_token(Token *token){
   printf("Typ tokenu: %s, hodnota tokenu: %s\n",token->type,token->value);
}

char* validate_string(char* string, bool *valid){
   char* new=NULL;
   for (int i=0;i<strlen(string);++i){
      if (string[i]=='\\'){
         ++i;
         if (i<strlen(string)){
            switch (string[i]){
               case 'n':
                  new=append_char_to_string(new,'\n');
                  break;
               case 'r':
                  new=append_char_to_string(new,'\r');
                  break;
               case 't':
                  new=append_char_to_string(new,'\t');
                  break;
               case '0':
                  new=append_char_to_string(new,'\0');
                  break;
               case '\\':
                  new=append_char_to_string(new,'\\');
                  break;
               case '"':
                  new=append_char_to_string(new,'"');
                  break;
               case '\'':
                  new=append_char_to_string(new,'\\');
                  break;
               case 'u':
                  //TODO: Converting \u{dd} needed
                  *valid=false;
                  free(string);
                  free(new);
                  return "\0";
                  break;
               default:
                  *valid=false;
                  free(string);
                  free(new);
                  return "\0";
            }
         } else {
               *valid=false;
               free(string);
               free(new);
               return "\0";
         }
      } else {
         new=append_char_to_string(new,string[i]);
      }
   }
   *valid=true;
   free(string);
   return new;
}

int get_token(char** type, char **value){
    int c;
    STATE state=START;
    *value=NULL;

    while ((c = fgetc(stdin) ) != EOF) {
      char chr=(char) c;
      switch (state){
         case START:
            if (isalpha(chr)){
               *value=append_char_to_string(*value,chr);
               state=IDENTIFIER;
               continue;
            } else if (isdigit(chr)){
               *value=append_char_to_string(*value,chr);
               state=INT_LITERAL;
               continue;
            } else if (isspace(chr)){
               continue;
            }
            else {
               switch (chr){
                  case '(':
                     *type="(";
                     *value=append_char_to_string(*value,'(');
                     return 0;
                  case ')':
                     *type=")";
                     *value=append_char_to_string(*value,')');
                     return 0;
                  case '{':
                     *type="{";
                     *value=append_char_to_string(*value,'{');
                     return 0;
                  case '}':
                     *type="}";
                     *value=append_char_to_string(*value,'}');
                     return 0;
                  case '+':
                     *type="+";
                     *value=append_char_to_string(*value,'+');
                     return 0;
                  case ':':
                     *type=":";
                     *value=append_char_to_string(*value,':');
                     return 0;
                  case '*':
                     *type="*";
                     *value=append_char_to_string(*value,'*');
                     return 0;
                  case '.':
                     *type=".";
                     *value=append_char_to_string(*value,'.');
                     return 0;
                  case ',':
                     *type=",";
                     *value=append_char_to_string(*value,',');
                     return 0;
                  case '!':
                     state=EXCL_MARK;
                     *value=append_char_to_string(*value,'!');
                     continue;
                  case '-':
                     state=DASH;
                     *value=append_char_to_string(*value,'-');
                     continue;
                  case '?':
                     state=QUEST_MARK;
                     *value=append_char_to_string(*value,'?');
                     continue;
                  case '/':
                     state=SLASH;
                     *value=append_char_to_string(*value,'/');
                     continue;
                  case '"':
                     state=FIRST_QUOTES;
                     continue;
                  case '_':
                     state=UNDERSCORE;
                     *value=append_char_to_string(*value,'_');
                     continue;
                  case '=':
                     state=EQUALS;
                     *value=append_char_to_string(*value,'=');
                     continue;
                  case '>':
                     state=LESS_THAN;
                     *value=append_char_to_string(*value,'>');
                     continue;
                  case '<':
                     state=GREATER_THAN;
                     *value=append_char_to_string(*value,'<');
                     continue;
                  default:
                     printf("ERROR\n");
                     continue;
                  }
            }
         case FIRST_QUOTES:
            if (chr=='"'){
               state=SECOND_QUOTES;
               continue;
            } else if (chr=='\n'){
               return 1;
            } else if (chr=='\\'){
               state=STRING_SLASH;
               *value=append_char_to_string(*value,chr);
               *type="string";
               continue;
            } else {
               state=STRING;
               *value=append_char_to_string(*value,chr);
               *type="string";
               continue;
            }
         case STRING:
            if (chr=='"'){
               bool valid=false;
               *value=validate_string(*value,&valid);
               if (valid){
                  return 0;
               }
               return 1;
            } else if (chr=='\n'){
               return 1;
            } else if (chr=='\\') {
               state=STRING_SLASH;
            }
            *value=append_char_to_string(*value,chr);
            continue;
         case STRING_SLASH:
            if (chr=='\n'){
               return 1;
            } else if (chr=='\\') {
               state=STRING_SLASH;
               *value=append_char_to_string(*value,chr);
               continue; 
            } else {
               state=STRING;
               *value=append_char_to_string(*value,chr);
               continue;
            }
         case SECOND_QUOTES:
            if (chr=='"'){
               state=THIRD_QUOTES;
               continue;
            } else {
               *type="string";
               *value=append_char_to_string(*value,'\0');
               return 0;
            }
         case THIRD_QUOTES:
            if (chr=='\n'){
               *type="string";
               state=MULTILINE_STR_NEWLINE;
            } else if (chr=='"') {
               *type="string";
               *value=append_char_to_string(*value,'\0');
               return 0;
            } else {
               *type="string";
               *value=append_char_to_string(*value,chr);
               state=STRING;
            } 
            continue;
         case MULTILINE_STR_NEWLINE:
            if (chr=='"'){
               state=MULTILINE_STR_QMARK1;
            } else if (chr=='\n') {
               *value=append_char_to_string(*value,chr);
               continue;
            } else {
               state=MULTILINE_STR;
               *value=append_char_to_string(*value,chr);
            }
            continue;
         case MULTILINE_STR:
            if (chr=='"'){
               state=MULTILINE_STR_QMARK_ERR1;
            } else if (chr=='\n') {
               state=MULTILINE_STR_NEWLINE;
               *value=append_char_to_string(*value,chr);
            } else {
               *value=append_char_to_string(*value,chr);
            }
            continue;
         case MULTILINE_STR_SAFE:
            if (chr=='"'){
               *value=append_char_to_string(*value,chr);
            } else if (chr=='\n') {
               state=MULTILINE_STR_NEWLINE;
               *value=append_char_to_string(*value,chr);
            } else {
               *value=append_char_to_string(*value,chr);
               state=MULTILINE_STR;
            }
            continue;
         case MULTILINE_STR_QMARK1:
            if (chr=='"'){
               state=MULTILINE_STR_QMARK2;
            } else if (chr=='\n') {
               state=MULTILINE_STR_NEWLINE;
               *value=append_char_to_string(*value,'"');
               *value=append_char_to_string(*value,chr);
            } else {
               state=MULTILINE_STR;
               *value=append_char_to_string(*value,'"');
               *value=append_char_to_string(*value,chr);
            }
            continue;
         case MULTILINE_STR_QMARK2:
            if (chr=='"'){
               state=MULTILINE_STR_QMARK3;
            } else if (chr=='\n') {
               state=MULTILINE_STR_NEWLINE;
               *value=append_char_to_string_times(*value,'"',2);
               // *value=append_char_to_string(*value,'"');
               // *value=append_char_to_string(*value,'"');
               *value=append_char_to_string(*value,chr);
            } else {
               state=MULTILINE_STR;
               *value=append_char_to_string_times(*value,'"',2);
               // *value=append_char_to_string(*value,'"');
               // *value=append_char_to_string(*value,'"');
               *value=append_char_to_string(*value,chr);
            }
            continue;
         case MULTILINE_STR_QMARK3:
            if (chr=='"'){
               *value=append_char_to_string_times(*value,'"',3);
               // for (int i=0;i<=3;++i){
               //    *value=append_char_to_string(*value,'"');
               // }
               state=MULTILINE_STR_SAFE;   
            } else if (chr=='\n') {
               *value[strlen(*value)-1]='\0';
               bool valid=false;
               *value=validate_string(*value,&valid);
               if (valid){
                  return 0;
               }
               return 1;
            } else {
               return 1;
            }
            continue;
         case MULTILINE_STR_QMARK_ERR1:
            if (chr=='"'){
               state=MULTILINE_STR_QMARK_ERR2;
               continue;
            } else if (chr=='\n'){
               state=MULTILINE_STR_NEWLINE;
            } else {
               state=MULTILINE_STR;
            }
            *value=append_char_to_string(*value,'"');
            *value=append_char_to_string(*value,chr);
            continue;
         case MULTILINE_STR_QMARK_ERR2:
            if (chr=='"'){
               state=MULTILINE_STR_QMARK_ERR3;
               continue;
            } else if (chr=='\n'){
               state=MULTILINE_STR_NEWLINE;
            } else {
               state=MULTILINE_STR;
            }
            *value=append_char_to_string_times(*value,'"',2);
            *value=append_char_to_string(*value,chr);
            continue;
         case MULTILINE_STR_QMARK_ERR3:
            if (chr=='"'){
               *value=append_char_to_string_times(*value,'"',3);
               state=MULTILINE_STR_SAFE;
               continue;
            } else {
               return 1;
            }
            continue;
         case IDENTIFIER:
            if (isalpha(chr) || isdigit(chr) || chr=='_'){
               *value=append_char_to_string(*value,chr);
               continue;
            } else if (chr=='?'){
               if (is_type(*value)){
                  *value=append_char_to_string(*value,chr);
                  *type="type";
                  return 0;
               } else {
                  *type="identifier";
                  ungetc(c,stdin);
                  return 0;
               }
            } else {
               if (is_keyword(*value)){
                  *type="keyword";
               } else {
                  *type="identifier";
               }
               ungetc(c,stdin);
               return 0;
            }
         case UNDERSCORE:
            if (isalpha(chr) || isdigit(chr) || chr=='_'){
               *value=append_char_to_string(*value,chr);
               state=IDENTIFIER;
               continue;
            } else {
               ungetc(c,stdin);
               return 0;
            }
         case INT_LITERAL:
            if (isdigit(chr)){
               *value=append_char_to_string(*value,chr);
            } else if (chr=='.') {
               *value=append_char_to_string(*value,chr);
               state=DOUBLE_LITERAL_DOT;
            } else if (chr=='e'||chr=='E'){
               *value=append_char_to_string(*value,chr);
               state=DOUBLE_LITERAL_E;
            } else {
               *type="int";
               ungetc(c,stdin);
               return 0;
            }
            continue;
         case DOUBLE_LITERAL_DOT:
            if (isdigit(chr)){
               *value=append_char_to_string(*value,chr);
               state=DOUBLE_LITERAL;
            } else {
               printf("ERROR\n");
               state=START;
            }
            continue;
         case DOUBLE_LITERAL:
            if (isdigit(chr)){
               *value=append_char_to_string(*value,chr);
            } else if (chr=='e' || chr=='E'){
               *value=append_char_to_string(*value,chr);
               state=DOUBLE_LITERAL_E;
            } else {
               *type="double";
               ungetc(c,stdin);
               return 0;
            }
            continue;
         case DOUBLE_LITERAL_E:
            if (isdigit(chr)){
               *value=append_char_to_string(*value,chr);
               state=DOUBLE_LITERAL_WITH_E;
            } else if (chr=='+' || chr=='-'){
               *value=append_char_to_string(*value,chr);
               state=DOUBLE_LITERAL_WITH_E;
            } else {
               printf("ERROR\n");
               state=START;
            }
            continue;
         case DOUBLE_LITERAL_WITH_E:
            if (isdigit(chr)){
               *value=append_char_to_string(*value,chr);
               continue;
            } else {
               *type="double";
               ungetc(c,stdin);
               return 0;
            }
         case DASH:
            if (chr=='>'){
               *value=append_char_to_string(*value,chr);
               *type="->";
            } else {
               *type="-";
               ungetc(c,stdin);
            }
            return 0;
         case LESS_THAN:
            if (chr=='='){
               *value=append_char_to_string(*value,chr);
               *type=">=";
            } else {
               *type=">";
               ungetc(c,stdin);
            }
            return 0;
         case GREATER_THAN:
            if (chr=='='){
               *value=append_char_to_string(*value,chr);
               *type=">=";
            } else {
               *type=">";
               ungetc(c,stdin);
            }
            return 0;
         case EQUALS:
            if (chr=='='){
               *value=append_char_to_string(*value,chr);
               *type="==";
            } else {
               *type="=";
               ungetc(c,stdin);
            }
            return 0;
         case EXCL_MARK:
            if (chr=='='){
               *value=append_char_to_string(*value,chr);
               *type="!=";
            } else {
               *type="!";
               ungetc(c,stdin);
            }
            return 0;
         case QUEST_MARK:
            if (chr=='?'){
               *value=append_char_to_string(*value,chr);
               *type="??";
               return 0;
            } else {
               return 1;
            }
         case SLASH:
            if (chr=='/'){
               state=LINE_COMMENT;
               free(*value);
               *value=NULL;
            } else if (chr=='*'){
               state=BLOCK_COMMENT_OUTER;
               free(*value);
               *value=NULL;
            } else {
               *type="/";
               ungetc(c,stdin);
               return 0;
            }
            continue;
         case LINE_COMMENT:
            if (chr=='\n'){
               state=START;
            }
            continue;
         case BLOCK_COMMENT_OUTER:
            if (chr=='/'){
               state=BLOCK_COMMENT_INNER_START;
               continue;
            } else if (chr=='*'){
               state=BLOCK_COMMENT_OUTER_END;
               continue;
            } else {
               continue;
            }
         case BLOCK_COMMENT_OUTER_END: /**/
            if (chr=='/'){
               state=START;
               continue;
            } else {
               state=BLOCK_COMMENT_OUTER;
            }
         case BLOCK_COMMENT_INNER_START:
            if (chr=='*'){
               state=BLOCK_COMMENT_INNER;
               continue;
            } else {
               state=BLOCK_COMMENT_OUTER;
               continue;
            }
         case BLOCK_COMMENT_INNER:
            if (chr=='*') {
               state=BLOCK_COMMENT_INNER_END;
               continue;
            } else {
               continue;
            }
         case BLOCK_COMMENT_INNER_END:
            if (chr=='/') {
               state=BLOCK_COMMENT_OUTER;
               continue;
            } else {
               continue;
            }
      }
   }
   return -1;
}