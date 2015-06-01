#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>


#define M 100

typedef struct tabCritique{
  int nombres[M];
  int somme;
  pthread_mutex_t lock;
  sem_t sema;
  char c;
}tabCritique;

void tabInit(tabCritique* t)
{
  pthread_mutex_init(&(t->lock), NULL);
  sem_init(&(t->sema),0,0);
  t->somme = 0;
}

void* filtre(void* arg)
{
  tabCritique *t = (tabCritique*) arg;
  sem_t *sema = &(t->sema);
  pthread_mutex_t *lock = &(t->lock);

  int n;
  int i=0;
  int s;
  while(i<=M)
    {
      sem_wait(sema);
      
      pthread_mutex_lock(lock);
      n = (t->nombres)[i];
      pthread_mutex_unlock(lock);
      sem_getvalue(sema,&s);
      printf("filtre %c :%d  n = %d sem = %d\n",t->c,i,n,s);
      if( n == -1)
	pthread_exit(0);

      t->somme = t->somme + n;
      i++;

    }

  pthread_exit(0);
}

int main()
{
  int N = 10;
  
  tabCritique tPairs, tImpairs;
  tabInit(&tPairs); tabInit(&tImpairs);
  tPairs.c = 'p'; tImpairs.c = 'i';
  
  pthread_t thrPairs, thrImpairs;
  //pthread_create(&thrPairs,NULL, filtre,&tPairs);
  //pthread_create(&thrImpairs,NULL, filtre,&tImpairs);

  int p=0, i=0; 
  int k=0;
  int x;
  while(k<N)
    {
      x =rand()%100;
      if(x%2 ==0) 
	{
	  pthread_mutex_lock(&(tPairs.lock));
	  printf("Papa: x = %d \n",x);
	  tPairs.nombres[p] = x;
	  sem_post(&(tPairs.sema));
	  pthread_mutex_unlock(&(tPairs.lock));
	  p++;
	}
      else
	{
	  pthread_mutex_lock(&(tImpairs.lock));
	  printf("Papa: x = %d \n",x);
	  tImpairs.nombres[p] = x;
	  sem_post(&(tImpairs.sema));
	  pthread_mutex_unlock(&(tImpairs.lock));
	  i++;
	}
      k++;

    }
  pthread_mutex_lock(&(tPairs.lock));
  tPairs.nombres[p+1] = -1;
  sem_post(&(tPairs.sema));
  pthread_mutex_unlock(&(tPairs.lock));

  pthread_mutex_lock(&(tImpairs.lock));
  tImpairs.nombres[i+1]= -1;
  sem_post(&(tImpairs.sema));
  pthread_mutex_unlock(&(tImpairs.lock));

  
  //pthread_join(thrPairs,NULL);
  //pthread_join(thrImpairs,NULL);

  int sommePairs= tPairs.somme;
  int sommeImpairs = tImpairs.somme;
  printf("Somme de nombres pairs = %d \n", sommePairs);
  printf("Somme de nombres impairs = %d \n", sommeImpairs);
  printf("Somme de nombres = %d \n", sommePairs+sommeImpairs);


  return 0;
}

