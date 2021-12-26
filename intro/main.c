#include <stdio.h>
#include <unistd.h>

// int main() {
//     char tampon [ ] = "Hello, world 123\n";
//     write(STDOUT_FILENO, tampon, 17);
//     return 0;
// }

// int main() {
//     write(STDOUT_FILENO, "Ecrire : ", 10);

//     char tampon[17];
//     int nb = read(STDIN_FILENO, tampon, 100);

//     write(STDOUT_FILENO, "J'ai lu : ", 10);

//     write(STDOUT_FILENO, tampon, nb);
// }

int main() {
    char *input;
    int lineLength = read(STDIN_FILENO, input, 200);
    
}
