// pipeline-graphic.c

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "gpu.h"

// Les données pour les producteurs / consommateurs de Triangles
#define NB_TRIANGLES 1000
struct Triangle triangles[NB_TRIANGLES];
int triangle_read_index = 0;
int triangle_write_index = 0;
pthread_mutex_t triangle_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t triangle_sem_prod;
sem_t triangle_sem_conso;

// Les données pour les producteurs / consommateurs de Fragments.
#define NB_FRAGMENTS 10000
struct Fragment fragments[NB_FRAGMENTS];
int fragment_read_index = 0;
int fragment_write_index = 0;
pthread_mutex_t fragment_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t fragment_sem_prod;
sem_t fragment_sem_conso;


// Les nombres de threads par défauts pour les deux dernièrs étages.
int nb_rasters = 4;
int nb_shaders = 4;

// Ajouter ici les données indispensables pour synchroniser les étages (barrières ou conditions) 
pthread_barrier_t raster_barrier;
pthread_barrier_t shader_barrier;
pthread_barrier_t global_barrier;

// Création des triangles
static void *triangle_stage(void *a)
{
  struct Triangle triangle;
  // création du créateur de triangles.
  void *triangle_creator = triangle_creator_init();
  // On boucle pour chaque triangle.
  while (triangle_creator_get_next_triangle(triangle_creator, &triangle))
  {
    // Production des triangles dans le tableau des triangles :
    sem_wait(&triangle_sem_prod);
    pthread_mutex_lock(&triangle_mutex);
    
    triangles[triangle_write_index] = triangle;
    triangle_write_index = (triangle_write_index + 1) % NB_TRIANGLES;
    
    pthread_mutex_unlock(&triangle_mutex);
    sem_post(&triangle_sem_conso);
    
#if 1
    if (triangle_is_sync(&triangle))
    {
      int i;
      // produire le bon nombre de "triangles" de synchronisation 
      // pour tous les threads "raster_stage"

      // Attention, on vient d'en produire déjà un !

      for (i=0; i<nb_rasters-1; i++)
      {
            sem_wait(&triangle_sem_prod);
	    pthread_mutex_lock(&triangle_mutex);
	    
	    triangles[triangle_write_index] = triangle;
	    triangle_write_index = (triangle_write_index + 1) % NB_TRIANGLES;
    
	    pthread_mutex_unlock(&triangle_mutex);
	    sem_post(&triangle_sem_conso);
      }
      // On attend que tous les rasters finissent
      pthread_barrier_wait(&raster_barrier);
      // produire le bon nombre de fragments de synchronisation pour tous les threads "shader_stage"
      for (i = 0; i < nb_shaders; i++)
      {
	sem_wait(&fragment_sem_prod);
	pthread_mutex_lock(&fragment_mutex);

	fragments[fragment_write_index] = fragment_sync();
	fragment_write_index = (fragment_write_index + 1) % NB_FRAGMENTS;

	pthread_mutex_unlock(&fragment_mutex);
	sem_post(&fragment_sem_conso);
      }
      // On attend que tous les shaders finissent
      pthread_barrier_wait(&shader_barrier);
      // On vide les pipeline
      triangle_read_index = triangle_write_index = 0;
      fragment_read_index = fragment_write_index = 0;
      // On relance tout le monde
      pthread_barrier_wait(&global_barrier);
    }
#endif 
  }
  // libération des données et écriture de l'image sur le disque.
  triangle_creator_destroy(triangle_creator);
  return a;
}

// thread de création des fragments à partir des triangles
void *raster_stage(void *a)
{
  while (1)
  {
    void *raster;
    struct Triangle triangle;
    struct Fragment fragment;

    // Consomation des triangles
    sem_wait(&triangle_sem_conso);
    pthread_mutex_lock(&triangle_mutex);

    triangle = triangles[triangle_read_index];
    triangle_read_index = (triangle_read_index + 1) % NB_TRIANGLES;

    pthread_mutex_unlock(&triangle_mutex);
    sem_post(&triangle_sem_prod);

#if 1
    if (triangle_is_sync(&triangle))
    {
      // On attend que tout le monde finisse
      pthread_barrier_wait(&raster_barrier);
      // On attend de se faire relancer
      pthread_barrier_wait(&global_barrier);
    } else
#endif
    {
      raster = raster_init(&triangle);
      while (raster_get_next_fragment(raster, &fragment))
      {
	sem_wait(&fragment_sem_prod);
	pthread_mutex_lock(&fragment_mutex);

	fragments[fragment_write_index] = fragment;
	fragment_write_index = (fragment_write_index + 1) % NB_FRAGMENTS;

	pthread_mutex_unlock(&fragment_mutex);
	sem_post(&fragment_sem_conso);
	
      }
    }
  }
  return a;
}

// thread de colorisation de l'image.
void *shader_stage(void *a)
{
  while (1)
  {
    struct Fragment fragment;
    // consomation des fragments.
    sem_wait(&fragment_sem_conso);
    pthread_mutex_lock(&fragment_mutex);

    fragment = fragments[fragment_read_index];
    fragment_read_index = (fragment_read_index + 1) % NB_FRAGMENTS;

    pthread_mutex_unlock(&fragment_mutex);
    sem_post(&fragment_sem_prod);
    

#if 1
    if (fragment_is_sync(&fragment))
    {
      // On attend que tout le monde finisse
      pthread_barrier_wait(&shader_barrier);
      // On attend de se faire relancer
      pthread_barrier_wait(&global_barrier);
    } else
#endif
      gpu_shader(fragment);
  }

  return a;
}

// Fonction principale
int main(int argc, char *argv[])
{
  int i;
  // Lecture des arguments pour changer le nombre de threads sur la ligne de commande.
  if (argc > 1)
  {
    nb_rasters = atoi(argv[1]);
    if (argc > 2)
      nb_shaders = atoi(argv[2]);
  }
  // Protection contre des valeurs aberrantes.
  if (nb_rasters < 1 ) nb_rasters = 1;
  if (nb_rasters > 50) nb_rasters = 50;
  if (nb_shaders < 1 ) nb_shaders = 1;
  if (nb_shaders > 100)nb_shaders = 100;

  pthread_t t_tri;                   // thread stage 1
  pthread_t t_rasters[nb_rasters];   // threads stage 2
  pthread_t t_shaders[nb_shaders];   // threads stage 3

  // Création des barrière ou conditions utiles pour les synchros.

  pthread_barrier_init(&raster_barrier,NULL,nb_rasters+1);
  pthread_barrier_init(&shader_barrier,NULL,nb_shaders+1);
  pthread_barrier_init(&global_barrier,NULL,nb_rasters+nb_shaders+1);

  // Création des sémaphore pour les producteurs / consommateurs de triangles
  sem_init(&triangle_sem_conso, 0, 0);
  sem_init(&triangle_sem_prod, 0, NB_TRIANGLES);

  // Création des sémaphore pour les producteurs / consommateurs de fragments
  sem_init(&fragment_sem_conso, 0, 0);
  sem_init(&fragment_sem_prod, 0, NB_FRAGMENTS);

  // Création de tous les threads.
  pthread_create(&t_tri, NULL, triangle_stage, NULL);

  for (i = 0; i < nb_rasters; i++)
    pthread_create(&t_rasters[i], NULL, raster_stage, NULL);

  for (i = 0; i < nb_shaders; i++)
    pthread_create(&t_shaders[i], NULL, shader_stage, NULL);

  // On n'attend que la fin du premier étages, les autres seront synchronisés.
  pthread_join(t_tri, NULL);

  return 0;
}
