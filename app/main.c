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
    cmd_values cmd = cmd_linear_search(token);

    switch(cmd){
      case ERR: input[strlen(input) - 1] = '\0';
                printf("%s: command not found\n", input);
                break;
      case CD:    puts("Not yet implemented!"); break;
      case ECHO:  cmd_function_echo(token); break;
      case EXIT:  cmd_function_exit(token); break;
      case PWD:   cmd_function_pwd(token); break;
      case TYPE:  cmd_function_type(token); break;
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

  return 0;

}
