#include "autocomplete.h"
#include <string.h>

static char **entries_buffer = NULL; // Global buffer to store suggestions
static int entries_buffer_size = 0;  // Number of entries in the buffer
static int entries_buffer_capacity = 0;


void free_buffer() {    
    
    for (int i = 0; i < entries_buffer_size; i++) {
      free(entries_buffer[i]);
    }
  
    entries_buffer_size = 0;
}

void set_suggestion_buffer(char *input) {
  char *path = getenv("PATH");
  if (!path) return;

  char *path_copy = strdup(path);
  char *token = strtok(path_copy, ":");

  // Allocate initial buffer capacity
  if (entries_buffer == NULL) {
      entries_buffer_capacity = 10;
      entries_buffer = malloc(entries_buffer_capacity * sizeof(char *));
  }

  while (token) {
      DIR *dir = opendir(token);
      if (!dir) {
          token = strtok(NULL, ":");
          continue;
      }

      struct dirent *entry;
      while ((entry = readdir(dir)) != NULL) {
          if (entry->d_name[0] == '.' || strncmp(entry->d_name, input, strlen(input)) != 0)
              continue;

          // Ensure capacity
          if (entries_buffer_size >= entries_buffer_capacity) {
              entries_buffer_capacity *= 2;
              entries_buffer = realloc(entries_buffer, entries_buffer_capacity * sizeof(char *));
          }

          entries_buffer[entries_buffer_size++] = strdup(entry->d_name);
      }

      closedir(dir);
      token = strtok(NULL, ":");
  }

  free(path_copy);
}

void sort_entries_buffer() {

    if (entries_buffer_size <= 1) {
      return; // No need to sort
    }
  
    for (int i = 0; i < entries_buffer_size - 1; i++) {
      for (int j = i + 1; j < entries_buffer_size; j++) {
        if (strcmp(entries_buffer[i], entries_buffer[j]) > 0) {
  
          // Swap entries
          char *temp = entries_buffer[i];
          entries_buffer[i] = entries_buffer[j];
          entries_buffer[j] = temp;
  
        }
  
      }
  
    }
  
}

void print_entries_buffer(char *input) {
  sort_entries_buffer();
  write(STDOUT_FILENO, "\n", 1);

  for (int i = 0; i < entries_buffer_size; i++) {
      write(STDOUT_FILENO, entries_buffer[i], strlen(entries_buffer[i]));
      write(STDOUT_FILENO, "  ", 2);  // Two spaces
      free(entries_buffer[i]);
  }

  write(STDOUT_FILENO, "\n$ ", 3);
  write(STDOUT_FILENO, input, strlen(input));

  free(entries_buffer);
  entries_buffer = NULL;
  entries_buffer_size = 0;
}

void sort_entries_buffer_on_size() {

    if (entries_buffer_size <= 1) {
      return; // No need to sort
    }
  
    for (int i = 0; i < entries_buffer_size - 1; i++) {
      for (int j = i + 1; j < entries_buffer_size; j++) {
        if (strlen(entries_buffer[i]) > strlen(entries_buffer[j])) {
  
          // Swap entries
          char *temp = entries_buffer[i];
          entries_buffer[i] = entries_buffer[j];
          entries_buffer[j] = temp;
  
        }
  
      }
  
    }
  
}

// Find and Print Autocomplete Suggestions
int autocomplete(char *input, int *idx, int tab_presses) {
  set_suggestion_buffer(input);

  if (entries_buffer_size == 0) {
      write(STDOUT_FILENO, "\a", 1); // No matches, ring the bell
      return 0;
  }

  if (entries_buffer_size == 1) {
      // Single match, autocomplete the input
      strcpy(input, entries_buffer[0]);
      *idx = strlen(input);
      write(STDOUT_FILENO, "\r$ ", 3);
      write(STDOUT_FILENO, input, strlen(input));
      write(STDOUT_FILENO, " ", 1);
      free_buffer();
      return 1;
  }

  if (tab_presses == 1) {
      // Multiple matches, first Tab press: ring bell
      write(STDOUT_FILENO, "\a", 1);
      return 0;
  }

  if (tab_presses == 2) {
      // Multiple matches, second Tab press: print suggestions
      print_entries_buffer(input);
      return 0;
  }

  return 0;
}
