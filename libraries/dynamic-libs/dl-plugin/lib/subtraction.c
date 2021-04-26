#include <stdio.h>

#include "../plugin_interface.h"


struct function_info info0 = {
  "subtraction",
  "Subtract the second integer number from the first"
};


void subtraction (long reduced, long subtracted) {
  printf("Result: %ld\n", reduced - subtracted);
}
