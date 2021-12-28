#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "StringVector.h"
#include "shell.h"

typedef void (*Action)(
    struct Shell *,
    const struct StringVector *);

Action minishell_exit(struct Shell *s, const struct StringVector *v);

Action help(struct Shell *s, const struct StringVector *v);

Action sub_shell(struct Shell *s, const struct StringVector *v);

Action current_dir(struct Shell *s, const struct StringVector *v);

Action list_dir(struct Shell *s, const struct StringVector *v);

Action rappel(struct Shell *s, const struct StringVector *v);

#endif
