#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "StringVector.h"
#include "actions.h"

#define Reset "\033[0m"
#define Purple "\033[0;35m"
#define Yellow "\033[0;33m"
#define Cyan "\033[0;36m"

static struct {
    const char *name;
    Action action;
    const char *description;
} actions[5] = {
    {.name = "rappel", .action = rappel},       {.name = "ls", .action = list_dir},
    {.name = "exit", .action = minishell_exit}, {.name = "cd", .action = current_dir},
    {.name = "help", .action = help},           {.name = "!", .action = sub_shell},
};

Action get_action(char *name) {
    for (int i = 0; i < 5; i++)
        if (strcmp(actions[i].name, name) == 0)
            return actions[i].action;
    return NULL;
}

void shell_init(struct Shell *s) {
    s->running = false;
    s->line_number = 0;
    s->buffer_size = 0;
    s->buffer = malloc(BUFFER_CAPACITY * sizeof(char));
}

void shell_read_line(struct Shell *s) {
    free(s->buffer);
    s->buffer = calloc(BUFFER_CAPACITY, sizeof(char));
    char cwd[100];
    getcwd(cwd, sizeof(cwd));
    printf("%s> %s[%d]%s %s %s$ ", Yellow, Purple, ++s->line_number, Cyan, cwd, Reset);
    fflush(stdout);
    int line_length = read(STDIN_FILENO, s->buffer, BUFFER_CAPACITY);
    s->buffer_size = line_length;
}

// args.strings = { "ls" , "-l" };
// args.strings = { "ls" , "-l", ">", "r.txt"};
// args.strings = { "<", "f.cc", "wc", "-l", ">", "r.txt"};

void shell_execute_line(struct Shell *s) {
    struct StringVector tokens = split_line(s->buffer);
    int nb_tokens = string_vector_size(&tokens);

    if (nb_tokens == 0) {
        string_vector_free(&tokens);
        return;
    }

    char *name = string_vector_get(&tokens, 0);
    Action action = get_action(name);
    if (action == NULL) {
        pid_t pid = fork();
        if (pid == 0) {
            execvp(name, tokens.strings);
            // if(string_vector_get(&tokens, nb_tokens - 1) = '&')
            exit(EXIT_SUCCESS);
        }
        waitpid(pid, NULL, 0);
    } else
        action(s, &tokens);

    string_vector_free(&tokens);
}

void shell_run(struct Shell *s) {
    s->running = true;
    while (s->running) {
        shell_read_line(s);
        shell_execute_line(s);
    }
}

void shell_free(struct Shell *s) {
    free(s->buffer);
}