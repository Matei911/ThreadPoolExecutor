/* CALUGARU Matei - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#ifndef _COADA_
#define _COADA_

typedef struct nod_Q
{
    void *data;
    struct nod_Q *urm;
} nod_Q;

typedef struct Queue
{
    nod_Q *inc, *sf;

} Queue;

void push_Q(Queue *q, void *elem);

nod_Q *pop_Q(Queue *q);

Queue *createQueue();

void destroyQueue(Queue **q);

#endif