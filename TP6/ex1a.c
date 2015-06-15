#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
if(fork() == 0)
  {
    execlp("who","who",NULL);
    printf("Failure at FIRST \n");
  }	
 if(fork() == 0)
   {
     execlp("ps","ps",NULL);
     printf("Failure at SECOND \n");
   }
 if(fork() == 0)
   {
     execlp("ls","ls","-l",NULL);
     printf("Failure at THIRD \n");
   }

 while(wait(NULL)!=-1);

 return 0;
}
