#include <stdio.h>
#include "libmycalc.h"
#include <stdbool.h>
#include <stdlib.h>

void print_menu();
void command_sum();
void command_subtraction();
void command_division();
void command_multiplication();
void command_exit();
void command_not_exist();
void get_two_numbers_for(long* first_num, long* second_num, char* operation_name);

int main() {
  enum commands { NONE, SUM, SUBTRACTION, DIVISION, MULTIPLICATION, EXIT };
  enum commands command_num = NONE;

  printf("WELCOME TO THE BEST CALCULATOR EVER!!!\n\n");
  
  while (true) {
    print_menu();
    scanf(" %d", &command_num);

    switch (command_num) {
    case SUM:
      command_sum();
      break;
    case SUBTRACTION:
      command_subtraction();
      break;
    case DIVISION:
      command_division();
      break;
    case MULTIPLICATION:
      command_multiplication();
      break;
    case EXIT:
      command_exit();
      break;
    default:
      command_not_exist();
      break;
    }
    
    printf("\n\n");
    command_num = NONE;
  }
}

void print_menu() {
  printf("You can choose one of these commands:\n");
  printf("1 -- Get the sum of two integer numbers\n");
  printf("2 -- Get the difference of two integer numbers\n");
  printf("3 -- Get the division of two integer numbers (without remainder)\n");
  printf("4 -- Get the multiplication of two integer numbers\n");
  printf("5 -- Exit from the best calculator\n");
  printf("And your choice: ");
}

void command_sum() {
  long first_num = 0;
  long second_num = 0;
  
  get_two_numbers_for(&first_num, &second_num, "sum");

  printf("And result: %ld\n", sum(first_num, second_num));
}

void command_subtraction() {
  long first_num = 0;
  long second_num = 0;

  get_two_numbers_for(&first_num, &second_num, "subtraction");

  printf("And result: %ld\n", subtraction(first_num, second_num));
}

void command_division() {
  long first_num = 0;
  long second_num = 0;

  get_two_numbers_for(&first_num, &second_num, "division");

  printf("And result: %ld\n", division(first_num, second_num));
}

void command_multiplication() {
  long first_num = 0;
  long second_num = 0;

  get_two_numbers_for(&first_num, &second_num, "multiplication");

  printf("And result: %ld\n", multiplication(first_num, second_num));
}

void get_two_numbers_for(long* first_num, long* second_num, char* operation_name) {
  printf("Enter two integer numbers to %s: ", operation_name);
  scanf(" %ld %ld", first_num, second_num);
}

void command_exit() {
  printf("Bye! :)\n");
  exit(EXIT_SUCCESS);
}

void command_not_exist() {
  printf("Sorry, command with this number does not exist\n");
}
