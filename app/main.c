#include "commands.h"
#include "autocomplete.h"

int main() {
  int idx = 0;
  int tab_presses = 0;
  int suggested = 0;

  while(1){
    // printf("$ ");
    //Get the original stdout and stderr file descriptor
    int stdout_fd = dup(fileno(stdout));
    int stderr_fd = dup(fileno(stderr));
    fflush(stdout);
    write(STDOUT_FILENO, "$ ", 2); // Print new line

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
        tab_presses++;
        if (autocomplete(input, &idx, tab_presses)) {
          tab_presses = 0;
        }
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

    // free(input);
    //Bring the stdout back to the console
    fflush(stdout);
    fflush(stderr);
    dup2(stdout_fd, fileno(stdout));
    dup2(stderr_fd, fileno(stderr));
    close(stdout_fd);
    close(stderr_fd);

  }

  return 0;

}
