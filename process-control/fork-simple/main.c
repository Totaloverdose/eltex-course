#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  pid_t child_pid;

  child_pid = fork();

  if (0 == child_pid) {
    printf("Papa! Big as a mountain and...\n");
    exit(EXIT_SUCCESS);
  } else {
    if (-1 == child_pid) {
      perror("fork: ");
      exit(EXIT_FAILURE);
    }
  
    printf("There is Malphite, I am Malphite.\n");
    wait(NULL);
    printf("... twice as strong. Hah, good memory, Chip.\n");
  }
}
