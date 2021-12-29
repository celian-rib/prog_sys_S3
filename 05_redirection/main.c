#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// USAGE :
// ./main input.txt output.txt
int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage error : \n");
        printf("./main input.txt output.txt\n");
        exit(1);
    }

    // On ouvre le fichier d'entrée
    int fdInput = open(argv[1], O_RDONLY);
    // On duplique le fichier d'entrée sur l'entrée standard du programme 
    // (rappel : qui est aussi un fichier)
    dup2(fdInput, STDIN_FILENO);
    // On ferme le fichier d'entré
    close(fdInput);

    // On ouvre le fichier de sortie
    int fdOut = open(
        argv[2],            // path
        O_CREAT | O_WRONLY, // flags
        0644                // perms
    );
    // On duplique le fichier de sortie vers la sortie standards 
    dup2(fdOut, STDOUT_FILENO);
    // On ferme le fichier
    close(fdOut);

    // On execute la commande tr pour passer de min à maj (Voir exec)
    execlp("tr", "tr", "[a-z]", "[A-Z]", NULL);
}
