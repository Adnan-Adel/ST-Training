#include "executer.h"
#include "alias.h"
#include "environment.h"
#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 10

int is_builtin_command(char** args) {
  if (strcmp(args[0], "cd") == 0) {
    if (args[1]) {
      if (chdir(args[1]) != 0) {
        perror("cd failed");
      }
    }
    else {
      printf("cd: missing argument\n");
    }
    return 1;
  }
  else if (strcmp(args[0], "history") == 0) {
    show_history();
    return 1;
  }
  else if (strcmp(args[0], "alias") == 0) {
    if (args[1] && args[2]) {
      add_alias(args[1], args[2]);
    }
    else {
      printf("Usage: alias <name> <command>\n");
    }
    return 1;
  }
  return 0;
}

int parse_and_execute(char* input, char** args) {
  char* token = strtok(input, " ");
  int index = 0;

  // Tokenize the input into arguments
  while (token != NULL && index < MAX_ARGS - 1) {
    args[index++] = token;
    token = strtok(NULL, " ");
  }
  args[index] = NULL;

  // Expand aliases first
  expand_aliases(args);

  // Expand environment variables
  expand_environment_variables(args);

  // Handle built-in commands
  if (is_builtin_command(args)) {
    return 0;
  }

  // Fork and execute the command
  pid_t pid = fork();
  if (pid == 0) {
    execvp(args[0], args);
    perror("Command execution failed");
    exit(EXIT_FAILURE);
  }
  else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
  }
  else {
    perror("Fork failed");
  }
  return 0;
}

