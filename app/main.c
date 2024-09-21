#include <stdio.h>
#include <string.h>

int main() {

  while(1){
    printf("$ ");
    fflush(stdout);

    // Wait for user input
    char input[100];
    fgets(input, 100, stdin);

    if(strstr(input, "echo") != NULL){
      
      printf("%s", input + 5);

    }else if (strstr(input, "exit 0") != NULL) {
      
      return 0;

    }else{
      
      input[strlen(input) - 1] = '\0'; //Removes new line
      printf("%s: command not found\n", input);
      
    }
  }

}
