#include "commands.h"

int main() {

  while(1){
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';

    if (input[0] == '\"'){
        char *token = strtok(input, "\"");

      while(token != 0){
        token = strtok(0, "\"");
        if(strtok(0, "\"") == 0){
            break;
        }
      }
      
      char cat_str[100] = "cat";
      strcat(cat_str, token);
      strcpy(input, cat_str);
    }else if (input[0] == '\''){
        char *token = strtok(input, "\'");

      while(token != 0){
        token = strtok(0, "\'");
        if(strtok(0, "\'") == 0){
            break;
        }
      }
      
      char cat_str[100] = "cat";
      strcat(cat_str, token);
      strcpy(input, cat_str);
    }

    char *token = strtok(input, " ");
    cmd_values cmd = cmd_linear_search(token);

    switch(cmd){
      case CD:    cmd_function_cd(token); break;
      //case ECHO:  cmd_function_echo(token); break;
      case EXIT:  cmd_function_exit(token); break;
      case PWD:   cmd_function_pwd(token); break;
      case TYPE:  cmd_function_type(token); break;
      default:    cmd_function_exe(input); break;
    }
    
  }

  return 0;

}
