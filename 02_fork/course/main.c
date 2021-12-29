#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int nb_courreur_par_equipe = 4;

struct Equipe {
    pid_t pid;
    char *name;
};

int random_range(int a, int b, int seed) {
    srand(time(0) + seed);
    int r = a + rand() % (b - a + 1);
    return r;
}

void course_equipe(const struct Equipe *equipe) {
    for (int i = 0; i < nb_courreur_par_equipe; i++) {
        printf("Le courreur %d de l'equipe %s est partit.\n", i + 1, equipe->name);
        sleep(random_range(1, 4, getpid()));
        if (random_range(0, 100, getpid() + i) < 10)
            exit(EXIT_FAILURE);
    }
    printf("L'équipe %s est arrivée !\n", equipe->name);
}

int main(int argc, char **argv) {
    int nb_equipes = argc - 1;

    if (nb_equipes == 0) {
        printf("Aucune équipe...\n");
        return 0;
    }
    printf("%d équipes sur le départ\n", nb_equipes);

    struct Equipe equipes[nb_equipes];

    printf("* début de la course\n");

    for (int i = 0; i < nb_equipes; i++) {
        equipes[i].name = argv[i + 1];
        equipes[i].pid = fork();
        if (equipes[i].pid == 0) {
            course_equipe(&equipes[i]);
            return 0;
        }
    }

    struct Equipe *arrivees[nb_equipes];
    int nb_arrivees = 0;

    for (int i = 0; i < nb_equipes; i++) {
        int status;
        pid_t p = waitpid(-1, &status, 0);
        if (WEXITSTATUS(status) == EXIT_SUCCESS)
            for (int i = 0; i < nb_equipes; i++)
                if (equipes[i].pid == p)
                    arrivees[nb_arrivees++] = &equipes[i];
    }

    printf("* fin de la course\n");

    for (int i = 0; i < nb_arrivees; i++) {
        printf("%d - %s\n", i + 1, arrivees[i]->name);
    }

    if (nb_equipes - nb_arrivees != 0)
        printf("%d équipe éliminée(s)\n", nb_equipes - nb_arrivees);

    return 0;
}
