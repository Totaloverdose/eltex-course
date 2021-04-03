#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME_LENGTH 30
#define MAX_SURNAME_LENGTH 30
#define MAX_PHONE_NUMBER_LENGTH 20


struct subscriber {
  char name[MAX_NAME_LENGTH];
  char surname[MAX_SURNAME_LENGTH];
  char phone_number[MAX_PHONE_NUMBER_LENGTH];

  struct subscriber* next;
};

void command_add_subscriber(struct subscriber** last_subscriber);
void command_show_all_subscribers(struct subscriber* first_subscriber);
void command_find_subscriber(struct subscriber* first_subscriber);
void command_delete_subscriber(struct subscriber** first_subscriber, struct subscriber** last_subscriber);

void print_menu();
void print_subscriber(struct subscriber, int serial_num);
void clear_buf();
// Deletes newline at the end of the string if exists
void delete_newline(char*);

void free_phonebook(struct subscriber* first_subscriber);

struct subscriber* create_subscriber(char* name, char* surname, char* phone_number);


int main() {
  struct subscriber* first_subscriber = NULL;
  struct subscriber* last_subscriber = NULL;
  int command_num = 0;

  printf("WELCOME TO THE BEST PHONEBOOK EVER!!!");

  while (true) {
    print_menu();

    scanf(" %d", &command_num);
    clear_buf();
    printf("\n\n");

    switch (command_num) {
    
    case 1:
      command_add_subscriber(&last_subscriber);
      break;
    
    case 2:
      command_show_all_subscribers(first_subscriber);
      break;

    case 3:
      command_find_subscriber(first_subscriber);
      break;

    case 4:
      command_delete_subscriber(&first_subscriber, &last_subscriber);
      break;

    case 5:
      printf("Goodbye! :)\n");
      free_phonebook(first_subscriber);
      first_subscriber = NULL;
      last_subscriber = NULL;
      return 0;

    default:
      printf("Sorry, a command with this number does not exist");
      break;
    }

    command_num = 0;
    if (first_subscriber == NULL) {
      first_subscriber = last_subscriber;
    }
  }

  return 1;
}

void command_add_subscriber(struct subscriber** last_subscriber)
{
  char name[MAX_NAME_LENGTH] = {0};
  char surname[MAX_SURNAME_LENGTH] = {0};
  char phone_number[MAX_PHONE_NUMBER_LENGTH] = {0};
  
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
    if (*last_subscriber == NULL) {
      *last_subscriber = create_subscriber(name, surname, phone_number);
    } else {
      (*last_subscriber)->next = create_subscriber(name, surname, phone_number);
      *last_subscriber = (*last_subscriber)->next;
    }
  } else {
    printf("Cannot add the subscriber: incorrect data was given");
    return;
  }

  printf("Added subscriber: %s %s, phone number is %s",
         (*last_subscriber)->name, (*last_subscriber)->surname, (*last_subscriber)->phone_number);
}

void command_show_all_subscribers(struct subscriber* first_subscriber)
{
  int serial_num = 1;
  struct subscriber* sub = first_subscriber;

  if (sub == NULL) {
    printf("Sorry, there is no subscribers in your phonebook yet :(\n");
  }
  
  while (sub != NULL) {
    print_subscriber(*sub, serial_num);
    serial_num++;
    sub = sub->next;
  }
}

void command_find_subscriber(struct subscriber* first_subscriber)
{
  char name_to_find[MAX_NAME_LENGTH];
  char surname_to_find[MAX_SURNAME_LENGTH];
  int serial_num = 1;
  struct subscriber* sub = first_subscriber;
  
  printf("Enter the name of subscriber you want to find: ");
  fgets(name_to_find, MAX_NAME_LENGTH, stdin);
  delete_newline(name_to_find);

  printf("Enter the surname of subscriber you want to find or "
         "leave it empty: ");
  fgets(surname_to_find, MAX_SURNAME_LENGTH, stdin);
  delete_newline(surname_to_find);
  printf("\n");

  while (sub != NULL) {
    bool does_name_fit = !strcmp(name_to_find, sub->name);
    bool does_surname_fit = !strcmp(surname_to_find, "");
    
    if (!does_surname_fit) {
      does_surname_fit = !strcmp(surname_to_find, sub->surname);
    }
      
    if (does_name_fit && does_surname_fit) {
      print_subscriber(*sub, serial_num);
      serial_num++;
    }
    
    sub = sub->next;
  }
}

void command_delete_subscriber(struct subscriber** first_subscriber, struct subscriber** last_subscriber)
{
  char name_to_delete[MAX_NAME_LENGTH];
  char surname_to_delete[MAX_SURNAME_LENGTH];
  struct subscriber* sub = *first_subscriber;
  struct subscriber* prev_sub = NULL;
  struct subscriber* next_sub = NULL;

  printf("Enter the name of subscriber you want to delete: ");
  fgets(name_to_delete, MAX_NAME_LENGTH, stdin);
  delete_newline(name_to_delete);

  printf("Enter the surname of subscriber you want to delete: ");
  fgets(surname_to_delete, MAX_SURNAME_LENGTH, stdin);
  delete_newline(surname_to_delete);
  printf("\n");
  
  while (sub != NULL) {
    next_sub = sub->next;
    
    bool does_name_fit = !strcmp(name_to_delete, sub->name);
    bool does_surname_fit = !strcmp(surname_to_delete, sub->surname);
      
    if (does_name_fit && does_surname_fit) {
      if (prev_sub == NULL && *first_subscriber != NULL) {
        free(*first_subscriber);
        *first_subscriber = next_sub;
        if ((*first_subscriber) == NULL) {
          *last_subscriber = NULL;
        }
      } else if (next_sub == NULL && *last_subscriber != NULL) {
        free(*last_subscriber);
        *last_subscriber = prev_sub;
        (*last_subscriber)->next = NULL;
      } else {
        free(sub);
        prev_sub->next = next_sub;
      }
      
      break;
    }

    prev_sub = sub;
    sub = next_sub;
  }
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
  printf("%2d) %-13s %30s\n", serial_num, "Name:", sbscr.name);
  printf("    %-13s %30s\n", "Surname:", sbscr.surname);
  printf("    %-13s %30s\n\n", "Phone number:", sbscr.phone_number);
}

struct subscriber* create_subscriber(char* name, char* surname, char* phone_number)
{
  struct subscriber* new_subscriber = (struct subscriber*) malloc(sizeof(struct subscriber));
  strcpy(new_subscriber->name, name);
  strcpy(new_subscriber->surname, surname);
  strcpy(new_subscriber->phone_number, phone_number);

  return new_subscriber;
}

void free_phonebook(struct subscriber* first_subscriber)
{
  struct subscriber* sub = first_subscriber;

  while (sub != NULL) {
    struct subscriber* next_sub = sub->next;
    free(sub);
    sub = next_sub;
  }
}
