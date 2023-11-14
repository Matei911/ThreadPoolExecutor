/* CALUGARU Matei - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifndef _STIVA_
#define _STIVA_

typedef struct nod_stv
{
    void *data;
    struct nod_stv *urm;
} nod_stv;

typedef struct Stack
{
    nod_stv *inc;
} Stack;

void push_S(void *elem, Stack *s);

nod_stv *pop_S(Stack *s);

Stack *createStack();

void destroyStack(Stack **s);

#endif