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

    if (path == NULL) {
      // PATH not found  
      return;
    }
  
    char *path_copy = strdup(path);
    char *token = strtok(path_copy, ":");
  
    while (token != NULL) {
      DIR *dir = opendir(token);
  
      if (dir == NULL) {
        // Cannot open directory, move to the next token
        token = strtok(NULL, ":");
        continue;
      }
  
      struct dirent *entry;
  
      while ((entry = readdir(dir)) != NULL) {
        char *dir_file = entry->d_name;
  
        // Skip "." and ".." entries
        if (strcmp(dir_file, ".") == 0 || strcmp(dir_file, "..") == 0) {
          continue;
        }
  
        if (strncmp(dir_file, input, strlen(input)) == 0) {
  
          // Check for duplicates in entries_buffer
          int duplicate = 0;
  
          for (int i = 0; i < entries_buffer_size; i++) {
            if (strcmp(entries_buffer[i], dir_file) == 0) {
              duplicate = 1;
              break;
            }
          }
  
          if (!duplicate) {
            // Add the entry to entries_buffer
            entries_buffer[entries_buffer_size] = strdup(dir_file);
  
            if (entries_buffer[entries_buffer_size] == NULL) {
              // Memory allocation failure
              closedir(dir);
              free(path_copy);
              return;
  
            }
  
            entries_buffer_size++;
  
          }
  
        }
  
      }
  
      token = strtok(NULL, ":");
  
    }
  
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
    // printf("\n");
    write(STDOUT_FILENO,  "\n", 3);
  
    for (int i = 0; i < entries_buffer_size; i++) {
        //   printf("%s  ", entries_buffer[i]);
        write(STDOUT_FILENO,  entries_buffer[i], 100);
        free(entries_buffer[i]);
    }
  
    entries_buffer_size = 0;
  
    free(entries_buffer);
  
    // printf("\n");
    write(STDOUT_FILENO,  "\n", 3);
    // printf("$ %s", input);
    write(STDOUT_FILENO, input, 100);
    fflush(stdout);
  
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
int autocomplete(char *input, int *idx) {

    const char *commands[] = {"echo", "exit", "type"};
    int num_of_commands = sizeof(commands) / sizeof(commands[0]);
    int found = 0;
  
    for (int i = 0; i < num_of_commands; i++) {
  
      if (strncmp(commands[i], input, strlen(input)) == 0) {
        // strcpy(input + strlen(input), commands[i] + strlen(input));
        strcpy(input, commands[i]);
        
        *idx = strlen(input);
        
        // printf("\r$ %s", input);
        write(STDOUT_FILENO,  "\r$ ", 5);
        write(STDOUT_FILENO,  input, 100);
        fflush(stdout);
        
        found = 1;
        
        return 1;
      }
  
    }
  
    // check executables
    if (!found) {

        set_suggestion_buffer(input);
    
        char *shortest_match = NULL;
        int input_len = strlen(input);
    
        sort_entries_buffer_on_size();
    
        int comapre_idx = input_len;
        char common_prefix[256]; // Adjust size as needed
    
        if (entries_buffer_size < 1) {
          goto end;
        }
    
        if (entries_buffer_size == 1) {
    
          // Only one match; complete the input
          strcpy(input, entries_buffer[0]);
    
          *idx = strlen(input);
    
          //   printf("\r$ %s ", input);
          strcat(input, "\r$ %s ");
          write(STDOUT_FILENO,  input, 100);
          fflush(stdout);
    
          found = 1;
    
          // Free entries_buffer
          free_buffer();
    
          return 1;
    
        }
    
        strcpy(common_prefix, entries_buffer[0]);
    
        for (int i = 1; i < entries_buffer_size; i++) {
    
          int j = input_len; // Start comparing from the end of input
          
          while (common_prefix[j] && entries_buffer[i][j] && common_prefix[j] == entries_buffer[i][j]) {
            j++;
          }
    
          common_prefix[j] = '\0'; // Truncate at divergence
    
        }
    
        if (strlen(common_prefix) > input_len) {
    
          // Extend input to the longest common prefix
          strcpy(input, common_prefix);
    
          *idx = strlen(input);
    
          //   printf("\r$ %s", input);
          strcat(input, "\r$ %s ");
          write(STDOUT_FILENO,  input, 100);
          fflush(stdout);
    
          found = 1;
    
          // Free entries_buffer
          free_buffer();
    
          return 1;
    
        }
    
    }
end:
    if (!found)
        // printf("\a");
        write(STDOUT_FILENO,  "\a", 3);
    return 0;
}
