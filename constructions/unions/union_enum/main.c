#include <stdio.h>
#include <limits.h>

int main() {
  enum brothers { SMALL_CHAR, MIDDLE_SHORT, OLD_INT};
  union brothers_union {
    char smallest;
    short middle;
    int oldest;
  };

  union brothers_union three_brothers;
  three_brothers.oldest = INT_MAX;

  for (enum brothers brothers_counter = SMALL_CHAR; brothers_counter <= OLD_INT; brothers_counter++) {
    switch (brothers_counter) {
    case SMALL_CHAR:
      printf("The smallest brother was not genius... His IQ was %d.\n", three_brothers.smallest);
      break;
    case MIDDLE_SHORT:
      printf("The middle brother was little smarter. He could even talk... His IQ was %d.\n",
             three_brothers.middle);
      break;
    case OLD_INT:
      printf("The head of the oldest brother was bigger than giant watermelon. "
             "If he doesn't die from starving, he even could be helpful for humanity... "
             "His IQ was %d.\n", three_brothers.oldest);
      break;
    }
  }

  return 0;
}
