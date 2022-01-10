#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// int main() {
//     char tampon [ ] = "Hello, world 123\n";
//     write(STDOUT_FILENO, tampon, 17);
//     return 0;
// }

// int main() {
//     write(STDOUT_FILENO, "Ecrire : ", 10);
//     char tampon[100];
//     int nb = read(STDIN_FILENO, tampon, 100);
//     write(STDOUT_FILENO, "J'ai lu : ", 10);
//     write(STDOUT_FILENO, tampon, nb);
// }

// int main() {
//     chdir("/bin");
//     system("ls");
// }

// typedef void (*une_fct)();

// void manger() {
//     printf("Je mange\n");
// }

// void dormir(int heures) {
//     printf("Je dors %d heures\n", heures);
// }

// int main() {
//     une_fct a = manger;
//     a(); // Appel à manger()
//     a = dormir;
//     a(8); // Appel à dormir avec un paramètre
// }

// int main() {
//     pid_t p = fork();
//     if (p == 0)
//         exit(EXIT_FAILURE);
//     int status;
//     waitpid(p, &status, 0);
//     printf("le s'est terminé avec le status %d\n", status);
// }

// int main() {
//     execl ("/bin/ls", "ls", "-la", NULL);
// }

int main() {
    for(int i = 0; i < 2; i++)
        fork();
    printf("pid = %d\n", getpid());
}
