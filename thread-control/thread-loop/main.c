#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define THREADS_COUNT 10
#define ITERS_COUNT 10000000


unsigned long a;

void* start_routine(void* n)
{
  int iterations_count = * (int*) n;
  
  for (int i = 0; i < iterations_count; i++) {
    int tmp = a;
    tmp++;
    a = tmp;
  }

  printf("a = %lu\n", a);
  
  return NULL;
}

int main()
{
  const int iterations_count = ITERS_COUNT;
  int retval = 0;
  a = 0;
  pthread_t thread_ids[THREADS_COUNT];
  
  for (int i = 0; i < THREADS_COUNT; i++) {
    retval = pthread_create(&thread_ids[i], NULL, start_routine, (int*) &iterations_count);

    if (0 != retval) {
      fprintf(stderr, "Can't create a new thread\n");
      exit(EXIT_FAILURE);
    }
  }

  for (int i = 0; i < THREADS_COUNT; i++) {
    retval = pthread_join(thread_ids[i], NULL);

    if (0 != retval) {
      fprintf(stderr, "An unsuccessful attempt to pthread_join (%lu)\n", thread_ids[i]);
    }
  }
}
