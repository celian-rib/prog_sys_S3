#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Action.h"
#include "StringVector.h"

static void do_help(struct Shell *this, const struct StringVector *args) {
    printf("Tapez exit pour arrêter.\n");
    printf("Tapez ! pour lancer un sous shell.\n");
    printf("Tapez ! suivit d'une commande pour l'exécuter.\n");
    printf("Tapez cd <nom de dossier> pour changer de dossier\n");
    (void)this;
    (void)args;
}

static void do_exit(struct Shell *this, const struct StringVector *args) {
    this->running = false;
    (void)args;
}

static void do_cd(struct Shell *this, const struct StringVector *args) {
    char *monChemin;
    if (string_vector_size(args) == 1) {
        // Si cd est seul, notre chemin par défaut est le HOME
        monChemin = getenv("HOME");
    } else {
        // Sinon notre chemin est le mot qui suit cd dans notre commande
        monChemin = string_vector_get(args, 1);
    }
    chdir(monChemin); // On change de répertoire
    (void)this;
}

static void do_execute(struct Shell *this, const struct StringVector *args) {
    printf("Commande inconnue\n");
    (void)this;
    (void)args;
}

void fin_fils(int sig) {
    (void)sig;
    int status;
    pid_t childPid = wait(&status);

    if (-1 == childPid) { // le processus était attaché
        return;
    }

    printf("fin de [%d] avec le code de retour %d !\n", childPid, WEXITSTATUS(status));
    jobs_affiche_un(&(shell_global->jobList), childPid);
    jobs_remove_job(&(shell_global->jobList), childPid);
}

static void do_system(struct Shell *this, const struct StringVector *args) {
    if (string_vector_size(args) == 1) {
        // Premier cas: on a juste tapé "!"
        // -> on lance un sous shell
        char *bash = getenv("SHELL"); // On récupère le chemin vers le shell par défault
        printf("Starting sub-shell\n");
        system(bash); // On lance le sous shell
        printf("Ending sub-shell\n");
    } else {
        // Deuxième cas: On éxécute la commande qui suit le !
        int isAttached = 1;
        size_t nbArgs = string_vector_size(args);

        if (string_vector_get(args, nbArgs - 1)[0] == '&') { // strcmp("&", ...) == 0
            isAttached = 0;
            nbArgs--; // Pour ignore & dans les paramètres
        }

        pid_t pidChild = fork();

        if (pidChild == 0) { // Fils
            struct StringVector argv;
            string_vector_init(&argv, nbArgs - 1);

            int entreeALire = 0;
            int sortieALire = 0;

            for (size_t i = 1; i < nbArgs; i++) {
                char *argumentNumeroI = string_vector_get(args, i);
                if (argumentNumeroI[0] == '<') {
                    // TODO
                } else if (argumentNumeroI[0] == '>') {
                } else if (argumentNumeroI[0] == '<') {
                } else if (entreeALire) {
                } else if (sortieALire) {
                } else {
                    string_vector_add(&argv, argumentNumeroI,
                                      argumentNumeroI + strlen(argumentNumeroI));
                }
            }
            // On ajoute la string NULL à la fin de notre tableau d'arguments
            string_vector_add(&argv, NULL, NULL);

            // On fait l'appel à execvp pour exécuter notre commande
            char *name = string_vector_get(&argv, 0);
            execvp(name, argv.strings);
            exit(EXIT_FAILURE); // On ne doit jamais passer ici
        }

        // père
        if (isAttached) {
            waitpid(pidChild, NULL, 0);
        } else {
            // On mémorise les fils
            char *command = (char *)malloc(100 * sizeof(char));
            strjoinarray(command, args, 1, nbArgs, " ");
            jobs_add_job(&this->jobList, pidChild, command);
            jobs_affiche_un(&this->jobList, pidChild);
        }
    }
    (void)this;
    (void)args;
}

static void do_rappel(struct Shell *this, const struct StringVector *args) {
    pid_t p = fork(); // On fork
    if (!p) {         // Si on est le fils, on fait des trucs
        int secondes = atoi(string_vector_get(args, 1));
        sleep(secondes);

        char message[1000];
        strjoinarray(message, args, 2, string_vector_size(args), " ");
        printf("RAPPEL : %s\n", message);
        exit(EXIT_SUCCESS); // Le fils doit finir son exécution
    }
    (void)this;
    (void)args;
}

static void do_jobs(struct Shell *this, const struct StringVector *args) {
    jobs_affiche_all(&this->jobList);
    (void)this;
    (void)args;
}

static struct {
    const char *name;
    Action action;
} actions[] = {{.name = "exit", .action = do_exit}, {.name = "cd", .action = do_cd},
               {.name = "help", .action = do_help}, {.name = "rappel", .action = do_rappel},
               {.name = "jobs", .action = do_jobs}, {.name = "?", .action = do_help},
               {.name = "!", .action = do_system},  {.name = NULL, .action = do_execute}};

Action get_action(char *name) {
    int i = 0;
    while (actions[i].name != NULL && strcmp(actions[i].name, name) != 0) {
        i++;
    }
    return actions[i].action;
}

void apply_action(char *name, struct Shell *s, const struct StringVector *sV) {
    Action action = get_action(name);
    action(s, sV);
}
