/* CALUGARU Matei - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "stiva.h"
#include "coada.h"
#ifndef _SCHED_
#define _SCHED_

typedef struct Thread
{
    int ID;
} Thread;

typedef struct Task
{
    int ID;
    int threadID;
    int priority;
    int exec_time; // timpul ramas de rulat
    int init_time; // timpul initial necesar terminarii task-ului

} Task;
typedef struct Sched
{
    int Q, C, N;
    Stack *stack;
    Queue *q_wait, *q_run, *q_fin;
    int nMaxTaskIDS;
    char *available_task_id;
    FILE *f_out;
    int T_total;
} Sched;

Sched *createSched(int Q, int C, FILE *f_out);
// initializeaza toate cozile si datele necesare problemaei
void destroySched(Sched **sch);
// elibereaza memoria
void ins_Q_wait(int nr_task, int exec_time, int pry, Sched *sch);
// creeaza task-urile noi
void get_task(int task_id, Sched *sch);
// functia cauta si afiseaza task-ul ce are id-ul task_id
void get_thread(int thread_id, Sched *sch);
// functia cauta si afiseaza toate thread-urile ce au id-ul thread_id
void print_Q(Sched *sch, char *tip);
// functie de afisare a cozilor
void run(Sched *sch, int T);
// executa task-urile in T timp
void finish(Sched *sch);
// este calculat timpul total, pana cand toate task-urile sunt in coada de finish

#endif