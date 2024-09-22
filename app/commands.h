#include <string.h>

typedef struct{
    char *key;
    int value;
} command;

typedef enum {
    CD,
    ECHO,
    EXIT,
    PWD,
    TYPE
} cmd_values;

command commands[] = {
    {"cd", CD},
    {"echo", ECHO},
    {"exit", EXIT},
    {"pwd", PWD},
    {"type", TYPE}
};

const size_t SIZE = sizeof(commands) / sizeof(command);

command* cmd_linear_search(const char* key);