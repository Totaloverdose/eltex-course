#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


void print_pid_ppid(int pnum);

int main()
{
  pid_t pid2;
  pid_t pid3;

  pid2 = fork();

  if (0 == pid2) {
    int pid4;
    int pid5;

    pid4 = fork();

    if (0 == pid4) {
      print_pid_ppid(4);
      exit(EXIT_SUCCESS);
    } else {
      wait(NULL);
    }

    pid5 = fork();

    if (0 == pid5) {
      print_pid_ppid(5);
      exit(EXIT_SUCCESS);
    } else {
      wait(NULL);
    }

    print_pid_ppid(2);
    exit(EXIT_SUCCESS);
  } else {
    wait(NULL);
  }

  pid3 = fork();

  if (0 == pid3) {
    int pid6;

    pid6 = fork();

    if (0 == pid6) {
      print_pid_ppid(6);
      exit(EXIT_SUCCESS);
    } else {
      wait(NULL);
    }

    print_pid_ppid(3);
    exit(EXIT_SUCCESS);
  } else {
    wait(NULL);
  }

  print_pid_ppid(1);
}


void print_pid_ppid(int pnum)
{
  printf("%d: I have pid %d and my parent has pid %d\n", pnum,  getpid(), getppid());
}
