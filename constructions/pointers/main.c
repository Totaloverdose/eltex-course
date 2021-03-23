#include <stdio.h>

int main() {
  int num = 270533154;
  char* num_byte_ptr = (char*) &num;

  printf("Number in dec: %d\n", num);
  printf("Number in hex: %x\n\n", num);
  for (int i = 0; i < 4; i++) {
    printf("Byte %d in dec: %hhd\n", i+1,  *(num_byte_ptr + i));
    printf("Byte %d in hex: %hhx\n\n", i+1, *(num_byte_ptr + i));
  }

  num_byte_ptr += 2;
  *num_byte_ptr = 0xFF;
  num_byte_ptr = (char*) &num;

  printf("Number in dec: %d\n", num);
  printf("Number in hex: %x\n\n", num);
  for (int i = 0; i < 4; i++) {
    printf("Byte %d in dec: %hhd\n", i+1,  *(num_byte_ptr + i));
    printf("Byte %d in hex: %hhx\n\n", i+1, *(num_byte_ptr + i));
  }
}
