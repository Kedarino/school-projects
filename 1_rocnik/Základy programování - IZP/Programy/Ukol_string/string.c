#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *str_init = "Hello World!";

int find_substr(char *str, char *substr)
{
    for(int i = 0; str[i] !='\0'; i++)
    {
        for (int j = 0; str[i + j] ==substr[j]; j++)
        {
            if (substr[j + 1] == '\0')
            {
                return i;
            }
        }
    }
    return -1;
}
void replace_same_length (char *str, char *substr, char *new_substr)
{
    int position = find_substr(str, substr);
    if (position == -1)
    {
        return;
    }
    for (int i = 0; new_substr[i] != '\0'; i++)
    {
        str[position + i] = new_substr[i];
    }
}
char *replace(char *str, char *substr, char *new_substr)
{
    int position = find_substr(str, substr);
    if (position == -1)
    {
        return NULL;
    }
    int str_len = strlen(str);
    int substr_len = strlen(substr);
    int new_substr_len = strlen(new_substr);
    int len_diff = substr_len - new_substr_len;

    if (len_diff > 0)
    {
        int i;
        for( i = position + substr_len; str[i] != '\0'; i++)
        {
            str[i - len_diff] = str[i];
        }
        str[i - len_diff] = '\0';
        char *new_str = (char *)realloc(str, str_len - len_diff + 1);
        if (new_str == NULL)
        {
            return NULL;
        }
        str = new_str;
    }
    if (len_diff < 0)
    {
        char *new_str = (char *)realloc(str, str_len - len_diff +1);
        if (new_str == NULL)
        {
            return NULL;
        }
        str = new_str;
        
        for(int i = str_len - len_diff; i >= position + substr_len; i--)
        {
            str[i] = str[i + len_diff];
        }
    }

    for (int i = 0; new_substr[i] != '\0'; i++)
    {
        str[position + i] = new_substr[i];
    }
    return str;
    
}
int main()
{
 char *str = (char *)malloc (strlen(str_init)+ 1);
    if(str == NULL)
    {
        return 1;
    }
 strcpy(str, str_init);
 replace_same_length(str, "World!", "worlds");
 printf("%s\n", str);

 str = replace(str, "worlds", "IZP!");
    if (str == NULL)
    {
        return 1;
    }
 printf("%s\n", str);

 str = replace(str, "IZP!", "World!");
    if (str == NULL)
    {
        return 1;
    }
 printf("%s\n", str);

 free(str);
 printf("successfully replaced all substrings!\n");
    if (str == NULL)
    {
        return 1;
    }
 str = replace (str, "worlds", "World!");
    if(str == NULL) 
    {
        free(str);
        return 1;
    }

return 0;
}