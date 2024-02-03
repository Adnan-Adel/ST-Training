#include <stdio.h>
#include "string/my_string.h"

#define STR_MAX_LENGTH     100

const char* commands[5]= {"cp", "echo", "pwd", "cat", "exit"};

int main(int argc, char** argv)
{
    char in_str[STR_MAX_LENGTH];
    while(1)
    {
        printf("my prompt > ");

        fgets(in_str, STR_MAX_LENGTH, stdin);
	uint32_t len= str_length(in_str);
	if(in_str[len - 1] == '\n')
	{
		in_str[len - 1]= '\0';
	}


        if((compare_str(in_str, "exit")) == 1)
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

