#include "commands.h"
#include <stdlib.h>

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
    cmd_values val = cmd_linear_search(token + 5);
    if(val != ERR){
        printf("%s is a shell builtin\n", token + 5);
    }else{
        char *env_path = getenv("PATH");

        // char *PATH = malloc(strlen(env_path)); // strtok mutates the PATH string
        // strcpy(PATH, env_path);

        char *PATH = strdup(env_path);
        char *dirpath = strtok(PATH, ":");

        while (dirpath) {

            DIR *directory = opendir(dirpath);

            if (directory == NULL) {
                dirpath = strtok(NULL, ":");
                continue;
            }

            struct dirent *file;

            while ((file = readdir(directory))) {
                if (strcmp(file->d_name, token + 5) == 0) {
                printf("%s is %s/%s\n", token + 5, dirpath, file->d_name);
                closedir(directory);
                free(PATH);
                return;
                }

            }

            dirpath = strtok(NULL, ":");
            closedir(directory);

        }

        free(PATH);
        printf("%s not found\n", token + 5);
    }
}

char **getPaths(char *path, int *path_count) {

    path_count[0] = 0;

    for (int i = 0; path[i]; i++) {
        if (path[i] == ':')
            path_count[0]++;
    }

    path_count[0]++;
    char **filepaths = calloc(path_count[0], sizeof(char *));

    for (int i = 0; i < path_count[0]; i++)
        filepaths[i] = calloc(SIZE, sizeof(char));

    int x = 0;
    int y = 0;

    for (int i = 0; path[i]; i++) {
        if (path[i] != ':') {
            filepaths[x][y++] = path[i];
        } else {
            filepaths[x++][y] = '\0';
            y = 0;
        }

    }

    return filepaths;
}

void cmd_function_exe(char* input){   
    // int status = system(token);

    // if(status != 0){
    //     printf("%s: command not found\n", token);
    // }

    char *PATH = getenv("PATH");
    int *path_count = calloc(1, sizeof(int));
    char **filepaths = getPaths(PATH, path_count);

    for (int i = 0; i < path_count[0]; i++) {

        char *inputCopy = calloc(SIZE, sizeof(char));

        strcpy(inputCopy, input);

        char *command = strtok(inputCopy, " ");
        char fullpath[strlen(filepaths[i]) + strlen(command)];

        sprintf(fullpath, "%s/%s", filepaths[i], command);

        if (access(fullpath, X_OK) == 0) {
            char exec[strlen(filepaths[i]) + strlen(input) + 2]; // +2 for space and null terminator
            sprintf(exec, "%s %s", command, input + strlen(command) + 1);
            system(exec);
            return;
        }

    }
    
    printf("%s: command not found\n", input);
}