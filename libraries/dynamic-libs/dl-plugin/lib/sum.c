#include <stdio.h>

#include "../plugin_interface.h"


struct function_info info0 = {
  "sum",
  "Sum two integer numbers"
};


void sum(long a, long b) {
  printf("Result: %ld\n", a + b);
}
