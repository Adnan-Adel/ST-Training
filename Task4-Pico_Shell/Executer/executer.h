/* --------------------- Includes --------------------- */
#include"../String/my_string.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

/* --------------------- Macros Declarations --------------------- */
#define COMMANDS_LENGTH                 5
#define COMMAND_ARGS_MAX_LENGTH         10

/* --------------------- data types Declarations --------------------- */

/* --------------------- Functions Declarations --------------------- */
uint8_t is_valid_Command(char* str);

void executer(char** ar, uint32_t length);

void execute_exit(void);
void execute_cls(void);
void execute_pwd(void);
void execute_echo(char** arg, uint32_t length);
void execute_cd(char** arg, uint32_t length);