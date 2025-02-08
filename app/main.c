#include "commands.h"
#include "autocomplete.h"
#include <stdio.h>

int main() {

  TrieNode *root = createNode();

  insert(root, "cd");
  insert(root, "echo");
  insert(root, "exit");
  insert(root, "pwd");
  insert(root, "type");

  while(1){
    // printf("$ ");
    write(STDOUT_FILENO, "$ ", 2); // Print new line
    fflush(stdout);

    // Wait for user input
    char input[100];
    char c;
    memset(input, 0, sizeof(input));

    struct termios orig_term;
    tcgetattr(STDIN_FILENO, &orig_term);
    orig_term.c_lflag &= ~(ECHO | ICANON);  // Disable echo and canonical mode
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);

    while (read(STDIN_FILENO, &c, 1) > 0){
      if (c == '\n') {  // Enter key pressed
        write(STDOUT_FILENO, "\n", 1); // Print new line
        
        // Enable echo and canonical mode
        tcgetattr(STDIN_FILENO, &orig_term);
        orig_term.c_lflag |= (ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
        
        break;
      } else if (c == '\t') {
        input[strlen(input)] = '\0'; // Null-terminate before autocomplete
        autocomplete(root, input);
        write(STDOUT_FILENO, " ", 1); // Print a space
      }else if(c == 127){ //Back space
        if (strlen(input) > 0){
          input[strlen(input) - 1] = '\0';
          write(STDOUT_FILENO, "\b \b", 3); // Move cursor back, overwrite with space, move back again
        }
      }else{
        input[strlen(input)] = c;  // Store character
        write(STDOUT_FILENO, &c, 1); // Print other characters normally
      }
    }

    // when the user just pressed the Enter key, the loop continues
    if(input[0] == '\n'){
      continue;
    }

    input[strlen(input)] = '\0';

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
    
    free(input);
    fflush(stdout);

  }

  return 0;

}
