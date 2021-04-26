#include <stdio.h>

#include "../plugin_interface.h"


struct function_info info0 = {
  "division",
  "Divide two integer numbers"
};

struct function_info info1 = {
  "test_fun",
  "This is a test function (description)"
};


void division(long divisible, long divisor) {
  if (divisor != 0) {
    printf("Result: %ld\n", divisible / divisor);
  } else {
    printf("Can't divide by 0\n");
  }
}

void test_fun(long a, long b)
{
  printf("Two numbers was given to the test function: %ld, %ld\n", a, b);
}
