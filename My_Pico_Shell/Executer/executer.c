#include "executer.h"
//#include <string.h>
#include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

const char* Commands[COMMANDS_LENGTH]= {"echo", "pwd", "cd", "exit", "clear"};

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
    if (is_valid_Command(arr[0]))
    {
        // Set the command
        char* command = arr[0];
        
        // If it's a command with an argument
        char** args = NULL;
        if (compare_str(command, "echo") == 1 || compare_str(command, "cd") == 1)
       	{
            args = ++arr;
        }

        if (compare_str(command, "echo") == 1) 
	{
            execute_echo(args, length);
        }
       	else if (compare_str(command, "pwd") == 1) 
	{
            execute_pwd();
        }
       	else if (compare_str(command, "cd") == 1)
       	{
            execute_cd(args, length);
        }
       	else if (compare_str(command, "exit") == 1)
       	{
            execute_exit();
	    exit(0);
        }
       	else if (compare_str(command, "clear") == 1)
       	{
            execute_clear();
        }

        // Free the command and argument
        free(command);
    } 

    else
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
		
	    Handle_redirection(arr, length);

            int ret= execve(arr[0], newargv, newenvp);
	    if(ret == -1)
	    {
	 	    printf("Not a valid command!!\n");
	    }
        }

    }
 
}

void execute_exit(void)
{
    // Execute exit command
    printf("Exit command executed\n");
    exit(0); // Exit the program
}

void execute_clear(void)
{
    system("clear");
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
    free(*arg);
}

void execute_cd(char** arg, uint32_t length)
{
    uint32_t index_i= 0;
    // Execute cd command
    if (chdir(*arg) != 0)
    {
            perror("cd");
    }
    free(*arg);
}

void Handle_redirection(char** arr, uint32_t length)
{
	uint32_t index_i= 0;
	for(index_i= 1; index_i < length; index_i++)
	{
		if(*arr[index_i] == '<')
		{
			int in_fd= open(arr[index_i+1], O_RDWR | O_CREAT);
			close(0);
			dup(in_fd);
			close(in_fd);
		}
		
		else if(*arr[index_i] == '>')
                {
                        int out_fd= open(arr[index_i+1], O_RDWR | O_CREAT, 0644);
                        close(1);
                        dup(out_fd);
                        close(out_fd);
                }
	}
}
