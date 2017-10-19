#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

typedef enum{think, eat, wait} T_state;

typedef struct{
  struct philosopher * eater;
  pthread_mutex_t mutex;
}spoon;

typedef struct {
  spoon * right_hand;
  spoon * left_hand;
  T_state state;
  pthread_t tid;

}philosopher;


void take_spoon(philosopher * philo){

}

void eating(philosopher * philo){
  printf("Le philosophe %lu mange",philo->tid);
}

void release_spoon(philosopher * philo){

}

void thinking(philosopher * philo){
  printf("Le philosophe %lu pense",philo->tid);
}

void * action_philosopher(void * arg){
  philosopher * philo = (philosopher *) arg;
  while(1){
    thinking(philo);
    take_spoon(philo);
    eating(philo);
    release_spoon(philo);
  }
  return NULL;
}

void init_spoons(spoon * tab_spoon,int nb_spoon){
  int i;
  for(i = 0;i<nb_spoon;i++){
    spoon new_spoon;
    new_spoon.eater = NULL;
    pthread_mutex_init(&(new_spoon.mutex),NULL);
    tab_spoon[i] = new_spoon;
  }

}

void init_philosophers(philosopher * tab_philosopher,spoon * tab_spoon,int nb_philosopher){
  int i;
  for(i = 0;i<nb_philosopher;i++){
    philosopher new_philosopher;
    new_philosopher.right_hand = &(tab_spoon[i]);
    new_philosopher.left_hand = &(tab_spoon[(i+1)%nb_philosopher]);
    new_philosopher.state = think;
    tab_philosopher[i] = new_philosopher;
    pthread_create (&(new_philosopher.tid),NULL,action_philosopher,&new_philosopher);

  }
}

int main (int argc, char **argv){
//  pthread_mutex_t global_mutex;
  philosopher * tab_philosopher;
  spoon * tab_spoon;
  int nb_philosophers;

  if (argc != 2){
    fprintf(stderr, "usage : %s nb_philosophers\n", argv[0]) ;
    exit (-1) ;
  }
  nb_philosophers = atoi(argv[1]);
  tab_philosopher = malloc(sizeof(philosopher)*nb_philosophers);
  tab_spoon = malloc(sizeof(spoon)*nb_philosophers);
  init_spoons(tab_spoon,nb_philosophers);
  init_philosophers(tab_philosopher,tab_spoon,nb_philosophers);

  for (int i = 0; i < nb_philosophers; i++){
    pthread_join (tab_philosopher[i].tid, NULL) ;
  }

  return EXIT_SUCCESS;
}

/*
void take_fork(pthread_mutex_t m){

    pthread_mutex_lock(&m);
    while (left_fork || right_fork)
    {
        cond_wait(m, eaten_queue);
    }

    pthread_mutex_unlock(&m);
    cond_signal(hungry_queue);

}

void release_fork()
*/
