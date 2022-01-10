#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

struct thread_args {
    int *total;
    pthread_mutex_t *mutex;
};

int nb_aleatoire() {
    srand(time(NULL) + getpid());
    return (rand() % 200);
}

void *thread_num_aleatoire(void *args) {
    struct thread_args *a = args;
    pthread_mutex_lock(a->mutex);
    *(a->total) += nb_aleatoire();
    pthread_mutex_unlock(a->mutex);
}

int main() {
    int total = 0;

    // Création du mutex pour les threads
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    struct thread_args args = {
        .total = &total,
        .mutex = &mutex,
    };

    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1, NULL, thread_num_aleatoire, &args);
    pthread_create(&t2, NULL, thread_num_aleatoire, &args);

    printf("Total : %d\n", *(args.total));
}
