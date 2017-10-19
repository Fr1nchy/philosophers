#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>

typedef enum{think, eat, wait} T_state;

typedef struct{
  pthread_mutex_t mutex;
}spoon;

typedef struct {
  spoon * right_hand;
  spoon * left_hand;
  T_state state;
  pthread_t tid;
  int number_thread,max_food, nb_philosopher,nb_eaten;
  struct philosopher * tab_philo;
}philosopher;

// Global semaphore to prevent from all the philosophers taking one spoon
sem_t sema;
pthread_mutex_t mut_tab_philo;

int ate_more_than_others(philosopher philo){
  int i = 1, min = philo->tab_philo[0];
  pthread_mutex_lock(&mut_tab_philo);
  while(i < philo->nb_philosopher)
  {
  	if (philo->tid != philo->tab_philo[i])
  	{
  		if (philo->tab_philo[i].nb_eaten < min)
  		{
  			min = philo->tab_philo[i];
  		}
  	}
    	i++;
  }

  if(philo->nb_eaten == min){
    return 0;
  }else{
    return 1;
  }
  pthread_mutex_unlock(&mut_tab_philo);
}

void take_spoon(philosopher * philo){
    philo->state = wait;
    printf("Le philosophe %i veut manger, il est en attente des cuillères\n",philo->number_thread);
    if(ate_more_than_others(philo)==0){
        sem_wait(&sema);
        pthread_mutex_lock(&(philo->right_hand->mutex));
        pthread_mutex_lock(&(philo->left_hand->mutex));
    }
}

void eating(philosopher * philo){
    philo->state = eat;
    printf("Le philosophe %i mange\n",philo->number_thread);
    srand(philo->tid);
    sleep (rand() / RAND_MAX * 1000000.) ;
}
// function drop the mutex on spoons taken
void release_spoon(philosopher * philo){
    pthread_mutex_unlock(&(philo->left_hand->mutex));
    pthread_mutex_unlock(&(philo->right_hand->mutex));
    sem_post(&sema);
    printf("Le philosophe %i a fini de manger, il pose les cuillères\n",philo->number_thread);
    (philo->nb_eaten)++;
}
// function think for a philosopher
void thinking(philosopher * philo){
    philo->state = think;
    printf("Le philosophe %i pense\n",philo->number_thread);
    srand(philo->tid);
    sleep (rand() / RAND_MAX * 10000.) ;
}

// function which decribes action for a philosopher
void * action_philosopher(void * arg){
    int i= 0;
    philosopher * philo = (philosopher *) arg;
    while(i < philo->max_food){
        thinking(philo);
        take_spoon(philo);
        eating(philo);
        release_spoon(philo);
        i++;
    }
  return NULL;
}

// initialization of tab struct spoon
void init_spoons(spoon * tab_spoon,int nb_spoon){
  int i;
  for(i = 0;i<nb_spoon;i++){
    pthread_mutex_init(&mut_tab_philo,NULL);
  }

}
// initialization of tab struct philosopher
void init_philosophers(philosopher * tab_philosopher,spoon * tab_spoon,int nb_philosopher,int max_food){
  pthread_mutex_init(&mut_tab_philo,NULL);
  for(int i = 0;i<nb_philosopher;i++){
    tab_philosopher[i].right_hand = &(tab_spoon[i]);
    tab_philosopher[i].left_hand = &(tab_spoon[(i+1)%nb_philosopher]);
    tab_philosopher[i].state = think;
    tab_philosopher[i].number_thread = i;
    tab_philosopher[i].max_food = max_food;
    tab_philosopher[i].nb_eaten = 0;
    tab_philosopher[i].nb_philosopher = nb_philosopher;
    tab_philosopher[i].tab_philo = tab_philosopher;

    pthread_create (&(tab_philosopher[i].tid),NULL,action_philosopher,&tab_philosopher[i]);
  }
}

int main (int argc, char **argv){
//  pthread_mutex_t global_mutex;
  philosopher * tab_philosopher;
  spoon * tab_spoon;
  int nb_philosophers,max_food;

  if (argc != 3){
    fprintf(stderr, "usage : %s nb_philosophers nb_max_food \n", argv[0]) ;
    exit (-1) ;
  }
  nb_philosophers = atoi(argv[1]);
  max_food = atoi(argv[2]);
  sem_init(&sema,0,nb_philosophers-1);

  tab_philosopher = malloc(sizeof(philosopher)*nb_philosophers);
  tab_spoon = malloc(sizeof(spoon)*nb_philosophers);

  init_spoons(tab_spoon,nb_philosophers);
  init_philosophers(tab_philosopher,tab_spoon,nb_philosophers,max_food);

  for (int i = 0; i < nb_philosophers; i++){
    pthread_join (tab_philosopher[i].tid, NULL) ;
  }

  free(tab_philosopher);
  free(tab_spoon);
  return EXIT_SUCCESS;
}
