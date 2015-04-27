#include <stdio.h>
#include <pthread.h>

void *afficherMessage(void *ptr)
{
  char* message  =(char*) ptr;

  int i = 0;
  while( i < (15))
    {
      printf("%d %s \n",i,message);
      i++;
    }
  pthread_exit(0);
    
}


int main()
{
  pthread_t thread1, thread2;
  char *message1 = "Thread 1!";
  char *message2 = "Thread 2!";
  int iret1, iret2;

  pthread_create(&thread1, NULL, afficherMessage, message1);
  pthread_create(&thread2, NULL, afficherMessage, message2);

  void *r1 = &iret1,*r2 =&iret2;
  pthread_join(thread1, &r1);
  pthread_join(thread2, &r2);
  
  printf("Thread 1 renvoie: %d \n",iret1);
  printf("Thread 2 renvoie: %d \n",iret2);

  
  pthread_exit(0);

}
