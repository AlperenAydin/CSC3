#include <stdio.h>
#include <stdlib.h>

struct data 
{
  char const *texte;
  int n;
};

typedef struct data Data;

void* task(void *p_data)
{
  Data* d = (Data *) p_data;

  int i = 0;
  
  while(i < d->n)
    {
      printf("%s ",d->texte);
      i++;      
    }

  return 0;
}


int main()
{
  Data* d = malloc(sizeof(Data));
  int n;
  scanf("%d",&n);
  d->n = n;
  char t[10];
  scanf("%s",t);
  d-> texte = t;
  task(d);

  printf("\n");
  free(d);
  return 0;
}
