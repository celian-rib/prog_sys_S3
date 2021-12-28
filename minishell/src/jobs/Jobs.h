#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>

struct Job {
    pid_t pid;
    char *command;
};

void job_free(struct Job *);

struct Jobs {
    struct Job job_list[100];
    int job_list_size;
};

void jobs_init(struct Jobs *);
void jobs_free(struct Jobs *);
void jobs_add_job(struct Jobs *, pid_t p, char *command);
pid_t jobs_remove_job(struct Jobs *, pid_t p);
void jobs_affiche_un(struct Jobs *j, pid_t p);
void jobs_affiche_all(struct Jobs *);

#endif
