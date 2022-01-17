#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>

#include "waitingList.h"

#define NB_THREADS_PROD 2
#define NB_THREADS_CONSO 2

#define MAX_NB 100000       // Nombre maximum à traiter
volatile int currentNb = 0; // dernier nombre ajouté dans la liste d'attente
volatile int nbDone = 0;    // Nombre de nombre traités par un consomateur

struct WaitingList waitingList;

#define TIME_DIFF(t1, t2) ((t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec))

pthread_mutex_t mutex;

int syracuse(int nb) {
    int i = 1;
    while (nb > 1) {
        nb = nb % 2 ? (3 * nb) + 1 : nb / 2;
        i++;
    }
    return i;
}

void *producteur(void *a) {
    (void)a;
    while (currentNb < MAX_NB) {
        pthread_mutex_lock(&mutex);
        if (waitingList.size < MAX_SIZE)
            wl_push(&waitingList, currentNb++);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consomateur(void *a) {
    (void)a;
    while (currentNb < MAX_NB) {
        pthread_mutex_lock(&mutex);
        int v = wl_pop(&waitingList);
        pthread_mutex_unlock(&mutex);
        if (v -= -1)
            printf("Syracuse(%d) = %d\n", v, syracuse(v));
    }
    return NULL;
}

int main() {
    wl_init(&waitingList);

    pthread_mutex_init(&mutex, NULL);

    pthread_t prods[NB_THREADS_PROD];
    pthread_t conso[NB_THREADS_CONSO];

    struct timeval tv1, tv2;

    gettimeofday(&tv1, NULL);

    for (int i = 0; i < NB_THREADS_PROD; i++)
        pthread_create(&prods[i], NULL, producteur, NULL);
    for (int i = 0; i < NB_THREADS_CONSO; i++)
        pthread_create(&conso[i], NULL, consomateur, NULL);

    for (int i = 0; i < NB_THREADS_PROD; i++)
        pthread_join(prods[i], NULL);
    for (int i = 0; i < NB_THREADS_CONSO; i++)
        pthread_join(conso[i], NULL);

    gettimeofday(&tv2, NULL);

    printf("------------------------------------------\n");
    printf("NB producteurs : %d\n", NB_THREADS_PROD);
    printf("NB consomateurs : %d\n", NB_THREADS_CONSO);
    printf("%f ms\n", ((float)TIME_DIFF(tv1, tv2) / 1000));
    printf("------------------------------------------\n");
    return 0;
}
