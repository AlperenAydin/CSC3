#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  int a=10;
  printf("Lancement de fork \n");
  // La creation du fils
  int valeur = fork();
  if(valeur == -1)
    {
      printf("Erreur lancement de fork \n");
      exit(-1);
    }
  if(valeur == 0) // Si on est dans le fils
    {
    a=a+2;
    printf("Je suis le fils, a=%d \n",a);
    }
  else // Sinon, on est dans le pere
    {
      a=a-2;
      printf("Je suis le pere, a=%d \n",a);
    }
  printf("Qui suis-je, sachant que a=%d ? \n",a);
  return 0;
}
