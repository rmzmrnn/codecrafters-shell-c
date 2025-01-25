#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "commands.h"

command commands[] = {
    {"cd", CD},
    {"echo", ECHO},
    {"exit", EXIT},
    {"pwd", PWD},
    {"type", TYPE}
};

const size_t SIZE = sizeof(commands) / sizeof(command);

cmd_values cmd_linear_search(const char* key){
    
    for (size_t i=0; i < SIZE; i++) {
        
        if (strcmp(commands[i].key, key) == 0) {
            
            return commands[i].value;

        }

    }
    
    return ERR;
}

void cmd_function_echo(char *token){
    printf("%s\n", token + 5);
}

void cmd_function_exit(char *token){
    
    if(*(token + 5) == '0'){
        exit(EXIT_SUCCESS);
        exit(0);
    }else
        puts("Invalid value - should be exit 0\n");
}

void cmd_function_pwd(char* token){
    // Displays current working directory
}

// Function to search for a command in PATH
void cmd_function_type(char* token){
    
    char *path_env = getenv("PATH");
    
    if (path_env == NULL){
        printf("%s: not found\n", token);
    }

    char *path_copy = strdup(path_env);
    char *dir = strtok(path_copy, ":");
    static char full_path[1024];

    while (dir != NULL) {

        snprintf(full_path, sizeof(full_path), "%s/%s", dir, token);

        if (access(full_path, X_OK) == 0) {
            free(path_copy);
            printf("%s is %s\n", token, full_path);
        }
        
        dir = strtok(NULL, ":");
    }

    free(path_copy);

}