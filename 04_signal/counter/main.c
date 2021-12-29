#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Faire un programme qui :
// - incrémente et affiche un compteur quand il recoit SIGUSR1
// s'arrete de lui même au bout de 30 secondes

void sig_handler(int sig) {
    static int counter = 0;
    printf("SIGUSR1 %d\n", ++counter);
}

int main(void) {
    signal(SIGUSR1, sig_handler);

    pid_t pid = getpid();

    for (int i = 0; i < 10; i++) { // On va créer plein d'enfants
        if (fork() == 0) {         // Chaque enfant emet 10 signaux
            for (int i = 0; i < 10; i++) {
                sleep(1);
                kill(pid, SIGUSR1);
            }
        }
    }

    for (int i = 0; i < 30; i++) // Attente 30 secs
        sleep(1);
}
