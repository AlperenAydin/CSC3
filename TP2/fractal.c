#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

const int size = 1000;

unsigned char *image;

// yc va etre accede par plusieurs threads
// et sert a la communication entre eux.
// lock_y0 sert a gere l'access
int yc = 0;
pthread_mutex_t lock_yc = PTHREAD_MUTEX_INITIALIZER; 


static void calcul(int x, int y, unsigned char *pixel)
{
  long double p_x = (long double)x/size * 2.0l - 1.0l;
  long double p_y = (long double)y/size * 2.0l - 1.0l;
  long double tz = 0.7;
  long double zoo = powl(0.5l, 13.0l * tz);
  long double cc_x = -0.05l + p_x * zoo;
  long double cc_y = 0.6805l + p_y * zoo;
  long double z_x = 0.0l;
  long double z_y = 0.0l;
  long double m2 = 0.0l;
  long double co = 0.0l;
  long double dz_x = 0.0l;
  long double dz_y = 0.0l;
  int i;
  for (i = 0; i < 2560; i++)
  {
    long double old_dz_x, old_z_x;
    if (m2 > 1024.0l)
      continue;
    // Z' -> 2�Z�Z' + 1
    old_dz_x = dz_x;
    dz_x = 2.0l * z_x * dz_x - z_y * dz_y + 1.0l;
    dz_y = 2.0l * z_x * dz_y + z_y * old_dz_x;
    // Z -> Z� + c
    old_z_x = z_x;
    z_x = cc_x + z_x * z_x - z_y * z_y;
    z_y = cc_y + 2.0l * old_z_x * z_y;
    m2 = z_x * z_x + z_y * z_y;
    co += 1.0l;
  }
  // distance	
  // d(c) = |Z|�log|Z|/|Z'|
  long double d = 0.0l;
  if (co < 2560.0l)
  {
    long double dot_z = z_x * z_x + z_y * z_y;
    d = sqrtl(dot_z/(dz_x*dz_x+dz_y*dz_y)) * logl(dot_z);
    pixel[0] = fmodl(co,256.0l);
    d = 4.0l * d / zoo;
    if (d < 0.0l) d = 0.0l;
    if (d > 1.0l) d = 1.0l;
    pixel[1] = fmodl((1.0l-d) * 255.0l * 300.0l, 255.0l);
    d = powl(d, 50.0l*0.25l);
    pixel[2] = d * 255.0l;
  }
  else
    pixel[0]=pixel[1]=pixel[2]=0;
}

void* parallel()
{
  int x,y;
  while(1)
    {
      pthread_mutex_lock(&lock_yc);
      if(yc>=size)
	{
	  pthread_mutex_unlock(&lock_yc);
	  pthread_exit(0);
	}
      y = yc;
      yc=yc+1;
      pthread_mutex_unlock(&lock_yc);
      
      for (x = 0; x < size; x++)
	calcul(x, y, image+3*(y * size + x));
    }
}

int main(int argc, char const *argv[])
{
  if(argc < 2)
    return 0;
    
  FILE *file;
  image = malloc(3*size*size);

  int N=1; 
  sscanf(argv[1],"%d",&N);
  pthread_t *thread = malloc(N*sizeof(pthread_t));

  int n = 0;
  while(n<N)
    {
      pthread_create(thread+n,NULL,parallel,NULL);
      n++;
    }

  
  n = 0;
  while(n<N)
    {
      pthread_join(*(thread+n),NULL);

      n++;
    }
  


  // �criture fichier
  file = fopen("image.ppm", "w");
  fprintf(file, "P6\n%d %d\n255\n", size, size);
  fwrite(image, size, size * 3, file);
  fclose(file);
  free(image);

  return 0;
}
