#include <stdio.h>

int main() {
  char str[] = {'A', 0, 0, 0, 'B', 0, 0, 0};

  struct without_packed {
    char c;
    int num;
  };

  struct with_packed {
    char c;
    int num;
  } __attribute__((__packed__));

  struct without_packed* ptr_struct_wop = (struct without_packed*) str;
  struct with_packed* ptr_struct_wp = (struct with_packed*) str;

  printf("Size of a structure without the 'packed' attribute in bytes: %lu\n", sizeof(struct without_packed));
  printf("Size of a structure with the 'packed' attribute in bytes: %lu\n\n", sizeof(struct with_packed));

  printf("From first element of the array\n\n");
  printf("Plain:\n");
  printf("without_packed.c = %c\n", ptr_struct_wop->c);
  printf("without_packed.num = %d\n", ptr_struct_wop->num);
  printf("with_packed.c = %c\n", ptr_struct_wp->c);
  printf("with_packed.num = %d\n\n", ptr_struct_wp->num);
  printf("Hex:\n");
  printf("without_packed.c = %x\n", ptr_struct_wop->c);
  printf("without_packed.num = %x\n", ptr_struct_wop->num);
  printf("with_packed.c = %x\n", ptr_struct_wp->c);
  printf("with_packed.num = %x\n", ptr_struct_wp->num);

  ptr_struct_wop++;
  ptr_struct_wp++;

  printf("\n\nAfter increment of the structure pointers:\n\n");
  printf("Plain:\n");
  printf("without_packed.c = %c\n", ptr_struct_wop->c);
  printf("without_packed.num = %d\n", ptr_struct_wop->num);
  printf("with_packed.c = %c\n", ptr_struct_wp->c);
  printf("with_packed.num = %d\n\n", ptr_struct_wp->num);
  printf("Hex:\n");
  printf("without_packed.c = %x\n", ptr_struct_wop->c);
  printf("without_packed.num = %x\n", ptr_struct_wop->num);
  printf("with_packed.c = %x\n", ptr_struct_wp->c);
  printf("with_packed.num = %x\n", ptr_struct_wp->num);

  return 0;
}
