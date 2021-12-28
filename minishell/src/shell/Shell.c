#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Action.h"
#include "Shell.h"
#include "StringVector.h"

struct Shell *shell_global;

void shell_init(struct Shell *s) {
    s->running = false;
    s->line_number = 0;
    s->buffer_size = 100;
    s->buffer = (char *)malloc(s->buffer_size * sizeof(char));
    jobs_init(&s->jobList);
    shell_global = s;

    // Armement du signal
    /*
       signal( SIGCHLD, fin_fils );
    */
    // SIGCHLD causing EOF in parent process
    // https://stackoverflow.com/questions/53451014/sigchld-causing-eof-in-parent-process)
    // getline() in shell_read_line() will reach EOF, and stop

    // forget about the obsolete signal(), and use sigaction() with the
    // SA_RESTART flag
    struct sigaction sa;
    sa.sa_handler = fin_fils; // la fonction à appeler lorsque le signal est reçu
    sa.sa_flags = SA_RESTART; // Restart functions if interrupted by handler

    printf("Installing signal handler for signal %d\n", SIGCHLD);
    int retval = sigaction(SIGCHLD, &sa, NULL);

    if (retval < 0) {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }
}

void shell_run(struct Shell *s) {
    if (!s)
        return; // Si le pointeur s est NULL, on passe notre chemin.
    s->running = true;
    printf("* Shell started\n");
    while (s->running) {
        shell_read_line(s);
        shell_execute_line(s);
    }
    printf("* Shell stopped\n");
}

void shell_free(struct Shell *s) {
    free(s->buffer);
    jobs_free(&s->jobList);
}

void shell_read_line(struct Shell *s) {
    // Affichage du début du shell. Ex: 12: ~/Documents/>
    char cwd[1000];
    getcwd(cwd, sizeof(cwd));                // On récupère le répertoire courant
    printf("%d: %s> ", s->line_number, cwd); // On affiche l'entête
    fflush(stdout);                          // Pour forcer l'affichage

    // Lecture de la ligne
    getline(&s->buffer, &s->buffer_size, stdin);
    s->line_number++;
}

void shell_execute_line(struct Shell *s) {
    // On sépare la ligne en mots
    struct StringVector tokens = split_line(s->buffer);
    if (string_vector_size(&tokens) == 0)
        return; // Si la ligne est vide (pas de mots), on passe notre chemin.

    char *name = string_vector_get(&tokens, 0);
    apply_action(name, s, &tokens);

    string_vector_free(&tokens);
}
