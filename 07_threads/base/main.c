#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Thread :
// 2 blocks d'execution en parallele qui ont la même mémoire
// Equivalent au sous processus avec fork mais moins lourd car mm mémoire
// Sa vie est liée à son père contrairement à avec un fork

// Fonction qui va être exécuté dans un ou plusieurs threads en parallèle
void *say_hello(void *data) {
    char *str;
    str = (char*) data; // On cast en string
    while(1) {
        printf("%s\n", str);
        sleep(1);
    }
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, say_hello, "Hello thread 1");
    pthread_create(&t2, NULL, say_hello, "Hello thread 2");
    // Param 1 pointeur vers le thread
    // Param 2 paramètre de setup du thread sous forme de struct (Souvent NULL = param par défaut)
    // Param 3 fonction à lancer (Toujours une void, ses param doivent être void*)
    // Param ++ (Arguments de la fonction say_hello)

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    // t1 et t2 sont liés au processu courant (Celui qui les a lancé)
    // Donc si on kill le programme, tous les thread sont tués
}
