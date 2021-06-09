#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>


void remove_newlines(char* string);


int main()
{
  while (true) {
    char command_name[NAME_MAX] = "exit";
    pid_t new_pid;
    int retval;
    
    printf("Enter the next command (exit to exit): ");
    fgets(command_name, NAME_MAX, stdin);
    remove_newlines(command_name);

    if (0 == strcmp(command_name, "exit")) {
      exit(EXIT_SUCCESS);
    }

    new_pid = fork();
    if (-1 == new_pid) {
      perror("fork");
      putchar('\n');
      continue;
    }

    if (0 == new_pid) {
      retval = execlp(command_name, command_name, NULL);
      if (-1 == retval) {
        perror(command_name);
        exit(EXIT_FAILURE);
      }
      exit(EXIT_SUCCESS);
    } else {
      pid_t ret_pid = wait(&retval);
      if (-1 == ret_pid) {
        perror("wait");
        putchar('\n');
        continue;
      }

      if (EXIT_SUCCESS == retval) {
        printf("The command was finished successfully\n");
      } else {
        printf("The command was finished with an error\n");
      }
      putchar('\n');
    }
  }
}

void remove_newlines(char* string)
{
  int string_size = strlen(string);

  for (int i = string_size - 1; 0 != i && '\n' == string[i]; i--) {
    string[i] = 0;
  }
}
