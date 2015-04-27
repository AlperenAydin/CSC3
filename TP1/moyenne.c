#include <stdio.h>
#include <stdlib.h>

int notes(int ln, char* l[],double n[])
{
  int isNum = 1; 
  int i;
  for(i=0; i<ln; i++)
    {
      isNum = sscanf(l[i],"%lf", n+i);
      if(isNum == 0 || *(n+i) > 20 || *(n+i)<0)
	return 0;
    }

  return isNum;
  
}

double moyenne(int ln, double n[])
{
  int i = 0;
  int sum = 0;
  for(i=0; i<ln; i++)
    {
      sum += n[i];
    }
  return sum/ln;
}


int main(int argc, char* argv[])
{
  if(argc == 1)
    {
      printf("Aucune moyenne à calculer \n");
      return -1;
    }
  double* n = malloc((argc-1)*sizeof(double));
  int valide = notes(argc-1, argv+1, n);

  if(valide == 0)
    {
      printf("Note non valide \n");
      return -2;
    }
  printf("Moyenne est: %.2lf \n", moyenne(argc-1, n));

  mempty(n);

  return 0;
  
}
