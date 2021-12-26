#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    printf("Hello world\n");

    if(fork()==0) {
        execl("/bin/ls", "ls", "-l", NULL);
    }
    int s;
    wait(&s);
    return 0;
}
