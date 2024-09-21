#include <stdio.h>
#include <string.h>

int main() {
  // Uncomment this block to pass the first stage
  printf("$ ");
  fflush(stdout);

  // Wait for user input
  char input[100];
  fgets(input, 100, stdin);

  input[strlen(input) - 1] = '\0'; //Removes new line
  printf("%s: command not found", input);

  return 0;
}
