#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    

    int fdInput = open(argv[1], O_RDONLY);
    dup2(fdInput, STDIN_FILENO);
    close(fdInput);

    int fdOut = open(argv[2], O_CREAT | O_WRONLY, 0644);
    dup2(fdOut, STDOUT_FILENO);
    close(fdOut);

    execlp("tr", "tr", "[a-z]", "[A-Z]", NULL);
}
