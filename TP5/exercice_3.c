#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>


void sommeImpairs()
{
  int s=0;
  int n=0;
  for(n=0;n<10000;n++)
    {
      s = s+ (2*n+1);
    }

  printf("La somme de 10 000 premiere nombres impairs est %d \n",s);
}


int main()
{
  if(fork() == 0)
    {
      execlp("grep","grep","fork","exercice_3.c",NULL);
      printf("exec fail");
    }
  else
    {
      if(fork()==0)
	{
	  sommeImpairs();
	}
    }
  wait(NULL);
  wait(NULL);
  printf("Je suis fini \n");

  return 0;


}
