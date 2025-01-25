#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "history.h"
#include "executer.h"

#define MAX_INPUT_LENGTH 100

int main() {
  char input[MAX_INPUT_LENGTH];
  char* args[MAX_INPUT_LENGTH / 2];

  init_history();

  while (1) {
    printf("Shell > ");
    
    // Read input
    fgets(input, MAX_INPUT_LENGTH, stdin);
    input[strcspn(input, "\n")] = '\0';  // Remove newline character

    // If input is not empty, save it to history
    if (strlen(input) > 0) {
      save_to_history(input);
      reset_history_index();  // Reset for navigation

      // Process command
      if (strcmp(input, "history") == 0) {
        show_history();
      }
      else if (strcmp(input, "exit") == 0) {
        break;
      }
      else {
        // Call parse_and_execute to process and execute the command
        parse_and_execute(input, args);
      }
    }
  }

  return 0;
}

