#include "Jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    struct Jobs jobs;
    jobs_init(&jobs);

    while(1) {
        printf("Lancer une commande : \n");
        char cmd[30];
        read(STDIN_FILENO, cmd, 30);

        jobs_add_job(&jobs, 10, strdup(cmd));
        jobs_affiche_all(&jobs);
    }
}
