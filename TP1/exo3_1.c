#include <stdio.h>

int ajouter(int x, int y)
{
  return x+y;
}


int main()
{
  int x,y;
  int (*ajout)(int,int); 
  scanf("%d",&x);
  printf("\n");
  scanf("%d",&y);
  printf("\n");

  ajout = ajouter;
  printf("\n %d + %d = %d. \n",x,y,(*(ajout))(x,y));
  return 0;
  
}
