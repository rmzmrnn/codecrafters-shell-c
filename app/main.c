#include "commands.h"
#include "autocomplete.h"

int main() {
  int idx = 0;
  int tab_presses = 0;

  while (1) {
    int stdout_fd = dup(fileno(stdout));
    int stderr_fd = dup(fileno(stderr));
    fflush(stdout);
    write(STDOUT_FILENO, "$ ", 2);

    char input[100] = {0};
    char c;
    struct termios orig_term;
    tcgetattr(STDIN_FILENO, &orig_term);
    orig_term.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);

    while (read(STDIN_FILENO, &c, 1) > 0) {
      if (c == '\n') {
        write(STDOUT_FILENO, "\n", 1);
        orig_term.c_lflag |= (ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
        break;
      } else if (c == '\t') {
        tab_presses++;
        if (autocomplete(input, &idx, tab_presses)) {
          input[idx] = ' ';
          input[idx + 1] = '\0';
          tab_presses = 0;
        }
      } else if (c == 127) {
        if (idx > 0) {
          input[--idx] = '\0';
          write(STDOUT_FILENO, "\b \b", 3);
        }
      } else {
        if (idx < sizeof(input) - 1) {
          input[idx++] = c;
          input[idx] = '\0';
          write(STDOUT_FILENO, &c, 1);
        }
      }
    }

    if (input[0] == '\0') {
      continue;
    }

    char *token = strtok(input, " ");
    cmd_values cmd = cmd_linear_search(token);

    switch (cmd) {
      case CD:    cmd_function_cd(token); break;
      case EXIT:  cmd_function_exit(token); break;
      case PWD:   cmd_function_pwd(token); break;
      case TYPE:  cmd_function_type(token); break;
      default:    cmd_function_exe(input); break;
    }

    fflush(stdout);
    fflush(stderr);
    dup2(stdout_fd, fileno(stdout));
    dup2(stderr_fd, fileno(stderr));
    close(stdout_fd);
    close(stderr_fd);
  }

  return 0;
}
