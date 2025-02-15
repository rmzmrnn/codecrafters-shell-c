#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <termios.h>
#include <sys/wait.h>


typedef struct{
    char *key;
    int value;
} command;

typedef enum {
    ERR = -1,
    CD,
    // ECHO,
    EXIT,
    PWD,
    TYPE
} cmd_values;

cmd_values cmd_linear_search(const char* key);

// void cmd_function_echo(char* token);
void cmd_function_exit(char* token);
void cmd_function_pwd(char* token);
void cmd_function_type(char* token);
void cmd_function_exe(char* input);
void cmd_function_cd(char* token);