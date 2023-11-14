/* CALUGARU Matei - 314CB */
#include "scheduler.h"
#include <stdbool.h>

Stack *init_thread_pool(int N)
{
    // aloca memorie pentru stiva de thred-uri si initializeaza cu id-uri thread-urile
    Stack *s = createStack();

    while (N != 0)
    {
        Thread *tmp = malloc(sizeof(Thread));
        tmp->ID = N - 1;
        push_S((void *)tmp, s);
        N--;
    }
    return s;
}
Sched *createSched(int Q, int C, FILE *f_out)
{
    Sched *sch = malloc(sizeof(Sched));
    sch->C = C;
    sch->Q = Q;
    sch->N = 2 * C;
    sch->nMaxTaskIDS = 32768;
    sch->stack = init_thread_pool(sch->N);
    sch->q_wait = createQueue();
    sch->q_run = createQueue();
    sch->q_fin = createQueue();
    sch->available_task_id = malloc((sch->nMaxTaskIDS + 1) * sizeof(char));
    sch->f_out = f_out;
    sch->T_total = 0;
    int i = 1;
    for (i = 1; i <= sch->nMaxTaskIDS; i++)
        sch->available_task_id[i] = 0;
    return sch;
}
void destroySched(Sched **sch)
{
    free((*sch)->available_task_id);
    destroyQueue(&((*sch)->q_fin));
    destroyQueue(&((*sch)->q_run));
    destroyQueue(&((*sch)->q_wait));
    destroyStack(&((*sch)->stack));
    free(*sch);
    *sch = NULL;
}
int index_disp(char available_task_id[], int nMaxTaskIDS)
{
    // vector de aparitii
    //  returneaza primul task id disponibil
    int i;
    for (i = 1; i <= nMaxTaskIDS; i++)
        if (available_task_id[i] == 0)
            return i;
    return 0;
}

Queue *build_Q_aux(int nr_task, int exec_time, int pry, char available_task_id[], int nMaxTaskIDS, FILE *f_out)
{
    // cosntruieste o coada auxiliara cu timp, prioritate, thread si id-uri disponibile
    // afiseaza mesaj dupa ce creeaza
    Queue *qaux = createQueue();
    int i = 1;
    for (i = 1; i <= nr_task; i++)
    {
        Task *tmp = malloc(sizeof(Task));
        tmp->priority = pry;
        tmp->exec_time = exec_time;
        tmp->init_time = exec_time;
        tmp->threadID = -1;
        tmp->ID = index_disp(available_task_id, nMaxTaskIDS);
        available_task_id[tmp->ID] = 1;
        push_Q(qaux, (void *)tmp);
        fprintf(f_out, "Task created successfully : ID %d.\n", tmp->ID);
    }
    return qaux;
}
Queue *interclasare_cozi(Queue *q1, Queue *q2)
{
    // ordonarea cozi cu metoada de interclasare
    /*folosim doua cozi prima este coada veche,
     iar a doua coada contine noile task-uri*/
    nod_Q *elem1 = pop_Q(q1);
    nod_Q *elem2 = pop_Q(q2);
    Task *data1;
    Task *data2;
    Queue *q_new = createQueue();
    while ((elem1 != NULL) && (elem2 != NULL))
    {

        data1 = (Task *)(elem1->data);
        data2 = (Task *)(elem2->data);
        bool primul;
        if (data1->priority > data2->priority)
            primul = true;
        else if (data1->priority < data2->priority)
            primul = false;
        else
        {
            if (data1->exec_time < data2->exec_time)
                primul = true;
            else if (data1->exec_time > data2->exec_time)
                primul = false;
            else
            {
                if (data1->ID < data2->ID)
                    primul = true;
                else
                    primul = false;
            }
        }
        if (primul == true)
        {
            push_Q(q_new, (void *)data1);
            free(elem1);
            elem1 = pop_Q(q1);
        }
        else
        {
            push_Q(q_new, (void *)data2);
            free(elem2);
            elem2 = pop_Q(q2);
        }
    }
    // in caz ca mai raman elemente in coada
    Queue *q_remaining;
    nod_Q *elem;
    Task *data;
    if (elem1 != NULL)
    {
        q_remaining = q1;
        elem = elem1;
    }
    else
    {
        q_remaining = q2;
        elem = elem2;
    }
    while (elem != NULL)
    {
        data = (Task *)(elem->data);
        push_Q(q_new, (void *)data);
        free(elem);
        elem = pop_Q(q_remaining);
    }
    destroyQueue(&q1);
    destroyQueue(&q2);
    return q_new;
}
void ins_Q_wait(int nr_task, int exec_time, int pry, Sched *sch)
{
    Queue *q_aux = build_Q_aux(nr_task, exec_time, pry, sch->available_task_id, sch->nMaxTaskIDS, sch->f_out);
    sch->q_wait = interclasare_cozi(sch->q_wait, q_aux);
}
bool get_task_general(int task_id, Queue **q, char *mesaj, FILE *out)
{
    // folosim o coada auxiliara pentru afisarea task-ului din coada potrivita cu task id
    bool exista_task = false;
    nod_Q *elem = pop_Q(*q);
    Queue *q_aux = createQueue();
    while (elem != NULL)
    {

        Task *data = (Task *)(elem->data);
        if ((data->ID == task_id) && (!exista_task))
        {
            fprintf(out, "Task %d is %s_time = %d).\n", task_id, mesaj, data->exec_time);
            exista_task = true;
        }
        free(elem);
        push_Q(q_aux, (void *)data);
        elem = pop_Q(*q);
    }
    destroyQueue(q);
    *q = q_aux;
    return exista_task;
}
void get_task(int task_id, Sched *sch)
{
    bool gasit = false;
    char mesaj[20];
    strcpy(mesaj, "running (remaining");
    gasit = gasit || get_task_general(task_id, &(sch->q_run), mesaj, sch->f_out);
    strcpy(mesaj, "waiting (remaining");
    gasit = gasit || get_task_general(task_id, &(sch->q_wait), mesaj, sch->f_out);
    strcpy(mesaj, "finished (executed");
    gasit = gasit || get_task_general(task_id, &(sch->q_fin), mesaj, sch->f_out);
    if (!gasit)
        fprintf(sch->f_out, "Task %d not found.\n", task_id);
}

