#ifndef ACTION_H
#define ACTION_H

#include "Shell.h"
#include "StringVector.h"

// Definition du type action
typedef void (*Action)(struct Shell *, const struct StringVector *);

Action get_action(char *name);
void apply_action(char *name, struct Shell *, const struct StringVector *);

void fin_fils(int sig);
#endif
