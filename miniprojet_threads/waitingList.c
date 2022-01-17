#include "waitingList.h"

void wl_init(struct WaitingList *wl) {
    for (int i = 0; i < MAX_SIZE; i++) {
        wl->array[i] = 0;
    }
    wl->size = 0;
}

int wl_pop(struct WaitingList *wl) {
    if (wl->size <= 0)
        return -1;
    return wl->array[--wl->size];
}

void wl_push(struct WaitingList *wl, int x) {
    wl->array[wl->size++] = x;
}
