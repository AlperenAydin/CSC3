#include <stdio.h> 
#include <stdlib.h>
#include <string.h>


char* renverserString(char* str)
{
  // On determine la longeur ln de str
  int ln = strlen(str);
  char* rv = malloc(ln*sizeof(char));
  int i = 0; 
  while(i<ln)
    {
      rv[i] = str[ln-i-1];
      i++; 
    }
  *(rv+ln) = '\0';

  return rv;
}


int main (int argc, char* argv[])
{

  int i = argc-1;
  while(i>0)
  {
    printf("%s ", renverserString(argv[i]) );
    i--;
  }
  printf("\n");

  return 0;

}
