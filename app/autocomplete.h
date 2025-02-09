#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <dirent.h>

#define ALPHABET_SIZE 26

void autocomplete(char *prefix, int *idx);