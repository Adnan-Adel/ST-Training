#include <stdio.h>
#include <string.h>
    
#define STR_MAX_LENGTH     100
    
int main(void)
{   
    char in_str[STR_MAX_LENGTH];
    while(1)
    {
        printf("my prompt > ");
    
        gets(in_str);
        
        if((strcmp(in_str, "exit")) == 0)
        {
            printf("GoodBye :)\n");
            break;
        }
        else
        {
            printf("You Said: %s\n", in_str);
        }
    }
    return 0;
}   