#include "alias.h"
#include <string.h>

#define MAX_ALIAS 20
#define MAX_ALIAS_LENGTH 50

typedef struct {
  char alias[MAX_ALIAS_LENGTH];
  char command[MAX_ALIAS_LENGTH];
} Alias;

static Alias aliases[MAX_ALIAS];
static int alias_count = 0;

void init_aliases() {
  alias_count = 0;
}

void add_alias(const char* alias, const char* command) {
  if (alias_count < MAX_ALIAS) {
    strcpy(aliases[alias_count].alias, alias);
    strcpy(aliases[alias_count].command, command);
    alias_count++;
  }
}

void expand_aliases(char** args) {
  for (int i = 0; i < alias_count; i++) {
    if (strcmp(args[0], aliases[i].alias) == 0) {
      args[0] = strdup(aliases[i].command);
      return;
    }
  }
}

