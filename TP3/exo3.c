#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>

#define NB_THREAD 2

int curseur = 1; // A quel fichier on en est
int fichiersRestant;

pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER; // Initialisation du mutex

void *passerMajuscule (void *tab)
{
	FILE *fp1, *fp2;
	int c = 0;

	char** tab_fichier = (char**)tab;

	while(1)
	{
		pthread_mutex_lock(&verrou);
		if (fichiersRestant == 0) { 
			pthread_mutex_unlock(&verrou); 
			pthread_exit(NULL); 
		}
		
		fp1 = fopen(tab_fichier[curseur], "r");
		fp2 = fopen(tab_fichier[curseur], "r+");
		curseur++; // On prend le fichier courant
		fichiersRestant--;
		pthread_mutex_unlock(&verrou);
		
		if((fp1 == NULL) || (fp2 == NULL))
		{
			perror("fopen");
			exit(1);
		}

		c='r';
		while(c != EOF)
		{
			c = fgetc(fp1);
			//if(c != EOF) fputc(tolower(c), fp2);
			if(c != EOF) fputc(toupper(c), fp2);
		}
		fclose(fp1);
		fclose(fp2);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	int NbFichiers = 0;

	if(argc == 1) // On vérifie si il y a bien un argument
    {
      printf("Aucun argument\n");
      return 0;
    }
    else // Si oui
    {
    	NbFichiers = argc-1;
    	fichiersRestant = NbFichiers;

  		pthread_t t_fichier[NB_THREAD]; // On crée autant de thread que de fichiers passés en paramètre

  		for(int i = 0 ; i < NB_THREAD ; i ++) // On crée ces threads
	    {	
	      	if(pthread_create(&t_fichier[i], NULL, passerMajuscule, (void *)argv) != 0)// On crée les threads
	      	{
		  	    perror("Thread"); // Si il y a eu un problème
		  	    exit(-1);
	      	}
    	}

    	for(int i = 0 ; i < NB_THREAD ; i ++)
  		{
  		  	pthread_join(t_fichier[i], NULL); // On attend la fin de l'execution de tous les threads
  		}
    }

	return 0;
}