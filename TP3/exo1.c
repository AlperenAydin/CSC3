#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>


#define N 16

typedef struct prodcons{
  int buffer[N];
  int posLec, posEcr;
  sem_t semLec;
  sem_t semEcr;
}Prodcons;

pthread_mutex_t lockPC = PTHREAD_MUTEX_INITIALIZER;

void prodcons_init(Prodcons* control);
  
void* prod(void* arg);

void* cons(void* arg);

int main(int argc, char* argv[])
{
  if(argc != 3)
    return -1;

  int i =0;
  int nConso; // Le nomber de Conso
  sscanf(argv[1],"%d",&nConso);
  printf("Nombre de Prod = %d \n",nConso);

  int nProd; // Le nomber de Prod
  sscanf(argv[2],"%d",&nProd);
  printf("Nombre de Cons = %d \n",nProd);

  Prodcons control;
  prodcons_init(&control);

  pthread_t* Tprods = malloc(nProd*sizeof(pthread_t));
  for(i=0;i<nProd;i++)
    {
      pthread_create(Tprods+i, NULL, prod, &control);
    }
  pthread_t* Tconsos = malloc(nConso*sizeof(pthread_t));
  for(i=0;i<nConso;i++)
    {
      pthread_create(Tconsos+i, NULL, cons, &control);
    }
    
  for(i=0;i<nProd;i++)
    {
      pthread_join(*(Tprods+i), NULL);
    }
  for(i=0;i<nConso;i++)
    {
      pthread_join(*(Tconsos+i), NULL);
    }
  free(Tprods);
  free(Tconsos);
  
  return 0;
}

void prodcons_init(Prodcons* control)
{
  control-> posLec = 0;
  control-> posEcr = 0;

  sem_init(&(control->semLec),0,0);
  sem_init(&(control->semEcr),0,N);

}

// Ecriture aux buffer
void* prod(void* arg)
{
  Prodcons* control = (Prodcons*) arg;
  while(1)
    {
      int a= rand();
      
      sem_wait(&(control->semEcr));
      pthread_mutex_lock(&lockPC); // Critical area
      int x = control -> posEcr;

      (control->buffer)[x] = a;

      control-> posEcr = (x+1)%N;
      sem_post(&(control->semLec));

      pthread_mutex_unlock(&lockPC); // Critical area
    }
  pthread_exit(0);
} 

// Lecture aux buffers
void* cons(void *arg)
{
  Prodcons* control = (Prodcons*) arg;

  while(1)
    {
      sem_wait(&(control->semLec));
      pthread_mutex_lock(&lockPC); // Critical area  
  
      int x = control -> posLec;
      int a = (control->buffer)[x];
      control-> posLec = (x+1)%N;
      sem_post(&(control->semEcr));

      pthread_mutex_unlock(&lockPC); // Critical area


      printf("Conso %d: %d => %lf \n", x, a, sqrt(a));
    }

  pthread_exit(0);

}
