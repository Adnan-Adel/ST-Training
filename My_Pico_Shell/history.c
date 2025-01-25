#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 50

static char* history[MAX_HISTORY];
static int history_count = 0;
static int history_index = -1;  // To track the current position for up/down navigation

void init_history() {
  for (int i = 0; i < MAX_HISTORY; i++) {
    history[i] = NULL;
  }
}

void save_to_history(const char* command) {
  if(command == NULL || *command == '\0') {
    return;
  }

  if(history_count < MAX_HISTORY) {
    history[history_count++] = strdup(command);
  }
  else {
    free(history[0]);
    memmove(history, history + 1, (MAX_HISTORY - 1) * sizeof(char*));
    history[MAX_HISTORY - 1] = strdup(command);
  }
}

void show_history() {
  for(int i = 0; i < history_count; i++) {
    printf("%d: %s\n", i + 1, history[i]);
  }
}

const char* get_previous_command() {
  if (history_index > 0) {
    history_index--;
    return history[history_index];
  }
  return NULL;
}

const char* get_next_command() {
  if (history_index < history_count - 1) {
    history_index++;
    return history[history_index];
  }
  return NULL;
}

void reset_history_index() {
  history_index = history_count;  // To start at the most recent command when up/down navigation starts
}

