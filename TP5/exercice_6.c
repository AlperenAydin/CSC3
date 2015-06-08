#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
int i=0;
int pid_pere=getpid();
 printf("digraph G{\n");
 fflush(stdout);
 for(i=0;i<5;i++) {
   int retour_fork=fork();
   if(retour_fork==-1)
     {
       printf("Erreur lancement fork\n");
       exit(-1);
     }
   wait(NULL);
   if(
      retour_fork==0)
     {
       printf("%d->%d \n",getppid(),getpid());
       fflush(stdout);
     }
 }
 while(wait(NULL)!=-1);
 if(getpid()==pid_pere)
   printf("}\n");
 return 0;
}
