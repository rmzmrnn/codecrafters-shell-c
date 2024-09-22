#include "commands.h"

cmd_values cmd_linear_search(const char* key){
    
    for (size_t i=0; i < SIZE; i++) {
        
        if (strcmp(commands[i].key, key) == 0) {
            
            return commands[i].value;

        }

    }
    
    return NULL;
}