#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  printf("Lancement de fork \n");
  int valeur=fork();
  if(valeur==-1)
    {
      printf("Erreur lancement de fork \n");
      exit(-1);
    }
  if(valeur==0) {
    //le fils dort 5 secondes
    sleep(5);
  }
  if(valeur>0)
    {
      sleep(10);
      printf("Je suis le père et je termine avant mon fils \n");
    }
  else
    printf("Je suis le fils et pas de père depuis 5 secondes \n");
  return 0;
}
