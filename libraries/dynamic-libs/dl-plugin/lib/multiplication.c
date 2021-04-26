#include <stdio.h>

#include "../plugin_interface.h"


struct function_info info0 = {
  "multiplication",
  "Multiply two integer numbers"
};


void multiplication(long a, long b) {
  printf("Result: %ld\n", a * b);
}
