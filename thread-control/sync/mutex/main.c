#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h> // for sleep()
#include <stdbool.h>
#include <string.h>


#define SHOPS_COUNT 5
#define CUSTOMERS_COUNT 3
#define SUPPLIERS_COUNT 5

#define SHOP_PRODUCTS_SCATTER 500
#define CUSTOMER_NEEDS_SCATTER 2500
#define SUPPLIER_PERFORMANCE_SCATTER 250

#define CUSTOMER_SLEEP_TIME 3
#define SUPPLIER_SLEEP_TIME 1


typedef struct customer {
  int c_need;
  int c_num;
  pthread_t c_tid;
} customer;

typedef struct shop {
  int shop_products_count;
  int shop_num;
  pthread_mutex_t coronavirus_mutex;
} shop;

typedef struct supplier {
  int sup_performance;
  int sup_num;
  pthread_t sup_tid;
} supplier;


void* customer_behavior(void* customer_struct);
void* supplier_behavior(void* supplier_struct);


struct shop shops[SHOPS_COUNT];

bool is_shop_closed;


int main()
{
  struct customer customers[CUSTOMERS_COUNT];
  struct supplier suppliers[SUPPLIERS_COUNT];

  time_t current_time = time(NULL);

  int retval = 0;

  is_shop_closed = false;

  
  srandom(current_time);


  printf("Mega Super Duper Puper Realistic World Simulation\n");
  printf("%d shops, %d customers, %d suppliers\n\n\n", SHOPS_COUNT, CUSTOMERS_COUNT, SUPPLIERS_COUNT);
  
  for (int i = 0; i < SHOPS_COUNT; i++) {
    retval = pthread_mutex_init(&shops[i].coronavirus_mutex, NULL);
    if (0 != retval) {
      perror("mutex_init");
      goto finally;
    }
  }
  
  for (int i = 0; i < SHOPS_COUNT; i++) {
    shops[i].shop_products_count = 1000 + (random() % (2 * SHOP_PRODUCTS_SCATTER)) - SHOP_PRODUCTS_SCATTER;
    shops[i].shop_num = i + 1;

    printf("Shop %d has %d products for start\n", shops[i].shop_num, shops[i].shop_products_count);
  }
  printf("\n");

  for (int i = 0; i < CUSTOMERS_COUNT; i++) {
    customers[i].c_need = 5000 + (random() % (2 * CUSTOMER_NEEDS_SCATTER)) - CUSTOMER_NEEDS_SCATTER;
    customers[i].c_num = i + 1;
  }

  for (int i = 0; i < SUPPLIERS_COUNT; i++) {
    suppliers[i].sup_performance =
      500 + (random() % (2 * SUPPLIER_PERFORMANCE_SCATTER)) - SUPPLIER_PERFORMANCE_SCATTER;
    suppliers[i].sup_num = i + 1;
  }


  for (int i = 0; i < CUSTOMERS_COUNT; i++) {
    retval = pthread_create(&customers[i].c_tid, NULL, customer_behavior, &customers[i]);
    if (0 != retval) {
      perror("pthread_create");
      goto finally;
    }
  }

  for (int i = 0; i < SUPPLIERS_COUNT; i++) {
    retval = pthread_create(&suppliers[i].sup_tid, NULL, supplier_behavior, &suppliers[i]);
    if (0 != retval) {
      perror("pthread_create");
      goto finally;
    }
  }


  for (int i = 0; i < CUSTOMERS_COUNT; i++) {
    retval = pthread_join(customers[i].c_tid, NULL);
    if (0 != retval) {
      perror("pthread_join");
      goto finally;
    }
  }

  printf("\n\nBoss: Hey, all the customers are served. From now on we closed down today!\n\n\n");
  is_shop_closed = true;

  for (int i = 0; i < SUPPLIERS_COUNT; i++) {
    retval = pthread_join(suppliers[i].sup_tid, NULL);
    if (0 != retval) {
      perror("pthread_join");
      goto finally;
    }
  }

  
 finally:
  for (int i = 0; i < SHOPS_COUNT; i++) {
    pthread_mutex_destroy(&shops[i].coronavirus_mutex);
  }
  return retval;
}

void* customer_behavior(void *customer_struct)
{
  struct customer *customer = (struct customer *) customer_struct;
  int retval = 0;

  char customer_name[24];
  strcpy(customer_name, "Customer ");
  sprintf(&customer_name[9], "%d", customer->c_num);

  while (0 <= customer->c_need) {
    int i = random() % SHOPS_COUNT; // shop serial number

    printf("%s: I am ready to shopping!!!\n", customer_name);
    printf("%s: I need %d products for now\n", customer_name, customer->c_need);
    
    retval = pthread_mutex_lock(&shops[i].coronavirus_mutex);
    if (0 != retval) {
      perror("mutex_lock");
      exit(EXIT_FAILURE);
    }

    printf("%s: Hooray, I am finally in the shop %d!\n", customer_name, shops[i].shop_num);
    printf("%s: I've bought %d products\n", customer_name, shops[i].shop_products_count);
    
    customer->c_need -= shops[i].shop_products_count;
    shops[i].shop_products_count = 0;

    if (0 <= customer->c_need) {
      printf("%s: But I still need %d products...\n", customer_name, customer->c_need);
    } else {
      printf("%s: And I don't need anymore!\n", customer_name);
    }

    printf("\n");

    retval = pthread_mutex_unlock(&shops[i].coronavirus_mutex);
    if (0 != retval) {
      perror("mutex_unlock");
      exit(EXIT_FAILURE);
    }

    printf("%s: Shopping is tiresome... I need to sleep\n\n", customer_name);
    sleep(CUSTOMER_SLEEP_TIME);
  }

  printf("%s: Ok, I've got all what I need. Let's go home\n", customer_name);
  return NULL;
}

void* supplier_behavior(void *supplier_struct)
{
  struct supplier *supplier = (struct supplier *) supplier_struct;
  int retval = 0;
  
  char supplier_name[24];
  strcpy(supplier_name, "Supplier ");
  sprintf(&supplier_name[9], "%d", supplier->sup_num);
  

  while (!is_shop_closed) {
    int i = random() % SHOPS_COUNT; // shop serial number

    
    printf("%s: I am ready to supplying!\n", supplier_name);
  
    retval = pthread_mutex_lock(&shops[i].coronavirus_mutex);
    if (0 != retval) {
      perror("mutex_lock");
      exit(EXIT_FAILURE);
    }

    printf("%s: Entered in Shop number %d\n", supplier_name, shops[i].shop_num);
    printf("%s: Shop %d has %d products\n",
           supplier_name, shops[i].shop_num, shops[i].shop_products_count);

    shops[i].shop_products_count += supplier->sup_performance;

    printf("%s: Delivered %d products to Shop %d\n",
           supplier_name, supplier->sup_performance, shops[i].shop_products_count);
    printf("%s: Now Shop %d has %d products\n",
           supplier_name, shops[i].shop_num, shops[i].shop_products_count);

    printf("\n");

    retval = pthread_mutex_unlock(&shops[i].coronavirus_mutex);
    if (0 != retval) {
      perror("mutex_unlock");
      exit(EXIT_FAILURE);
    }

    printf("%s: I need to sleep before another delivery\n\n", supplier_name);
    sleep(SUPPLIER_SLEEP_TIME);
  }

  printf("%s: Ok, Boss, understood. Turn off my delivery script...\n", supplier_name);
  return NULL;
}
