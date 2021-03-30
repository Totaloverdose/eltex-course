#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define PHONEBOOK_SIZE 10
#define MAX_NAME_LENGTH 30
#define MAX_SURNAME_LENGTH 30
#define MAX_PHONE_NUMBER_LENGTH 20


struct subscriber {
  char name[MAX_NAME_LENGTH];
  char surname[MAX_SURNAME_LENGTH];
  char phone_number[MAX_PHONE_NUMBER_LENGTH];
};


// returns first index with the 'true' value by it
// if such an index does not exist, then return -1
int find_empty(bool*);

void command_add_subscriber(struct subscriber*, bool*);
void command_show_all_subscribers(struct subscriber*, bool*);
void command_find_subscriber(struct subscriber*, bool*);
void command_delete_subscriber(struct subscriber*, bool*);

void print_menu();
void print_subscriber(struct subscriber, int serial_num);
void clear_buf();
// Deletes newline at the end of the string if exists
void delete_newline(char*);


int main() {
  struct subscriber subscribers[PHONEBOOK_SIZE];
  bool is_empty[PHONEBOOK_SIZE];
  int command_num = 0;

  for (int i = 0; i < PHONEBOOK_SIZE; i++) {
    subscribers[i].name[0] = 0;
    subscribers[i].surname[0] = 0;
    subscribers[i].phone_number[0] = 0;
    is_empty[i] = true;
  }

  printf("WELCOME TO THE BEST PHONEBOOK EVER!!!");

  while (true) {
    print_menu();

    scanf(" %d", &command_num);
    clear_buf();
    printf("\n\n");

    switch (command_num) {
    
    case 1:
      command_add_subscriber(subscribers, is_empty);
      break;
    
    case 2:
      command_show_all_subscribers(subscribers, is_empty);
      break;

    case 3:
      command_find_subscriber(subscribers, is_empty);
      break;

    case 4:
      command_delete_subscriber(subscribers, is_empty);
      break;

    case 5:
      printf("Goodbye! :)\n");
      return 0;

    default:
      printf("Sorry, a command with this number does not exist");
      break;
    }

    command_num = 0;
  }

  return 1;
}

void command_add_subscriber(struct subscriber* subscribers, bool* is_empty)
{
  char name[MAX_NAME_LENGTH] = {0};
  char surname[MAX_SURNAME_LENGTH] = {0};
  char phone_number[MAX_PHONE_NUMBER_LENGTH] = {0};
  int available_num = find_empty(is_empty);

  if (0 <= available_num) {
    printf("Enter the name of the subscriber: ");
    fgets(name, MAX_NAME_LENGTH, stdin);
    delete_newline(name);

    printf("Enter the surname of the subscriber: ");
    fgets(surname, MAX_SURNAME_LENGTH, stdin);
    delete_newline(surname);

    printf("Enter the phone number of the subscriber: ");
    fgets(phone_number, MAX_PHONE_NUMBER_LENGTH, stdin);
    delete_newline(phone_number);

    printf("\n\n");

    // If each field is not empty
    if (name[0] && surname[0] && phone_number[0]){
      strcpy(subscribers[available_num].name, name);
      strcpy(subscribers[available_num].surname, surname);
      strcpy(subscribers[available_num].phone_number, phone_number);
    } else {
      printf("Cannot add the subscriber: incorrect data was given");
      return;
    }

    is_empty[available_num] = false;

    printf("Added subscriber: %s %s, phone number is %s", name, surname, phone_number);
  } else {
    printf("Sorry, there is no available space for a new subscriber.\n"
           "If you want to add them, you should delete one of unneeded");
  }
}

void command_show_all_subscribers(struct subscriber* subscribers, bool* is_empty)
{
  int serial_num = 1;
  for (int i = 0; i < PHONEBOOK_SIZE; i++) {
    if (!is_empty[i]) {
      print_subscriber(subscribers[i], serial_num);
      serial_num++;
    }
  }
}

void command_find_subscriber(struct subscriber* subscribers, bool* is_empty)
{
  char name_to_find[MAX_NAME_LENGTH];
  char surname_to_find[MAX_SURNAME_LENGTH];
  int serial_num = 1;
  
  printf("Enter the name of subscriber you want to find: ");
  fgets(name_to_find, MAX_NAME_LENGTH, stdin);
  delete_newline(name_to_find);

  printf("Enter the surname of subscriber you want to find or "
         "leave it empty: ");
  fgets(surname_to_find, MAX_SURNAME_LENGTH, stdin);
  delete_newline(surname_to_find);
  printf("\n");

  for (int i = 0; i < PHONEBOOK_SIZE; i++) {
    if (!is_empty[i]) {
      // False (zero) if the name or surname fit
      bool does_name_fit_not = strcmp(name_to_find, subscribers[i].name);
      bool does_surname_fit_not = strcmp(surname_to_find, "");
      if (does_surname_fit_not) {
        does_surname_fit_not = strcmp(surname_to_find, subscribers[i].surname);
      }
      
      if (!does_name_fit_not && !does_surname_fit_not) {
        print_subscriber(subscribers[i], serial_num);
        serial_num++;
      }
    }
  }
}

void command_delete_subscriber(struct subscriber* subscribers, bool* is_empty)
{
  char name_to_delete[MAX_NAME_LENGTH];
  char surname_to_delete[MAX_SURNAME_LENGTH];

  printf("Enter the name of subscriber you want to delete: ");
  fgets(name_to_delete, MAX_NAME_LENGTH, stdin);
  delete_newline(name_to_delete);

  printf("Enter the surname of subscriber you want to delete: ");
  fgets(surname_to_delete, MAX_SURNAME_LENGTH, stdin);
  delete_newline(surname_to_delete);
  printf("\n");
  
  for (int i = 0; i < PHONEBOOK_SIZE; i++) {
    if (!is_empty[i]) {
      // False (zero) if the name or surname fit
      bool does_name_fit_not = strcmp(name_to_delete, subscribers[i].name);
      bool does_surname_fit_not = strcmp(surname_to_delete, subscribers[i].surname);
      
      if (!does_name_fit_not && !does_surname_fit_not) {
        is_empty[i] = true;
        break;
      }
    }
  }
}

int find_empty(bool* is_empty)
{
  for (int i = 0; i < PHONEBOOK_SIZE; i++) {
    if (true == is_empty[i]) {
      return i;
    }
  }

  return -1;
}

void clear_buf()
{
  char buf_c;
  while ((buf_c = getchar()) != '\n' && buf_c != EOF);
}

void delete_newline(char* str)
{
  int str_l = strlen(str);
  if ((0 != str_l) && ('\n' == str[str_l - 1])) {
    str[str_l - 1] = 0;
  }
}

void print_menu()
{
  printf("\n\n");
  printf("You can choose the one of following commands by typing its number:\n");
  printf("1 -- Add a subscriber\n");
  printf("2 -- Show all subscribers\n");
  printf("3 -- Find the subscriber\n");
  printf("4 -- Delete the subscriber\n");
  printf("5 -- Exit from your favorite phonebook ;)\n");
  printf("And your choice: ");
}

void print_subscriber(struct subscriber sbscr, int serial_num)
{
  printf("%2d) %-13s %s\n", serial_num, "Name:", sbscr.name);
  printf("    %-13s %5s\n", "Surname:", sbscr.surname);
  printf("    %-13s %5s\n\n", "Phone number:", sbscr.phone_number);
}
