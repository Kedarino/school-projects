#include "scanner.h"

int main(){
    int code=0;
    Token *token;
    while (code==0){
        char *type, *value=NULL;
        code=get_token(&type,&value);
        if (code==0){
            token=create_token(&type,&value);
            print_token(token);
            free_token(token);
        } else if (code==-1){
            printf("END OF FILE\n");
            exit(0);
        } else if (code==1) {
            free(value);
            printf("LEX ERROR\n");
            exit(1);
        } else {
            exit(2);
        }
    }
}