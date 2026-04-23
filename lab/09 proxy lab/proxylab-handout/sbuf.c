#include "sbuf.h"

void sbuf_init(sbuf_t* sbuf, int n) {
    sbuf->buf = (int*)Calloc(sizeof(int), n);
    sbuf->n = n;
    sbuf->front = sbuf->rear = 0;
    Sem_init(&sbuf->mutex, 0, 1);
    Sem_init(&sbuf->items, 0, 0);
    Sem_init(&sbuf->slots, 0, n);
}

void sbuf_free(sbuf_t* sbuf) {
    Free(sbuf->buf);
}

void sbuf_insert(sbuf_t* sbuf, int item) {
    P(&sbuf->slots);
    P(&sbuf->mutex);
    sbuf->buf[(sbuf->rear++) % sbuf->n] = item;
    V(&sbuf->mutex);
    V(&sbuf->items);
}

int sbuf_remove(sbuf_t* sbuf) {
    int item;
    P(&sbuf->items);
    P(&sbuf->mutex);
    item = sbuf->buf[(sbuf->front++) % sbuf->n];
    V(&sbuf->mutex);
    V(&sbuf->slots);
    return item;
}