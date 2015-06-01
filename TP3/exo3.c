#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>

#define NB_FILES
char *filenames[NB_FILES];
int read_index = 0;
int max_index; 
pthread_mutex_t acces = PTHREAD_MUTEX_INITIALIZER;


void* main(void) {

  while(1)
    {
      char filename[10]; 
      pthread_mutex_lock(acces);

      if(read_index >= max_index) 
	pthread_exit(0);
      index = read_index;
      strcpy(arg,filenames[read_index]);
      read_index ++;

      pthread_mutex_unlock(acces);
      FILE* fp1, *fp2;
      int c;
      fp1= fopen (filename , "r");
      fp2= fopen (filename , "r+");
      if ((fp1== NULL) || (fp2== NULL)) {
	perror ("fopen");
	exit (1);
      }
      while (c != EOF) {
	c = fgetc(fp1);
	if (c != EOF) fputc(toupper(c),fp2);
      }
      fclose (fp1);
      fclose (fp2);
    }
}
