#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>



int main(int argc, char* argv[])
{
  if(argc < 2)
    {
      printf("Pas de fichiers \n");
      return -1;
    }
  int i;
  pid_t pids[argc-1];
  // Compilation des fichiers
  for(i=1; i<argc; i++)
    {
      pids[i-1] = fork();
      if(pids[i-1] == 0) // Si on est dans les fils
	{
	  execlp("gcc","gcc","-c",argv[i],"-Wall","-Wextra",NULL);
	}
    }

  // On va attendre tout soit compile 
  // On affiche les comilation echoue
  int status;
  pid_t pid;
  int compilation_success = 1;
  for(i=1;i<argc; i++)
    {
      pid = wait(&status);
      int ok = WEXITSTATUS(status);
      if(ok != 0 )
	{
	  compilation_success =0; // Compilation failed
	  int j;
	  for(j=1;j<argc;j++) // On cherche la source de l'erreur
	    {
	      // L'indice d'un fils dans pids 
	      // est l'indice dans argv du fichier
	      if(pids[j-1] == pid) 
		{
		  printf("Erreur de syntaxe fichier %s \n",argv[j]);
		}
	    }	  
	}
    }

  // Complilations finies

  // Editions de liens
  if(compilation_success == 0)
    {
      return 0; // Compilation non reussi donc on sort
    }

  // Creation de la commande edition
  int l = argc-1+4;
  char** obj = malloc(l * sizeof(char*));
  // obj = gcc *.o *.o ... -o executable NULL
  obj[0] = "gcc";
  obj[l-3] = "-o";
  obj[l-2] = "executable";
  obj[l-1] = NULL;

  for(i=1;i<argc;i++)
    {
      int n = 1+strlen(argv[i]);
      obj[i] = malloc(n*sizeof(char));
      strcpy(obj[i],argv[i]);
      obj[i][n-2] = 'o'; // On remplace *.c par *.o
    }
  if(fork() == 0)
    {
      execvp("gcc",obj);
      printf("Erreur dans execvp de edition \n");
    }
  
 
  pid = wait(&status);
  int ok = WEXITSTATUS(status);
  if(ok != 0 )
    {
      printf("Erreur Edition de Liens \n");
    }
  for(i=1;i<argc;i++)
    {
      free(obj[i]);
    }
  free(obj);
  return 0;

}
