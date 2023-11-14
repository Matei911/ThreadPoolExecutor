/* CALUGARU Matei - 314CB */
#include "coada.h"

nod_Q *newNode(void *elem)
{
    nod_Q *temp = (nod_Q *)malloc(sizeof(nod_Q));
    if (!temp)
        return 0;
    temp->data = elem;
    temp->urm = NULL;
    return temp;
}
Queue *createQueue()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (!q)
        return NULL;
    q->inc = q->sf = NULL;
    return q;
}
void push_Q(Queue *q, void *elem)
{
    nod_Q *temp = newNode(elem);
    if (q->sf == NULL)
    {
        q->sf = q->inc = temp;
        return;
    }
    q->sf->urm = temp;
    q->sf = temp;
}

nod_Q *pop_Q(Queue *q)
{
    if (q->sf == NULL)
        return NULL;
    nod_Q *temp = q->inc;
    q->inc = q->inc->urm;
    if (q->inc == NULL)
        q->sf = NULL;
    return temp;
}
void destroyQueue(Queue **q)
{
    while ((*q)->inc != NULL)
    {
        nod_Q *temp = pop_Q(*q);
        free(temp->data);
        free(temp);
    }
    free(*q);
    *q = NULL;
}