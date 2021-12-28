#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Jobs.h"

void job_free( struct Job *job ) {
    free( job->command );
}

void jobs_init( struct Jobs *j ) {
    j->job_list_size = 0;
}

void jobs_free( struct Jobs *j ) {
    for ( int i = 0; i < j->job_list_size; i++ ) {
        job_free( &j->job_list[i] );
    }
}

void jobs_add_job( struct Jobs *j, pid_t p, char *command ) {
    if ( j->job_list_size < 100 ) {
        j->job_list[j->job_list_size].pid = p;
        j->job_list[j->job_list_size].command = command;
        j->job_list_size++;
    }
}

pid_t jobs_remove_job( struct Jobs *j, pid_t p ) {
    if ( !p )
        return 0;
    for ( int i = 0; i < j->job_list_size; i++ ) {
        if ( j->job_list[i].pid == p ) {
            job_free( &j->job_list[i] );
            j->job_list_size--;
            j->job_list[i] = j->job_list[j->job_list_size];
            j->job_list[j->job_list_size].pid = 0;
            j->job_list[j->job_list_size].command = NULL;
            return p;
        }
    }
    return 0;
}

void jobs_affiche_un( struct Jobs *j, pid_t p ) {
    if ( !p )
        return;

    for ( int i = 0; i < j->job_list_size; i++ ) {
        if ( j->job_list[i].pid == p ) {
            printf( "[%d] %s\n", j->job_list[i].pid, j->job_list[i].command );
        }
    }
}

void jobs_affiche_all( struct Jobs *j ) {
    for ( int i = 0; i < j->job_list_size; i++ ) {
        if ( j->job_list[i].pid ) {
            printf( "[%d] %s\n", j->job_list[i].pid, j->job_list[i].command );
        }
    }
}
