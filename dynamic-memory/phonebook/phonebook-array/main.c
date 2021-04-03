#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define PHONEBOOK_INITIAL_SIZE 8
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
// IF -1 RETURNED - THIS IS ABNORMAL BEHAVIOUR
int find_empty(bool* is_empty, int phonebook_size);

void command_add_subscriber(struct subscriber* subscribers, bool* is_empty,
                            int* subscribers_count, int phonebook_size);
void command_show_all_subscribers(struct subscriber* subscribers, bool* is_empty,
                                  int phonebook_size, int subscribers_count);
void command_find_subscriber(struct subscriber* subscribers, bool* is_empty,
                             int phonebook_size);
void command_delete_subscriber(struct subscriber* subscribers, bool* is_empty, int* subscribers_count,
                               int phonebook_size);

void print_menu();
void print_subscriber(struct subscriber, int serial_num);
void clear_buf();
// Deletes newline at the end of the string if exists
void delete_newline(char*);
// Initialazes structures' strings with first null-terminal character
// from first_index to (last_index - 1)
void subscribers_initialize(struct subscriber* subscribers, bool* is_empty,
                            int first_index, int last_index);
// Increase the size of the phonebook by 2 times
void phonebook_expand(struct subscriber** subscribers, bool** is_empty, int* current_phonebook_size);
// Move the subscribers to start of array
void phonebook_realign(struct subscriber* subscribers, bool* is_empty, int subscribers_count);
// Decrease the size of the phonebook by 2 times
void phonebook_shrink(struct subscriber** subscribers, bool** is_empty, int* current_phonebook_size);


int main() {
  struct subscriber* subscribers =
    (struct subscriber*) calloc(PHONEBOOK_INITIAL_SIZE, sizeof(struct subscriber));
  bool* is_empty = (bool*) calloc(PHONEBOOK_INITIAL_SIZE, sizeof(bool));
  int current_phonebook_size = PHONEBOOK_INITIAL_SIZE;
  int subscribers_count = 0;
  int command_num = 0;

  subscribers_initialize(subscribers, is_empty, 0, current_phonebook_size);

  printf("WELCOME TO THE BEST PHONEBOOK EVER!!!");

  while (true) {
    print_menu();

    scanf(" %d", &command_num);
    clear_buf();
    printf("\n\n");

    switch (command_num) {
    
    case 1:
      if (subscribers_count == current_phonebook_size) {
        phonebook_expand(&subscribers, &is_empty, &current_phonebook_size);
      }
      command_add_subscriber(subscribers, is_empty, &subscribers_count, current_phonebook_size);
      break;
    
    case 2:
      command_show_all_subscribers(subscribers, is_empty, current_phonebook_size, subscribers_count);
      break;

    case 3:
      command_find_subscriber(subscribers, is_empty, current_phonebook_size);
      break;

    case 4:
      command_delete_subscriber(subscribers, is_empty, &subscribers_count, current_phonebook_size);
      if (subscribers_count <= (current_phonebook_size / 4) &&
          subscribers_count >= (PHONEBOOK_INITIAL_SIZE) / 2) {
        phonebook_realign(subscribers, is_empty, subscribers_count);
        phonebook_shrink(&subscribers, &is_empty, &current_phonebook_size);
      }
      break;

    case 5:
      printf("Goodbye! :)\n");
      free(subscribers);
      free(is_empty);
      return 0;

    default:
      printf("Sorry, a command with this number does not exist");
      break;
    }

    command_num = 0;
  }

  return 1;
}

void command_add_subscriber(struct subscriber* subscribers, bool* is_empty,
                            int* subscribers_count, int phonebook_size)
{
  char name[MAX_NAME_LENGTH] = {0};
  char surname[MAX_SURNAME_LENGTH] = {0};
  char phone_number[MAX_PHONE_NUMBER_LENGTH] = {0};
  int available_num = find_empty(is_empty, phonebook_size);

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
    (*subscribers_count)++;

    printf("Added subscriber: %s %s, phone number is %s", name, surname, phone_number);
  } else {
    printf("Sorry, there is no available space for a new subscriber.\n"
           "If you want to add them, you should delete one of unneeded");
  }
}

