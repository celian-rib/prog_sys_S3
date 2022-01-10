#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // Ecrire dans le fichier STDOUT_FILENO (Soit la sortie standard = le terminal)
    write(STDOUT_FILENO, "Ecrire : ", 10);

    // Lecture de l'entrée standard
    char tampon[100]; // Variable dans laquelle on va mettre ce qu'on a lu
    int nb = read(STDIN_FILENO, tampon, 100); // Lire un fichier (Ici STDIN_FILENO soit l'entrée standard)

    // On écrit le résultat :
    write(STDOUT_FILENO, "J'ai lu : ", 10);
    write(STDOUT_FILENO, tampon, nb);
}
