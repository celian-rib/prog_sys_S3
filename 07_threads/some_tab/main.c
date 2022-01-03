#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define TAILLE_TAB 100000000
#define THREAD_COUNT 4

#define TIME_DIFF(t1, t2) ((t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec))

/**
 * Argument donnée à chaque thread en paramètre
 */
struct args_somme_tranche {
    double *tab; // Tab sur lequel faire la somme
    int debut; // Point de départ pour ce thread
    int fin; // Point de fin pour ce thread
    double *somme; // Somme trouvée pat ce thread
};

/**
 * Fait la somme sur une partie donnée du tableau
 */
double somme_tranche(double *tab, int debut, int fin) {
    double sum = 0;
    for (int i = debut; i < fin; i++)
        sum += tab[i];
    return sum;
}

void *thread_somme(void *data) {
    struct args_somme_tranche *a = data;
    *(a->somme) = somme_tranche(a->tab, a->debut, a->fin);
}

void remplir_tab(double *tab, int size) {
    for (int i = 0; i < size; i++)
        tab[i] = rand() % 100;
}

struct args_somme_tranche *create_args(int count, double *tab, double *sommes) {
    struct args_somme_tranche *threads = malloc(count * sizeof(struct args_somme_tranche));
    for (int i = 0; i < count; i++) {
        threads[i] = (struct args_somme_tranche){
            .tab = tab,
            .debut = i * TAILLE_TAB / count,
            .fin = (i + 1) * TAILLE_TAB / count,
            .somme = &sommes[i],
        };
    }
    threads[count - 1].fin = TAILLE_TAB;
    return threads;
}

int main() {
    struct timeval tv1, tv2; // Timer benchmark

    // Création tableau sur lequel on va faire la somme
    double *tab = malloc(sizeof(double) * TAILLE_TAB);
    remplir_tab(tab, TAILLE_TAB);

    // Création du tableau des sous sommes qui vont être données par les threads
    double *sommes = malloc(sizeof(double) * TAILLE_TAB);

    
    // Création de tous les threads
    // 1 arguments pour chaque thread
    struct args_somme_tranche *args = create_args(THREAD_COUNT, tab, sommes);
    // 2 threads
    pthread_t threads[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++) // Création
        pthread_create(&threads[i], NULL, thread_somme, &args[i]);

    // On lance tous les threads
    gettimeofday(&tv1, NULL);
    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_join(threads[i], NULL);

    // On récupère toutes les sommes et on fait le total
    double total = 0;
    for (int i = 0; i < THREAD_COUNT; i++)
        total += *args[i].somme;

    gettimeofday(&tv2, NULL);
    printf("\n--------------%d THREADS------------------\n", THREAD_COUNT);
    printf("Total %f\n", total);
    printf("%d threads : %f ms\n", THREAD_COUNT, ((float)TIME_DIFF(tv1, tv2) / 1000));
    printf("------------------------------------------\n");

    gettimeofday(&tv1, NULL);
    printf("\n--------------NO THREADS------------------\n");
    printf("Total %f\n", somme_tranche(tab, 0, TAILLE_TAB));
    gettimeofday(&tv2, NULL);
    printf("No threads : %f ms\n", ((float)TIME_DIFF(tv1, tv2) / 1000));
    printf("------------------------------------------\n");
}
