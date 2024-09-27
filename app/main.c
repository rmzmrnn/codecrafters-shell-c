#include <stdio.h>
#include "commands.h"

int main() {

  while(1){
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);

    input[strlen(input) - 1] = '\0';

    char *token = strtok(input, " ");
    cmd_values cmd = cmd_linear_search(token);

    switch(cmd){
      case ERR: printf("%s: command not found\n", input);
                break;
      case CD:    puts("Not yet implemented!\n"); break;
      case ECHO:  cmd_function_echo(token); break;
      case EXIT:  cmd_function_exit(token); break;
      case PWD:   cmd_function_pwd(token); break;
      case TYPE:  cmd_function_type(token); break;
    }
    
  }

  return 0;

}
