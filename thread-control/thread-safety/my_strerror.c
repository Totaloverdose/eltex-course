#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


static pthread_key_t key;
static pthread_once_t func_locker = PTHREAD_ONCE_INIT;


static void make_key_disposable()
{
  int retval = pthread_key_create(&key, NULL);
  if (0 != retval) {
    fprintf(stderr, "pthread_key_create: %s\n", strerror(retval));
    exit(EXIT_FAILURE);
  }
}

char *my_strerror(int err_num)
{
  enum {ERR_STRING_SIZE = 1024};
  
  void *error_string;

  static char const error_strings[6][ERR_STRING_SIZE] = {
    "First error pack my life",
    "Sceond eoerr",
    "Bird error",
    "Cucumber Rick hello from 4th error",
    "Plasma Exchange Whoop-de-doo Mammoth Fur Relay is here (5)",
    "How many error codes do I need? This is already seventh!"
  };

  
  int retval = pthread_once(&func_locker, make_key_disposable);
  if (0 != retval) {
    fprintf(stderr, "pthread_once: %s\n", strerror(retval));
    exit(EXIT_FAILURE);
  }

  error_string = pthread_getspecific(key);
  if (NULL == error_string) {
    error_string = malloc(ERR_STRING_SIZE);
    retval = pthread_setspecific(key, error_string);
    if (0 != retval) {
      fprintf(stderr, "pthread_setspecific: %s\n", strerror(retval));
      exit(EXIT_FAILURE);
    }
  }

  if (1 > err_num || 6 < err_num) {
    strcpy(error_string, "Unknown error");
  } else {
    strcpy(error_string, error_strings[err_num - 1]);
  }

  return error_string;
}
