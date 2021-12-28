#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void sig_handler(int sig) {
    printf("recue SIGINT\n");
    exit(0);
}

int main(void) {
    signal(SIGINT, sig_handler);
    while (1) sleep(1); // Attente infinie
}
