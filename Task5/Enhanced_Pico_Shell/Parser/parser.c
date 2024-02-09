#include "parser.h"

void Tokenizer(char* src)
{
    uint32_t left = 0, right = 0;
    uint32_t len = str_length(src);
    uint32_t index_i= 0;
    char* command = NULL;
    char** arg = NULL;

    char* com_arg[10];
    
    /* Look For White space */
    while(right < len)
    {
        if(src[right] != ' ' && src[right] != '\n')
        {
            right++;
        }
        else if(src[right] == ' ' || src[right] == '\n')
        {
            char* substr = substring(src, left, right - 1); 
            left = right + 1; // Move left to the next character after the whitespace
            right++;
            com_arg[index_i]= substr;
            index_i++;
            //free(substr);  //Free allocated memory
        }
    }
    
    executer(com_arg, index_i);
}

