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
    cmd_values val = cmd_linear_search(token + 5);
    if(val != ERR){
        // printf("%s is a shell builtin\n", token + 5);
        printf("%s is a shell builtin", token + 5);
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