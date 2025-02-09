#include "autocomplete.h"

// Find and Print Autocomplete Suggestions
void autocomplete(char *input, int *idx) {

    const char *commands[] = {"echo", "exit", "type"};
  
    int num_of_commands = sizeof(commands) / sizeof(commands[0]);
  
    int found = 0;
  
    for (int i = 0; i < num_of_commands; i++) {
  
      if (strncmp(commands[i], input, strlen(input)) == 0) {
        // strcpy(input + strlen(input), commands[i] + strlen(input));
        strcpy(input, commands[i]);
        *idx = strlen(input);
        // printf("\r$ %s", input);
        write(STDOUT_FILENO,  "\r$ ", 3);
        write(STDOUT_FILENO,  input, 100);
        fflush(stdout);
        found = 1;
        return;
  
      }
  
    }
  
    // check executables
    if (!found) {
        char *path = getenv("PATH");

        if (!path) {

        // fprintf(stderr, "Failed to get PATH environment variable.\n");
        write(STDOUT_FILENO, "Failed to get PATH environment variable.\n", 100);
        return;

        }

        char *path_copy = strdup(path);
        char *token = strtok(path_copy, ":");

        while (token != NULL) {

        DIR *dir = opendir(token);

        if (dir) {

            struct dirent *entry;

            while ((entry = readdir(dir)) != NULL) {

            char *dir_file = entry->d_name;

            if (strncmp(dir_file, input, strlen(input)) == 0) {
                // found match , append to a
                // strcpy(input + strlen(input), dir_file + strlen(input));
                strcpy(input, dir_file + strlen(input));
                *idx = strlen(input);
                // printf("\r$ %s ", input);
                write(STDOUT_FILENO, input, 100);
                fflush(stdout);
                found = 1;
                break;

            }
            }
            closedir(dir);
        }
        token = strtok(NULL, ":");
        }
        free(path_copy);
    }
  
    if (!found)
        // printf("\a");
        write(STDOUT_FILENO, "\a", 1);
}
