#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Action.h"
#include "Jobs.h"
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

    printf("[%d] done (%d)\n", childPid, WEXITSTATUS(status));
    jobs_affiche_un(&(shell_global->jobList), childPid);
    jobs_remove_job(&(shell_global->jobList), childPid);
}

static void do_system(struct Shell *this, const struct StringVector *args) {
    if (string_vector_size(args) == 1) {
        char *env_shell = getenv("SHELL");
        system(env_shell);
        return;
    }

    // Il y a des choses après '!'
    int is_foreground = 1;
    size_t nb_args = string_vector_size(args);

    if (*args->strings[nb_args - 1] == '&') {
        is_foreground = 0;
        nb_args--;
    }

    pid_t child = fork();
    if (child == 0) {
        char **args_filtered = NULL;
        int read_input = 0;
        int read_output = 0;

        for (size_t i = 1; i < nb_args; i++) {
            args_filtered = realloc(args_filtered, (i + 1) * sizeof(char *));

            char *arg_i = strdup(string_vector_get(args, i));

            if (*arg_i == '<') {
                read_input = 1;
            } else if (*arg_i == '>') {
                read_output = 1;
            } else if (read_output) {
                int fdOut = open(arg_i, O_CREAT | O_WRONLY, 0644);
                dup2(fdOut, STDOUT_FILENO);
                close(fdOut);
                
                args_filtered[i] = NULL;
                execvp(args_filtered[0], args_filtered);
            } else if (read_input) {
                int fdIn = open(arg_i, O_RDONLY);
                dup2(fdIn, STDIN_FILENO);
                close(fdIn);
                
                args_filtered[i] = NULL; // dernier argument = NULL (Pour exec)
                execvp(args_filtered[0], args_filtered);
            } else {
                args_filtered[i - 1] = arg_i;
            }
        }

        args_filtered[nb_args - 1] = NULL; // dernier argument = NULL (Pour exec)
        execvp(args_filtered[0], args_filtered);

        // ne devrait jamais arriver ici !!!
        exit(EXIT_SUCCESS);
    }

    if (is_foreground) {
        waitpid(child, NULL, 0);
    } else {
        char *command = (char *)malloc(100 * sizeof(char));
        strjoinarray(command, args, 1, nb_args, " ");
        jobs_add_job(&this->jobList, child, command);
        jobs_affiche_un(&this->jobList, child);
    }

    (void)this;
    (void)args;
}

// if (string_vector_size(args) == 1) {
//     // Premier cas: on a juste tapé "!"
//     // -> on lance un sous shell
//     char *bash = getenv("SHELL"); // On récupère le chemin vers le shell par défault
//     printf("Starting sub-shell\n");
//     system(bash); // On lance le sous shell
//     printf("Ending sub-shell\n");
// } else {
//     // Deuxième cas: On éxécute la commande qui suit le !
//     int isAttached = 1;
//     size_t nbArgs = string_vector_size(args);

//     if (string_vector_get(args, nbArgs - 1)[0] == '&') { // strcmp("&", ...) == 0
//         isAttached = 0;
//         nbArgs--; // Pour ignore & dans les paramètres
//     }

//     pid_t pidChild = fork();

//     if (pidChild == 0) { // Fils
//         struct StringVector argv;
//         string_vector_init(&argv, nbArgs - 1);

//         int entreeALire = 0;
//         int sortieALire = 0;

//         for (size_t i = 1; i < nbArgs; i++) {
//             char *argumentNumeroI = string_vector_get(args, i);
//             if (argumentNumeroI[0] == '<') {
//                 // TODO
//             } else if (argumentNumeroI[0] == '>') {
//             } else if (argumentNumeroI[0] == '<') {
//             } else if (entreeALire) {
//             } else if (sortieALire) {
//             } else {
//                 string_vector_add(&argv, argumentNumeroI, argumentNumeroI + strlen(argumentNumeroI));
//             }
//         }
//         // On ajoute la string NULL à la fin de notre tableau d'arguments
//         string_vector_add(&argv, NULL, NULL);

//         // On fait l'appel à execvp pour exécuter notre commande
//         char *name = string_vector_get(&argv, 0);
//         execvp(name, argv.strings);
//         exit(EXIT_FAILURE); // On ne doit jamais passer ici
//     }

//     // père
//     if (isAttached) {
//         waitpid(pidChild, NULL, 0);
//     } else {
//         // On mémorise les fils
//         char *command = (char *)malloc(100 * sizeof(char));
//         strjoinarray(command, args, 1, nbArgs, " ");
//         jobs_add_job(&this->jobList, pidChild, command);
//         jobs_affiche_un(&this->jobList, pidChild);
//     }
// }
// (void)this;
// (void)args;
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
    printf("Current jobs : \n");
    jobs_affiche_all(&this->jobList);
    (void)this;
    (void)args;
}

static struct {
    const char *name;
    Action action;
} actions[] = {{.name = "exit", .action = do_exit},     {.name = "cd", .action = do_cd},     {.name = "help", .action = do_help},
               {.name = "rappel", .action = do_rappel}, {.name = "jobs", .action = do_jobs}, {.name = "?", .action = do_help},
               {.name = "!", .action = do_system},      {.name = NULL, .action = do_execute}};

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
