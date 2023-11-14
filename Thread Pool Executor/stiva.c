/* CALUGARU Matei - 314CB */
#include "stiva.h"

Stack *createStack()
{
    Stack *s = (Stack *)malloc(sizeof(Stack));
    if (!s)
        return NULL;
    s->inc = NULL;
    return s;
}
void push_S(void *elem, Stack *s)
{
    nod_stv *tmp = (nod_stv *)malloc(sizeof(nod_stv));
    if (tmp != NULL)
    {
        tmp->data = elem;
        tmp->urm = s->inc;
        s->inc = tmp;
    }
}

nod_stv *pop_S(Stack *s)
{
    if (s->inc == NULL)
        return NULL;
    nod_stv *tmp = s->inc;
    s->inc = s->inc->urm;
    return tmp;
}
void destroyStack(Stack **s)
{
    while ((*s)->inc != NULL)
    {
        nod_stv *temp = pop_S(*s);
        free(temp->data);
        free(temp);
    }
    free(*s);
    *s = NULL;
}
