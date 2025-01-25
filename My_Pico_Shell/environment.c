#include "environment.h"
#include <stdlib.h>
#include <string.h>

void expand_environment_variables(char** args) {
  for (int i = 0; args[i] != NULL; i++) {
    if (args[i][0] == '$') {
      char* value = getenv(args[i] + 1);
      if (value) {
        args[i] = strdup(value);
      }
    }
  }
}