void command_show_all_subscribers(struct subscriber* subscribers, bool* is_empty,
                                  int phonebook_size, int subscribers_count)
{
  if (subscribers_count == 0) {
    printf("You have no subscribers in your phonebook\n");
    return;
  }
  
  int serial_num = 1;
  for (int i = 0; i < phonebook_size; i++) {
    if (!is_empty[i]) {
      print_subscriber(subscribers[i], serial_num);
      serial_num++;
    }
  }
}

void command_find_subscriber(struct subscriber* subscribers, bool* is_empty, int phonebook_size)
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

  for (int i = 0; i < phonebook_size; i++) {
    if (!is_empty[i]) {
      // False (zero) if the name or surname fit
      bool does_name_fit = !strcmp(name_to_find, subscribers[i].name);
      bool does_surname_fit = !strcmp(surname_to_find, "");
      if (!does_surname_fit) {
        does_surname_fit = !strcmp(surname_to_find, subscribers[i].surname);
      }
      
      if (does_name_fit && does_surname_fit) {
        print_subscriber(subscribers[i], serial_num);
        serial_num++;
      }
    }
  }
}

void command_delete_subscriber(struct subscriber* subscribers, bool* is_empty,
                               int* subscribers_count, int phonebook_size)
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
  
  for (int i = 0; i < phonebook_size; i++) {
    if (!is_empty[i]) {
      // False (zero) if the name or surname fit
      bool does_name_fit = !strcmp(name_to_delete, subscribers[i].name);
      bool does_surname_fit = !strcmp(surname_to_delete, subscribers[i].surname);
      
      if (does_name_fit && does_surname_fit) {
        is_empty[i] = true;
        (*subscribers_count)--;
        break;
      }
    }
  }
}

int find_empty(bool* is_empty, int phonebook_size)
{
  for (int i = 0; i < phonebook_size; i++) {
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
  printf("%2d) %-13s %30s\n", serial_num, "Name:", sbscr.name);
  printf("    %-13s %30s\n", "Surname:", sbscr.surname);
  printf("    %-13s %30s\n\n", "Phone number:", sbscr.phone_number);
}

void subscribers_initialize(struct subscriber* subscribers, bool* is_empty,
                            int first_index, int last_index)
{
  for (int i = first_index; i < last_index; i++) {
    subscribers[i].name[0] = 0;
    subscribers[i].surname[0] = 0;
    subscribers[i].phone_number[0] = 0;
    is_empty[i] = true;
  }
}

void phonebook_expand(struct subscriber** subscribers, bool** is_empty, int* current_phonebook_size)
{
  int last_phonebook_size = *current_phonebook_size;
  *current_phonebook_size *= 2;
  
  *subscribers = ((struct subscriber*)
                  reallocarray(*subscribers, *current_phonebook_size, sizeof(struct subscriber)));
  *is_empty = (bool*) reallocarray(*is_empty, *current_phonebook_size, sizeof(bool));

  subscribers_initialize(*subscribers, *is_empty, last_phonebook_size, *current_phonebook_size);
}

void phonebook_realign(struct subscriber* subscribers, bool* is_empty, int subscribers_count)
{
  for (int i = 0, j = 0; i < subscribers_count; i++, j++) {
    while (is_empty[j]) {
      j++;
    }

    if (i != j) {
      strcpy(subscribers[i].name, subscribers[j].name);
      strcpy(subscribers[i].surname, subscribers[j].surname);
      strcpy(subscribers[i].phone_number, subscribers[j].phone_number);

      is_empty[j] = true;
      is_empty[i] = false;
    }
  }
}

void phonebook_shrink(struct subscriber** subscribers, bool** is_empty, int* current_phonebook_size)
{
  *current_phonebook_size /= 2;

  *subscribers = ((struct subscriber*)
                  reallocarray(*subscribers, *current_phonebook_size, sizeof(struct subscriber)));
  *is_empty = (bool*) reallocarray(*is_empty, *current_phonebook_size, sizeof(bool));
}
