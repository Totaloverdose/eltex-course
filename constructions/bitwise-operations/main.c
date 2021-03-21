#include <stdio.h>
#include <stdlib.h>

void print_every_byte(uint);

int main() {
  uint a = 0;
  uint new_third_byte = 0;
    
  printf("Enter a number from 0 to 4 294 967 295 (4 byte):\n");
  scanf(" %d", &a);

  print_every_byte(a);

  printf("\n\nType number 0-255 with which you want to replace the third byte:\n");
  scanf(" %d", &new_third_byte);

  // Change third byte in a to the first byte from new_third_byte
  new_third_byte &= 0x000000FF;
  new_third_byte <<= 16;
  a &= 0xFF00FFFF;
  a |= new_third_byte;

  print_every_byte(a);
}

void print_every_byte(uint num) {
  uint result = num;
  
  printf("\nDecimal representation: %u\n", result);
  for (int i = 0; i < 4; i++) {
    printf("Byte %d: %d\n", (i + 1), (result & 0xFF));
    result = result >> 8;
  }

  result = num;
  printf("\nThe whole hex number: %x\n", result);
  for (int i = 0; i < 4; i++) {
    printf("Byte %d: %x\n", (i + 1), (result & 0xFF));
    result >>= 8;
  }
}
