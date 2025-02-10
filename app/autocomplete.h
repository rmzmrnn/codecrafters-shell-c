#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <dirent.h>

#define ALPHABET_SIZE 26

void print_entries_buffer(char *input);
int autocomplete(char *input, int *idx, int tab_presses);