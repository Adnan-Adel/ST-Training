#include "parser.h"

const char* Commands[35]= {"echo", "pwd", "cd", "exit"};

uint8_t is_valid_Command(char* str)
{
    if(NULL == str)
    {
        return 0;
    }
    else
    {
        uint8_t ret_flag= is_str_in_arr(str, Commands, COMMANDS_LENGTH);
        return ret_flag;
    }
}

void Tokenizer(char* src)
{
    uint32_t left = 0, right = 0;
    uint32_t len = str_length(src);
    char* command = NULL;
    char* arg = NULL;
    
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

            if(is_valid_Command(substr) == 1)
            {
                if (command != NULL) 
                {
                    // Free previous command
                    free(command);
                    command = NULL;
                }
                // Set the command
                command = substr;
                
                // If it's a command with an argument
                if (compare_str(command, "echo") == 1 || compare_str(command, "cd") == 1)
                {
                    // Extract the argument
                    arg = substring(src, left, len - 1);
                    break;
                }
                else
                {
                    // For commands like pwd and exit, execute directly without extracting arguments
                    break;
                }
            }
            free(substr); // Free allocated memory
        }
    }
    
    if (command == NULL) 
    {
        printf("No command found\n");
        return;
    }
    
    if (compare_str(command, "echo") == 1) 
    {
        // Execute echo command
        printf("Echo command executed with argument: %s\n", arg);
        // Free the command and argument
        free(command);
        free(arg);
    }
    else if (compare_str(command, "pwd") == 1) 
    {
        // Execute pwd command
        printf("PWD command executed\n");
        system("pwd");
        // Free the command
        free(command);
    }
    else if (compare_str(command, "cd") == 1)
    {
        // Execute cd command
        printf("CD command executed with argument: %s\n", arg);
        // Change the directory
        if (chdir(arg) != 0) 
        {
            perror("chdir() error");
        }
        // Free the command and argument
        free(command);
        free(arg);
    } 
    else if (compare_str(command, "exit") == 1)
    {
        // Execute exit command
        printf("Exit command executed\n");
        // Free the command
        free(command);
        exit(0); // Exit the program
    } 
    else 
    {
        printf("Unknown command: %s\n", command);
        // Free the command
        free(command);
    }
}

