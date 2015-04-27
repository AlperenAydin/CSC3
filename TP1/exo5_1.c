#include <stdio.h>
#include <pthread.h>

void* afficherMessage(void *ptr)
{
  char* message = (char*) ptr;

  int i= 0;
  while (i < 1)
    {
      printf("%s ",message);
      i++;
    }

  pthread_exit(0);
}

int main(int argc,char* argv[])
{
  pthread_t *threads = malloc((argc-1)*sizeof(pthread_t));

  int i=1;
  while(i<argc)
    {

      pthread_create(threads+i-1,NULL,afficherMessage,*(argv+i));
      i++;
   }
  


}
