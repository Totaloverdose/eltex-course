#include "my_strerror.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct thread_struct {
  pthread_t tid;
  int thread_num;
  int error_number;
} thread_struct;


void *thread_work(void *thread_ptr);


int main()
{
  const int THREADS_NUM = 10;

  struct thread_struct threads[THREADS_NUM];
  int retval = 0;


  for (int i = 0; i < THREADS_NUM; i++) {
    threads[i].thread_num = i + 1;
    threads[i].error_number = i - 1;
    retval = pthread_create(&threads[i].tid, NULL, thread_work, &threads[i]);
    if (0 != retval) {
      perror("pthread_create");
      goto finally;
    }
  }

  for (int i = 0; i < THREADS_NUM - 2; i++) {
    retval = pthread_join(threads[i].tid, NULL);
    if (0 != retval) {
      perror("pthread_join");
      goto finally;
    }
  }


 finally:
  return retval;
}

void *thread_work(void *thread_ptr)
{
  struct thread_struct *thread = thread_ptr;

  printf("Thread %d: trying to get the error %d\n", thread->thread_num, thread->error_number);
  printf("%s\n\n", my_strerror(thread->error_number));
  
  return NULL;
}
