#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
  int i =0,retour_fork;
  for(i=0;i<3;i++)
    {
      retour_fork = fork();

      if(retour_fork == -1)
	{
	  printf("Erreur lancement fork() \n");
	  exit(-1);
	}
      printf("i=%d je suis le processus [%d], mon pere est: [%d], code retour fork:[%d]\n",i,getpid(),getppid(),retour_fork);
    }
  while(wait(NULL)!=-1);//attend que tous ses fils terminent
  return 0;
  
}
