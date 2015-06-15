#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{

  int i, delai;

  for (i=0; i<4; i++)
    {
      if (fork()) break;
    }
  srand(getpid());
  delai = rand()%4;
  sleep(delai);
  wait(NULL);
  printf("Mon nom est %c, j’ai dormi %d secondes \n",'A'+i,delai);
  exit(EXIT_SUCCESS);
}
