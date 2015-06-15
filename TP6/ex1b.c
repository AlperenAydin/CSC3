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
 wait(NULL);
 if(fork() == 0)
   {
     execlp("ps","ps",NULL);
     printf("Failure at SECOND \n");
   }
 wait(NULL);
 if(fork() == 0)
   {
     execlp("ls","ls","-l",NULL);
     printf("Failure at THIRD \n");
   }
 wait(NULL);

 return 0;
}
