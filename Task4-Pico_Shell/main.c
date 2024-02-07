#include <stdio.h>
#include "Parser/parser.h"

#define STR_MAX_LENGTH     100
    
int main()
{   
    char in_str[STR_MAX_LENGTH];
    
    while(1)
    {
        printf("my prompt > ");
    
        fgets(in_str, STR_MAX_LENGTH, stdin);
    
        Tokenizer(in_str);
    }

    return 0;
}   