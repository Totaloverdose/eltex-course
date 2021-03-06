#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
  register long a = 0;
  clock_t start, end;
  double exec_time;

  start = clock();
  for (int i = 0; i < 1000000; i++) {
    a += i * i + 5;
  }
  end = clock();

  exec_time = ((double) (end - start)) / CLOCKS_PER_SEC;
  printf("Программа выполнилась за %lf секунд\n", exec_time);

  exit(EXIT_SUCCESS);
}
  
