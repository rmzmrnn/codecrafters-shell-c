#include "commands.h"

cmd_values cmd_linear_search(const char* key){
    
    for (size_t i=0; i < SIZE; i++) {
        
        if (strcmp(commands[i].key, key) == 0) {
            
            return commands[i].value;

        }

    }
    
    return ERR;
}

void cmd_function_echo(char *token){
    printf("%s", token + 5);
}

void cmd_function_exit(char *token){
    
    if(*(token + 5) == '0')
        exit(EXIT_SUCCESS);
    else
        puts("Invalid value - should be exit 0");
}

void cmd_function_pwd(char* token){
    // Displays current working directory
}

void cmd_function_type(char* token){
    
    cmd_values val = cmd_linear_search(token + 5);
    
    if(val != ERR){
        printf("%s is a shell builtin", token + 5);
    }else{
        // if directory does not exist
        printf("%s: not found", token + 5);

        // if directory exists
        // display path of directory
    }
}