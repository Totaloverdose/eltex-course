#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "kinda-string.txt"
#define STRING_TEXT "cucumba\n"

void print_critical_error(const int line, const char* func_name);

int main() {
  int fd = open(FILE_NAME, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
  int ret_num = 0;
  char read_char;
  const int STRING_LENGTH = strlen(STRING_TEXT);
  
  if (0 > fd) {
    ret_num = remove(FILE_NAME);
    if (EACCES == errno && ret_num) {
      fd = creat(FILE_NAME, S_IRUSR | S_IWUSR);
    }
  }
  if (0 > fd) {
    print_critical_error(__LINE__, __FUNCTION__);
  }

  ret_num = write(fd, STRING_TEXT, STRING_LENGTH);
  if (0 > ret_num) {
    print_critical_error(__LINE__, __FUNCTION__);
  }

  ret_num = close(fd);
  if (0 > ret_num) {
    print_critical_error(__LINE__, __FUNCTION__);
  }



  
  ret_num = open(FILE_NAME, O_RDONLY);
  if (0 > ret_num) {
    print_critical_error(__LINE__, __FUNCTION__);
  }

  for (int i = 0; i < STRING_LENGTH; i++) {
    ret_num = lseek(fd, -(i+1), SEEK_END);
    if (0 > ret_num) {
      print_critical_error(__LINE__, __FUNCTION__);
    }
    
    ret_num = read(fd, &read_char, 1);
    if (0 > ret_num) {
      print_critical_error(__LINE__, __FUNCTION__);
    }

    printf("%c", read_char);
  }

  printf("\n");
  return 0;
}

void print_critical_error(const int line, const char* func_name)
{
    char* msg = malloc(1024);
    sprintf(msg, "Line %d, function %s", line, func_name);
    perror(msg);

    free(msg);
    
    exit(EXIT_FAILURE);
}
