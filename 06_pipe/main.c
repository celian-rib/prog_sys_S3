#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Creer 2 processus
// 1 va envoyer des chiffres de 1 a 10 (Ecrire dans la sortie du pipe)
// 2 va récupérer tout les message de l'autre processus et les afficher (Lire l'entrée du pipe)

struct Message {
    int data;
};

void produire(int sortie) {
    for (int i = 0; i < 10; i++) {
        struct Message m;
        m.data = i;
        write(sortie, &m, sizeof(m));
    }
    close(sortie);
}

void consommer(int entree) {
    while(1) {
        struct Message m;
        int r = read(entree, &m, sizeof(m));
        if (r != sizeof(m))
            break;
        printf("%d\n", m.data);
    }
    close(entree);
}

int main() {
    // Création du pipe
    int fds[2];
    pipe(fds);

    int entree = fds[0];
    int sortie = fds[1];

    // Création fils (Emetteur)
    if (fork() == 0) {
        close(entree);
        produire(sortie);
        exit(EXIT_SUCCESS);
    }

    // Père (Receveur)
    close(sortie);
    consommer(entree);
    wait(NULL);
    exit(EXIT_SUCCESS);
}
