#include <stdio.h>
#include "commands.h"

int main() {

  while(1){
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);

    char *token = strtok(input, " ");
    int cmd = cmd_linear_search(token);

    if(cmd == NULL){
      input[strlen(input) - 1] = '\0'; //Removes new line
      printf("%s: command not found\n", input);
    }else{
      // Implement Array of Functions
      // arr_cmd_functions[cmd]();
    }

    /* if(strstr(input, "echo") != NULL){
      
      printf("%s", input + 5);

    }else if (strstr(input, "exit 0") != NULL) {
      
      return 0;

    }else if(strstr(input, "type") != NULL){
      
      if(strstr(input + 5, "echo") != NULL){
        puts("echo is a shell builtin");
      }else if(strstr(input + 5, "exit") != NULL){
        puts("exit is a shell builtin");
      }

    }else{
      
      input[strlen(input) - 1] = '\0'; //Removes new line
      printf("%s: command not found\n", input);
      
    } */
  }

}
