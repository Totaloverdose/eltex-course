#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

typedef struct command
{
  char *c_name;           // Command name
  char **c_args;          // Command arguments
  struct command *c_next; // Ponter to the next command
} command;

typedef enum pipe_mode
{
  READ_MODE = 0,
  WRITE_MODE = 1,
} pipe_mode;

/**
 * @brief Removes spaces from the end of the string
 *
 * @param string
 */
void remove_spaces(char *string);
/**
 * @brief Get the command list object
 *
 * @param src_command_arr // String that is source of commands
 * @param out_command_list_null // Output command list
 */
void get_command_list(char *src_command_arr, struct command **out_command_list_null);
/**
 * @brief Free the command structures' list
 *
 * @param command_list
 */
void free_command_list(struct command *command_list);
/**
 * @brief Execute commands from list using pipe
 *
 * @param command_list
 */
void execute_commands(struct command *command_list);
/**
 * @brief Creates a new command struct and returns pointer to it
 *
 * @return struct command*
 */
struct command *make_new_command_struct(void);
/**
 * @brief Receives commands' arguments as one string and splits it
 * to a commands' arguments string arrays using pipe character '|'
 * as a separator
 *
 * @param command_arr
 * @return char**
 */
char **split_commands(char *command_arr);
/**
 * @brief Receive command's arguments as a string and splits it to array of arguments
 * using space character ' ' as a separator
 *
 * @param args_str
 * @return char**
 */
char **split_arguments(char *args_str);
/**
 * @brief Create a pipe object
 *
 * @param pipe_fds
 */
void create_pipe(int pipe_fds[2]);
/**
 * @brief Set the pipe object instead of the standard input and output,
 * creates a new pipe if isn't already created
 *
 * @param stdio_fds NULL if you don't want to set it
 * @param pipe_fds
 */
void set_pipe(int pipe_fds[2], enum pipe_mode mode);
/**
 * @brief Reset the file descriptor of stdin or stdout to default
 * depending on mode
 *
 * @param stdio_fds
 * @param pipe_fds
 * @param mode
 */
void unset_pipe(int stdio_fds[2], enum pipe_mode mode);
/**
 * @brief closes pipe and copied stdio descriptor input or output desctiptor
 * depending on mode
 *
 * @param stdio_fds
 * @param pipe_fds
 * @param mode
 */
void close_fds(int pipe_fds[2], enum pipe_mode mode);
/**
 * @brief Reverse the list to the opposite direction
 *
 * @param first_command_ptr
 */
void reverse_list(struct command **first_command_ptr);
/**
 * @brief Notify users that a command was finished with an error
 *
 * @param command_name
 * @param command_args
 */
void notify_command_error(char *command_name, char **command_args);

enum
{
  C_NAME_MAX_SIZE = 512, // Max command name and argument size
};

int main()
{
  printf("Welcome to the command interpreter Whoop-de-doo 3000\n"
         "We have a new function: pipes support!\n\n\n");

  while (true)
  {
    char command_arr[C_NAME_MAX_SIZE] = "exit";
    struct command *command_list = NULL;

    printf("Enter the next command (exit to exit): ");
    fgets(command_arr, C_NAME_MAX_SIZE, stdin);
    putchar('\n');

    remove_spaces(command_arr);
    get_command_list(command_arr, &command_list);
    execute_commands(command_list);

    free_command_list(command_list);
  }
}

void remove_spaces(char *string)
{
  if (NULL == string)
  {
    return;
  }

  int string_size = strlen(string);

  for (int i = string_size - 1; 0 <= i && ('\n' == string[i] || ' ' == string[i]); i--)
  {
    string[i] = 0;
  }
}

