#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
  float f_num = 1.0;

  for (long int i = 0; i < LONG_MAX; i++) {
    printf("%f\n", f_num);
    f_num++;
  }

  exit(EXIT_SUCCESS);
}
