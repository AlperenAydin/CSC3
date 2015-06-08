#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
  int N= 5;
  if(argc>1)
    {
      sscanf(argv[1],"%d",&N);
    }
  
  
  int i =0,retour_fork;
  for(i=0;i<N;i++)
    {
      retour_fork = fork();

      if(retour_fork == -1)
	{
	  printf("Erreur lancement fork() \n");
	  exit(-1);
	}
      if(retour_fork == 0)
	{
	  printf("i=%d je suis le processus [%d], mon pere est: [%d], code retour fork:[%d]\n",i,getpid(),getppid(),retour_fork);
	  sleep(2*i);
	  exit(i);
	}
    }
  while(wait(NULL)!=-1);//attend que tous ses fils terminent
  return 0;
  
}
