#include "autocomplete.h"
#include <string.h>

static char **entries_buffer = NULL;
static int entries_buffer_size = 0;
static int entries_buffer_capacity = 0;

void free_buffer() {
  for (int i = 0; i < entries_buffer_size; i++) {
    free(entries_buffer[i]);
  }
  free(entries_buffer);
  entries_buffer = NULL;
  entries_buffer_size = 0;
}

void set_suggestion_buffer(char *input) {
  char *path = getenv("PATH");
  if (!path) return;

  free_buffer();

  char *path_copy = strdup(path);
  char *token = strtok(path_copy, ":");

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

      // Check for duplicates before adding
      int duplicate = 0;
      for (int i = 0; i < entries_buffer_size; i++) {
        if (strcmp(entries_buffer[i], entry->d_name) == 0) {
          duplicate = 1;
          break;
        }
      }

      if (!duplicate) {
        if (entries_buffer_size >= entries_buffer_capacity) {
          entries_buffer_capacity *= 2;
          entries_buffer = realloc(entries_buffer, entries_buffer_capacity * sizeof(char *));
        }
        entries_buffer[entries_buffer_size++] = strdup(entry->d_name);
      }
    }

    closedir(dir);
    token = strtok(NULL, ":");
  }

  free(path_copy);
}

void sort_entries_buffer() {
  if (entries_buffer_size <= 1) return;
  for (int i = 0; i < entries_buffer_size - 1; i++) {
    for (int j = i + 1; j < entries_buffer_size; j++) {
      if (strcmp(entries_buffer[i], entries_buffer[j]) > 0) {
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
    if (i < entries_buffer_size - 1) {
      write(STDOUT_FILENO, "  ", 2);
    }
  }

  write(STDOUT_FILENO, "\n$ ", 3);
  write(STDOUT_FILENO, input, strlen(input));

  free_buffer();
}

int autocomplete(char *input, int *idx, int tab_presses) {
  
  const char *commands[] = {"echo", "exit", "type"};
  int num_of_commands = sizeof(commands) / sizeof(commands[0]);

  int found = 0;

  // Check for built-in command matches
  for (int i = 0; i < num_of_commands; i++) {
    if (strncmp(commands[i], input, strlen(input)) == 0) {
      snprintf(input, 100, "%s ", commands[i]); // Ensure space after completion
      *idx = strlen(input);
      write(STDOUT_FILENO, "\r$ ", 3);
      write(STDOUT_FILENO, input, strlen(input));
      return 1;
    }
  }
  
  set_suggestion_buffer(input);
  
  if (entries_buffer_size == 0) {
    write(STDOUT_FILENO, "\a", 1);
    return 0;
  }

  sort_entries_buffer();

  char common_prefix[256];
  strcpy(common_prefix, entries_buffer[0]);

  for (int i = 1; i < entries_buffer_size; i++) {
    int j = 0;
    while (common_prefix[j] && entries_buffer[i][j] && common_prefix[j] == entries_buffer[i][j]) {
      j++;
    }
    common_prefix[j] = '\0';
  }

  if (strlen(common_prefix) > strlen(input)) {
    strcpy(input, common_prefix);
    *idx = strlen(input);

    write(STDOUT_FILENO, "\r$ ", 3);
    write(STDOUT_FILENO, input, strlen(input));

    if (entries_buffer_size == 1) {
      write(STDOUT_FILENO, " ", 1);
      *idx += 1;
    }

    free_buffer();
    return 1;
  }

  if (tab_presses == 1) {
    write(STDOUT_FILENO, "\a", 1);
    return 0;
  }

  if (tab_presses == 2) {
    print_entries_buffer(input);
    return 0;
  }

  return 0;
}
