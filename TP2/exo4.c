#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void rdv1()
{
  printf("\033[31m Je suis rdv1 \n");
}

void rdv2()
{
  printf("\033[32m Je suis rdv2 \n");
}

void* th1()
{
  int i;
  for(i=0;i<50;i++)
    {
      printf("\033[31m  %d eme fois \n",i);
    }

  pthread_mutex_unlock(&lock2);
  pthread_mutex_lock(&lock1);
  rdv1();


  pthread_exit(0);

}


void* th2()
{
  int i;
  for(i=0;i<100;i++)
    {
      printf("\033[32m  %d eme fois \n",i);
    }

  pthread_mutex_unlock(&lock1);
  pthread_mutex_lock(&lock2);
  rdv2();


  pthread_exit(0);

}

int main()
{
  pthread_t thread1, thread2;

  pthread_mutex_lock(&lock1);
  pthread_mutex_lock(&lock2);

  pthread_create(&thread1,NULL,th1,NULL);
  pthread_create(&thread2,NULL,th2,NULL);


  pthread_join(thread1,NULL);
  pthread_join(thread2,NULL);
  printf("\033[0m");


  return 0;
}
