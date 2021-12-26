#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#include "StringVector.h"
#include "shell.h"

void minishell_exit(struct Shell *s, const struct StringVector *v) {
    s->running = false;
    (void)s;
    (void)v;
}

void help(struct Shell *s, const struct StringVector *v) {
    printf("- exit : quit the shell\n");
    printf("- help : show this help menu\n");
    (void)s;
    (void)v;
}

void sub_shell(struct Shell *s, const struct StringVector *v) {
    printf("Starting sub-shell\n");
    system(getenv("SHELL"));
    printf("Ending sub-shell\n");
    (void)s;
    (void)v;
}

void current_dir(struct Shell *s, const struct StringVector *v) {
    char *arg = string_vector_get(v, 1);
    if (arg == NULL)
        arg = getenv("HOME");
    if (chdir(arg) != 0)
        printf("%s not found\n", arg);
    (void)s;
}

void list_dir(struct Shell *s, const struct StringVector *v) {
    system("ls");
    (void)s;
    (void)v;
}

void rappel(struct Shell *s, const struct StringVector *v) {
    char *rap = string_vector_get(v, 1);
    char *arg = string_vector_get(v, 2);
    int time = atoi(arg);

    if (time == 0) {
        printf("Erreur: usage\n rappel <name> <time>\n");
        return;
    }

    pid_t r = fork();
    if (r == 0) {
        sleep(time);
        printf("%s\n", rap);
        exit(EXIT_SUCCESS);
    }
    (void)s;
    (void)v;
}
