#include <stdio.h>
#include <stdlib.h>

int main() {
  int a = 4;
  
  for (long i = 0; i < 1000000000; i++) {
    if (a == 1) {
      a = 5;
    } else if (a == 2) {
      a = 10;
    } else if (a == 3) {
      a = 7;
    } else if (a == 4) {
      a = 1;
    } else if (a == 5) {
      a = 3;
    } else if (a == 6) {
      a = 8;
    } else if (a == 7) {
      a = 2;
    } else if (a == 8) {
      a = 6;
    } else if (a == 9) {
      a = 4;
    } else if (a == 10) {
      a = 9;
    }
  }

  exit(EXIT_SUCCESS);
}