void get_command_list(char *src_command_arr, struct command **out_command_list_null)
{
  struct command *first_command = NULL;
  struct command *prev_command = NULL;
  bool is_argument_beginning = true;
  char **sep_command_arr = NULL;

  if (NULL == src_command_arr)
  {
    fprintf(stderr, "[Function %s]: src_command_arr must not be NULL\n", __FUNCTION__);
    return;
  }

  if (NULL != *out_command_list_null)
  {
    fprintf(stderr,
            "[Line %s] Warning: the out list isn't NULL. If it is not empty,"
            "the data and the memory will be lost",
            __LINE__);
    *out_command_list_null = NULL;
  }

  sep_command_arr = split_commands(src_command_arr);

  for (int i = 0; NULL != sep_command_arr[i]; i++)
  {
    *out_command_list_null = make_new_command_struct();
    if (NULL != prev_command)
    {
      prev_command->c_next = *out_command_list_null;
    }
    else
    {
      first_command = *out_command_list_null;
    }

    (*out_command_list_null)->c_args = split_arguments(sep_command_arr[i]);
    if (NULL != (*out_command_list_null)->c_args && NULL != (*out_command_list_null)->c_args[0])
    {
      strcpy((*out_command_list_null)->c_name, (*out_command_list_null)->c_args[0]);
    }
    else
    {
      fprintf(stderr, "[Line %d]: Error. Command has no name\n", __LINE__);
    }

    prev_command = *out_command_list_null;
    free(sep_command_arr[i]);
  }

  free(sep_command_arr);
  *out_command_list_null = first_command;
}

void free_command_list(struct command *command_list)
{
  while (NULL != command_list)
  {
    struct command *next_command = command_list->c_next;

    free(command_list->c_name);
    for (int i = 0; NULL != command_list->c_args[i]; i++)
    {
      free(command_list->c_args[i]);
    }
    free(command_list->c_args);
    free(command_list);

    command_list = next_command;
  }
}

void execute_commands(struct command *command_list)
{
  pid_t new_pid = -1;
  int retval = 0;
  struct command *first_command_ptr = command_list;
  int stdout_fd = -2;
  int pipe_fds[2] = {-2, -2};

  stdout_fd = dup(STDOUT_FILENO);
  if (-1 == stdout_fd)
  {
    perror("dup");
    exit(EXIT_FAILURE);
  }

  if (0 == strcmp(command_list->c_name, "exit"))
  {
    printf("Exit from the Whoop-de-doo 3000...\n");
    free_command_list(first_command_ptr);
    exit(EXIT_SUCCESS);
  }

  reverse_list(&command_list);

  if (0 == strcmp(command_list->c_name, "exit"))
  {
    printf("Exit from the Whoop-de-doo 3000...\n");
    free_command_list(first_command_ptr);
    exit(EXIT_SUCCESS);
  }

  new_pid = fork();
  if (-1 == new_pid)
  {
    perror("fork");
    putchar('\n');
    return;
  }

  if (0 == new_pid)
  {
    while (NULL != command_list)
    {
      create_pipe(pipe_fds);

      new_pid = fork();
      if (-1 == new_pid)
      {
        perror("fork");
        putchar('\n');
        continue;
      }

      if (0 == new_pid)
      {
        close_fds(pipe_fds, READ_MODE);
        set_pipe(pipe_fds, WRITE_MODE);

        command_list = command_list->c_next;
      }
      else
      {
        close_fds(pipe_fds, WRITE_MODE);
        set_pipe(pipe_fds, READ_MODE);

        pid_t ret_pid = wait(&retval);
        if (-1 == ret_pid)
        {
          perror("wait");
          putchar('\n');
          continue;
        }

        if (EXIT_SUCCESS != retval)
        {
          notify_command_error(command_list->c_name, command_list->c_args);
        }

        execvp(command_list->c_name, command_list->c_args);
      }
    }

    exit(EXIT_SUCCESS);
  }
  else
  {
    wait(NULL);
    printf("\n\n");
  }
}

struct command *make_new_command_struct()
{
  struct command *new_command = malloc(sizeof(struct command));

  new_command->c_name = malloc(C_NAME_MAX_SIZE);
  new_command->c_args = NULL;
  new_command->c_next = NULL;

  return new_command;
}

char **split_commands(char *command_arr)
{
  int arr_size = 8;
  char **args_arr = calloc(arr_size, sizeof(char *));
  int i = 0; // Command index (argument array index)
  int j = 0; // Argument character position

  if (NULL == command_arr)
  {
    fprintf(stderr, "[Function %s]: command_arr must not be NULL\n", __FUNCTION__);
    return NULL;
  }

  for (i = 0; '\0' != *command_arr; i++)
  {
    // The last argument must be NULL (so execvp will know when to stop).
    // So we need more than i + 1 arg strings
    if (i == arr_size - 2)
    {
      arr_size *= 2;
      args_arr = reallocarray(args_arr, arr_size, sizeof(char *));
    }

    args_arr[i] = malloc(C_NAME_MAX_SIZE);

    for (j = 0; '|' != *command_arr && '\0' != *command_arr; j++)
    {
      args_arr[i][j] = *command_arr;
      command_arr++;
    }

    args_arr[i][j] = '\0';

    if ('|' == *command_arr)
    {
      command_arr++;
    }
  }

  args_arr[i] = NULL;

  return args_arr;
}

