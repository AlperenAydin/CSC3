#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

int main(int argc, char* argv[]) 
{
  int N;
  if(argc <2)
    N = 3;
  else 
    {
      sscanf(argv[1],"%d",&N);
    }
  int i; 
  for(i=0;i<N;i++)
    {
      fork(); fork();
    }
  printf("Bonjour \n");
  while(wait(NULL) != -1);
  return 0;
}
