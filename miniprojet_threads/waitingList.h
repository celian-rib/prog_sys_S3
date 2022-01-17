#ifndef WAITINGLIST_H
#define WAITINGLIST_H

#define MAX_SIZE 20

struct WaitingList {
    int array[MAX_SIZE];
    int size;
};

void wl_init(struct WaitingList *wl);

int wl_pop(struct WaitingList *wl);
void wl_push(struct WaitingList *wl, int x);

#endif