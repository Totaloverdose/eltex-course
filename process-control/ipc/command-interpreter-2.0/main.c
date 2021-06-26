#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>


typedef struct command {
  char *c_name;
  char **c_args;
  struct command *c_next;
} command;


void remove_spaces(char *string);
void get_command_list(struct command **command_list);
void free_command_list(struct command *command_list);
void execute_commands(struct command *command_list);


int main()
{
  printf("Welcome to the command interpreter Whoop-de-doo 3000\n"
         "We have a new function: pipes support!\n\n\n");
  
  while (true) {
    char command_arr[NAME_MAX] = "exit";
    struct command *command_list = NULL;
    
    printf("Enter the next command (exit to exit): ");
    fgets(command_arr, NAME_MAX, stdin);

    get_command_list(&command_list);
    execute_commands(command_list);

    free_command_list(command_list);
  }
}

void remove_spaces(char *string)
{
  int string_size = strlen(string);

  for (int i = string_size - 1; 0 <= i && ('\n' == string[i] || ' ' == string[i]); i--) {
    string[i] = 0;
  }
}

void get_command_list(char *src_command_arr, struct command **out_command_list)
{
  if (NULL != command_list) {
    free_command_list(*command_list);
    *command_list = NULL;
  }

  
}

void free_command_list(struct command *command_list)
{
  while (NULL != command_list) {
    struct command *next_command = command_list->c_next;
    free(command_list);
    command_list = next_command;
  }
}

void execute_commands(struct command *command_list)
{
  pid_t new_pid;
  int retval;
  struct command *first_command_ptr = command_list;

  
  while (NULL != command_list) {
      
    if (0 == strcmp(command_list->c_name, "exit")) {
      printf("Exit from Whoop-de-doo 3000...\n");
      free_command_list(first_command_ptr);
      exit(EXIT_SUCCESS);
    }

    new_pid = fork();
    if (-1 == new_pid) {
      perror("fork");
      putchar('\n');
      continue;
    }

    if (0 == new_pid) {
      retval = execvp(command_list->c_name, command_list->c_args);
      if (-1 == retval) {
        perror(command_list->c_name);
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
        printf("The command %s was finished successfully\n", command_list->c_name);
      } else {
        printf("The command %s was finished with an error\n", command_list->c_name);
      }
      putchar('\n');

      struct command *next_command = command_list->c_next;
      free(command_list);
      command_list = next_command;
    }
  }
}
