#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int S = 0;
pthread_mutex_t lockS = PTHREAD_MUTEX_INITIALIZER;

void *helloWorld(void *arg)
{
  int a = *((int *) arg);
  printf("Hello World, I am number %d \n",a);
  
  int i;
  for(i=0;i<1000000;i++)
    {
      pthread_mutex_lock(&lockS);
      S = S+a;
      pthread_mutex_unlock(&lockS);
    }
  return 0;
}


int main(int argc,char* argv[])
{
  if(argc != 2)
    {
      printf("Mauvais format");
      return 0;
    }

  int N=0;
  sscanf(argv[1],"%d",&N);

  pthread_t *thread = malloc(N*sizeof(pthread_t));

  int i = 0;
  for(i=0;i<N;i++)
    {
      int *p = malloc(sizeof(int));
      *p = i;
      pthread_create(thread+i,NULL,helloWorld,p);
    }

  for(i=0;i<N;i++)
    {
      pthread_join(*(thread+i),NULL);
    }
  printf("La somme est %d \n",S);
  
  return 0;


}
