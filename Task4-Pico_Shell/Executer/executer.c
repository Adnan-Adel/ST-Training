#include "executer.h"

const char* Commands[COMMANDS_LENGTH]= {"echo", "pwd", "cd", "exit", "cls"};

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

void executer(char** arr, uint32_t length)
{
    uint32_t index_i= 1;
    char* command;
    char** args;


    if(is_valid_Command(arr[0]) == 1)
    {
        // Set the command
        command = arr[0];
        
        // If it's a command with an argument
        if (compare_str(command, "echo") == 1 || compare_str(command, "cd") == 1)
        {
            printf(">>>>>>>> I'm a command with an argument\n");
            // Extract the argument
            args= (char**)(++arr);
        }
        else
        {
            printf(">>>>>>>> I got no arguments\n");
            // For commands like pwd and exit, execute directly without extracting arguments
        }

        if (command == NULL) 
        {
            printf("No command found\n");
            return;
        }

        if (compare_str(command, "echo") == 1) 
        {
            execute_echo(args, length);

            // Free the command and argument
            free(command);
        }
        else if (compare_str(command, "pwd") == 1) 
        {
            execute_pwd();

            // Free the command
            free(command);
        }
        else if (compare_str(command, "cd") == 1)
        {
            execute_cd(args, length);

            // Free the command and argument
            free(command);
        } 
        else if (compare_str(command, "exit") == 1)
        {
            execute_exit();

            free(command);
            exit(0); // Exit the program
        } 
        else if(compare_str(command, "cls") == 1)
        {
            execute_cls();

            free(command);
        }
    }

    /*else
    {
        int ret_pid= fork();

        if(ret_pid < 0)
            printf("Fork Failed\n");
        
        else if(ret_pid > 0)
        {
            int status;
            wait(&status);
        }
        
        else if(ret_pid == 0)
        {
            char* newargv[]={NULL};
            char* newenvp[]={NULL};

            execve(command, newargv, newenvp);
        }
    }*/
 
}

void execute_exit(void)
{
    // Execute exit command
    printf("Exit command executed\n");
    exit(0); // Exit the program
}

void execute_cls(void)
{
    system("cls");
}

void execute_pwd(void)
{
    // Execute pwd command
    printf("PWD command executed\n");
    system("pwd");
}

void execute_echo(char** arg, uint32_t length)
{
    uint32_t index_i= 0;
    // Execute echo command
   for(index_i= 0; index_i< length-1; index_i++)
   {
        printf("%s ", arg[index_i]);
   }
    printf("\n");
}

void execute_cd(char** arg, uint32_t length)
{
    uint32_t index_i= 0;
    // Execute cd command
    for(index_i= 0; index_i< length-1; index_i++)
    {
        printf("%s ", arg[index_i]);
    }
    printf("\n");
}