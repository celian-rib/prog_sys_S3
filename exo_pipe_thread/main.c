#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

struct Message {
    int data;
};

struct ThreadArg {
    int descripteur;
    int *nb_produit;
    int *nb_prod_threads;
    pthread_mutex_t *mutex;
};

void *thread_produire(void *args) {
    struct ThreadArg *a = args;
    for (int i = 0; i < 20; i++) {
        pthread_mutex_lock(a->mutex);

        struct Message m;
        m.data = ++(*a->nb_produit);
        write(a->descripteur, &m, sizeof(m));

        pthread_mutex_unlock(a->mutex);
    }
    if (--(*a->nb_prod_threads) == 0)
        close(a->descripteur);
    return NULL;
}

void *thread_consommer(void *args) {
    struct ThreadArg *a = args;
    while (1) {
        struct Message m;
        int r = read(a->descripteur, &m, sizeof(m));
        if (r != sizeof(m))
            break;
        printf("%d\n", m.data);
    }
    close(a->descripteur);
    return NULL;
}

int main() {

    // Création du mutex pour les threads
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    // Création du pipe
    int fds[2];
    pipe(fds);
    int entree = fds[0];
    int sortie = fds[1];

    // Variable a partager entre les producteurs
    int nb_produit = 0;

    struct ThreadArg conso = {
        .descripteur = entree,
        .mutex = &mutex,
        .nb_produit = &nb_produit,
    };

    int nb_prod_threads = 4;
    struct ThreadArg prod = {
        .descripteur = sortie,
        .mutex = &mutex,
        .nb_produit = &nb_produit,
        .nb_prod_threads = &nb_prod_threads,
    };

    pthread_t c;
    pthread_create(&c, NULL, thread_consommer, &conso);

    pthread_t *ps = malloc(sizeof(pthread_t) * nb_prod_threads);
    for (int i = 0; i < nb_prod_threads; i++) {
        pthread_create(&ps[i], NULL, thread_produire, &prod);
    }

    pthread_join(c, NULL);
    for (int i = 0; i < nb_prod_threads; i++)
        pthread_join(ps[i], NULL);
}
