#include <semaphore.h>
#include "csapp.h"

typedef struct {
    int *buf;
    int n;
    int front, rear;
    sem_t mutex;
    sem_t slots;
    sem_t items;
} sbuf_t;

void sbuf_init(sbuf_t* sbuf, int n);
void sbuf_free(sbuf_t* sbuf);
void sbuf_insert(sbuf_t* sbuf, int item);
int sbuf_remove(sbuf_t* sbuf);