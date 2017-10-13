#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>


int main (int argc, char **argv)
{
    pthread_mutex_t global_mutex;

  if (argc != 2){
    fprintf(stderr, "usage : %s nb_philosophers\n", argv[0]) ;
    exit (-1) ;
  }

  init_vector(argv[1]);







  return EXIT_SUCCESS;
}


void take_fork(pthread_mutex_t m)
{
    pthread_mutex_lock(&m);
    while (left_fork || right_fork)
    {
        cond_wait(m, eaten_queue);
    }

    pthread_mutex_unlock(&m);
    cond_signal(hungry_queue);



}

void release_fork()