void get_thread(int thread_id, Sched *sch)
{
    // verificam mai intai in stiva de thread-uri daca id-ul este idle
    // ne folosim de o stiva auxiliara
    bool gasit = false;
    Stack *stack_aux = createStack();
    nod_stv *elem = pop_S(sch->stack);
    while (elem != NULL)
    {
        Thread *data = (Thread *)(elem->data);
        if (thread_id == data->ID)
            gasit = true;
        free(elem);
        push_S((void *)data, stack_aux);
        elem = pop_S(sch->stack);
    }
    elem = pop_S(stack_aux);
    // punem elementele inapoi in stiva
    while (elem != NULL)
    {
        Thread *data = (Thread *)(elem->data);
        free(elem);
        push_S((void *)data, sch->stack);
        elem = pop_S(stack_aux);
    }
    if (gasit)
        fprintf(sch->f_out, "Thread %d is idle.\n", thread_id);
    // daca nu se gaseste in stiva, cautam in coada de running
    else
    {
        nod_Q *elem = pop_Q(sch->q_run);
        Queue *q_aux = createQueue();
        while (elem != NULL)
        {

            Task *data = (Task *)(elem->data);
            if (data->threadID == thread_id)
                fprintf(sch->f_out, "Thread %d is running task %d (remaining_time = %d).\n", thread_id, data->ID, data->exec_time);
            free(elem);
            push_Q(q_aux, (void *)data);
            elem = pop_Q(sch->q_run);
        }
        destroyQueue(&(sch->q_run));
        sch->q_run = q_aux;
    }
    destroyStack(&stack_aux);
}

void print_task_run(Task *data, FILE *f_out)
{
    // afiseaza un task run
    fprintf(f_out, "(%d: priority = %d, remaining_time = %d, running_thread = %d)", data->ID, data->priority, data->exec_time, data->threadID);
}
void print_task_wait(Task *data, FILE *f_out)
{
    // afiseaza un task wait
    fprintf(f_out, "(%d: priority = %d, remaining_time = %d)", data->ID, data->priority, data->exec_time);
}
void print_task_fin(Task *data, FILE *f_out)
{
    // afiseaza un task terminat
    fprintf(f_out, "(%d: priority = %d, executed_time = %d)", data->ID, data->priority, data->init_time);
}

void print_Q_general(Queue **q, void (*afis)(Task *, FILE *), FILE *f_out)
{
    // afiseaza mai multe task-uri
    // este folosita functia de "afis" pentru a prelua si afisa tipul de coada necesar
    nod_Q *elem = pop_Q(*q);
    Queue *q_aux = createQueue();
    fprintf(f_out, "[");
    while (elem != NULL)
    {

        Task *data = (Task *)(elem->data);
        (*afis)(data, f_out);
        free(elem);
        push_Q(q_aux, (void *)data);
        elem = pop_Q(*q);
        if (elem != NULL)
            fprintf(f_out, ",\n");
    }
    fprintf(f_out, "]\n");
    destroyQueue(q);
    *q = q_aux;
}