char **split_arguments(char *args_str)
{
  int arr_size = 8;
  char **args = calloc(arr_size, sizeof(char *));
  int i = 0; // Argument index
  int j = 0; // Argument character position

  if (NULL == args_str)
  {
    fprintf(stderr, "[Function %s]: args_str must not be NULL\n");
    return NULL;
  }

  for (i = 0; '\0' != *args_str; i++)
  {
    if (i == arr_size - 3)
    {
      arr_size *= 2;
      args = reallocarray(args, arr_size, sizeof(char *));
    }

    args[i] = malloc(C_NAME_MAX_SIZE);

    while (' ' == *args_str)
    {
      args_str++;
    }
    for (j = 0; ' ' != *args_str && '\0' != *args_str; j++)
    {
      args[i][j] = *args_str;
      args_str++;
    }
    while (' ' == *args_str)
    {
      args_str++;
    }

    args[i][j] = '\0';
  }

  args[i] = NULL;

  return args;
}

void create_pipe(int pipe_fds[2])
{
  int retval = 0;

  if (NULL == pipe_fds)
  {
    fprintf(stderr, "[Function %s]: pipe_fds should not be NULL to create a new pipe\n");
    return;
  }

  retval = pipe(pipe_fds);
  if (0 != retval)
  {
    perror("create_pipe");
    exit(EXIT_FAILURE);
  }
}

void set_pipe(int pipe_fds[2], enum pipe_mode mode)
{
  char func_name[C_NAME_MAX_SIZE];
  int retval = 0;

  if (READ_MODE != mode && WRITE_MODE != mode)
  {
    fprintf(stderr, "[Function %s]: mode should be READ_MODE or WRITE_MODE\n", __FUNCTION__);
    return;
  }

  if (NULL != pipe_fds && 0 < pipe_fds[mode])
  {
    retval = dup2(pipe_fds[mode], mode);
    if (-1 == retval)
    {

    error:
      perror("dup");
      exit(EXIT_FAILURE);
    }
  }
}

void unset_pipe(int stdio_fds[2], enum pipe_mode mode)
{
  int retval = 0;

  if (NULL == stdio_fds)
  {
    fprintf(stderr, "[Function %s]: stdio_fds must not be NULL\n");
    return;
  }

  if (READ_MODE != mode && WRITE_MODE != mode)
  {
    fprintf(stderr, "[Function %s]: mode should be READ_MODE or WRITE_MODE\n", __FUNCTION__);
    return;
  }

  retval = dup2(stdio_fds[mode], mode);
  if (-1 == retval)
  {
    goto error;
  }

  return;

error:
  perror("dup");
  exit(EXIT_FAILURE);
}

void close_fds(int pipe_fds[2], enum pipe_mode mode)
{
  int retval = 0;

  if (NULL != pipe_fds && 0 <= pipe_fds[mode])
  {
    retval = close(pipe_fds[mode]);
    if (-1 == retval)
    {
      goto error;
    }

    pipe_fds[mode] = -2;
  }

  return;

error:
  perror("close fd");
  exit(EXIT_FAILURE);
}

void reverse_list(struct command **first_command_ptr)
{
  struct command *current_command = NULL;
  struct command *prev_command = NULL;
  struct command *next_command = NULL;

  current_command = *first_command_ptr;

  while (NULL != current_command)
  {
    next_command = current_command->c_next;
    current_command->c_next = prev_command;
    prev_command = current_command;
    current_command = next_command;
  }

  *first_command_ptr = prev_command;
}

void notify_command_error(char *command_name, char **command_args)
{
  if (NULL == command_name)
  {
    return;
  }

  fprintf(stderr, "The command %s ", command_name);

  // Skip the first argument
  if (NULL != command_args)
  {
    command_args++;
  }

  while (NULL != command_args)
  {
    fprintf(stderr, "%s ", command_args);
    command_args++;
  }

  fprintf(stderr, "was finished with an error\n");
}
