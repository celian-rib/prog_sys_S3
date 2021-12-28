#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>
#include <unistd.h>

#include "Jobs.h"

// extern = le shell global 
// est déclaré à un autre endroit
extern struct Shell *shell_global;

struct Shell {
    bool running;
    int line_number;
    char *buffer;
    size_t buffer_size;
    struct Jobs jobList;
};

void shell_init( struct Shell *s );
void shell_run( struct Shell *s );
void shell_free( struct Shell *s );

void shell_read_line( struct Shell *s );
void shell_execute_line( struct Shell *s );

#endif
