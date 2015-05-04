#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


int x1 = 0;
int x2 = 0;
pthread_mutex_t lock_x1 =  PTHREAD_MUTEX_INITIALIZER;

void *th1 ()
{
  x1 = rand();
  pthread_mutex_unlock(&lock_x1);
  return NULL;
}

void *th2 (void* arg)
{
  if(arg != NULL)
    pthread_exit(0);
  x2 = rand();
  
  pthread_mutex_lock(&lock_x1);
  int* y = malloc(sizeof(int));
  *y = x1 +x2;

  pthread_exit(y);

}


int main()
{
  int valeur;
  pthread_t thread1, thread2;
  pthread_mutex_lock(&lock_x1);


  pthread_create(&thread1,NULL,th1,NULL);
  pthread_create(&thread2,NULL,th2,NULL);


  pthread_join(thread2, (void**)(&valeur));

  printf("Le nombre est %d \n",valeur);

  return 0;
}