void print_Q(Sched *sch, char *tip)
{
    // este verificat tipul de coada ce trebuie afisat
    if (strcmp(tip, "waiting") == 0)
    {
        fprintf(sch->f_out, "====== Waiting queue =======\n");
        print_Q_general(&(sch->q_wait), &print_task_wait, sch->f_out);
    }
    else if (strcmp(tip, "running") == 0)
    {
        fprintf(sch->f_out, "====== Running in parallel =======\n");
        print_Q_general(&(sch->q_run), &print_task_run, sch->f_out);
    }
    else if (strcmp(tip, "finished") == 0)
    {
        fprintf(sch->f_out, "====== Finished queue =======\n");
        print_Q_general(&(sch->q_fin), &print_task_fin, sch->f_out);
    }
}
void move_to_run_q(Sched *sch)
{
    // sunt mutate task-uri in coada de run
    Queue *q_run_aux = createQueue();
    while (sch->stack->inc != NULL && sch->q_wait->inc != NULL)
    {
        nod_stv *elem_s = pop_S(sch->stack);
        nod_Q *elem_q = pop_Q(sch->q_wait);
        Thread *dataTh = (Thread *)(elem_s->data);
        Task *dataQ = (Task *)(elem_q->data);
        dataQ->threadID = dataTh->ID; // atribuim ID-ul de thread unui task
        push_Q(q_run_aux, (void *)dataQ);
        free(dataTh);
        free(elem_s);
        free(elem_q);
    }
    // ordonam coada de run
    sch->q_run = interclasare_cozi(q_run_aux, sch->q_run);
}
void run_tasks(Sched *sch, int time)
{
    // rularea efectiva a task-urilor
    Queue *q_run_aux = createQueue();
    nod_Q *elem = pop_Q(sch->q_run);
    int max_run_time = 0;
    while (elem != NULL)
    {
        int time_for_task = 0;
        Task *data = (Task *)(elem->data);
        // verifica daca timpul ramas de rulat se incadreaza intr-un singur ciclu
        if (data->exec_time < time)
        {
            time_for_task = data->exec_time;
            data->exec_time = 0;
        }
        else
        {
            // daca nu se incadreza scade timpul ramas de rulat
            time_for_task = time;
            data->exec_time -= time;
        }
        if (time_for_task > max_run_time)
            max_run_time = time_for_task;
        free(elem);
        push_Q(q_run_aux, (void *)data);
        elem = pop_Q(sch->q_run);
    }
    destroyQueue(&(sch->q_run));
    sch->q_run = q_run_aux;
    // este calculat timpul total pentru terminarea task-urilr
    sch->T_total += max_run_time;
}

void move_to_fin_q(Sched *sch)
{
    Queue *q_run_aux = createQueue();
    nod_Q *elem = pop_Q(sch->q_run);
    // sunt mutate din coada de run in coada de finish task-urile terminate
    while (elem != NULL)
    {

        Task *data = (Task *)(elem->data);
        if (data->exec_time == 0)
        {
            data->exec_time = data->init_time;
            push_Q(sch->q_fin, (void *)data);
            // este eliberat un nou thread cu ID
            Thread *tmp = malloc(sizeof(Thread));
            tmp->ID = data->threadID;
            push_S((void *)tmp, sch->stack);
            // este setat ca fiid disponibil si poate sa preia task-uri
            sch->available_task_id[data->ID] = 0;
        }
        else
            push_Q(q_run_aux, (void *)data);
        free(elem);
        elem = pop_Q(sch->q_run);
    }
    destroyQueue(&(sch->q_run));
    sch->q_run = q_run_aux;
}
void run_cycle(Sched *sch, int time)
{
    // implementarea unui singure rulari
    move_to_run_q(sch);
    run_tasks(sch, time);
    move_to_fin_q(sch);
}
void run(Sched *sch, int T)
{
    while (T > 0)
    {
        // rularea nu a fost terminata
        if (T < sch->Q)
        {
            run_cycle(sch, T);
            T = 0;
        }
        else
        {
            run_cycle(sch, sch->Q);
            T -= sch->Q;
        }
    }
}
void finish(Sched *sch)
{
    while ((sch->q_run->inc != NULL) || (sch->q_wait->inc != NULL))
    {
        run(sch, sch->Q);
    }
    fprintf(sch->f_out, "Total time: %d", sch->T_total);
}