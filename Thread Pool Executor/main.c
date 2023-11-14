/* CALUGARU Matei - 314CB */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stiva.h"
#include "coada.h"
#include "scheduler.h"

int main(int argc, char *argv[])
{
    FILE *f_in;
    FILE *f_out;
    char line[256];
    if (argc == 3)
    {
        f_in = fopen(argv[1], "rt");
        f_out = fopen(argv[2], "w");
    }
    else
    {
        f_in = fopen("in_file", "rt");
        f_out = fopen("out_file", "w");
    }
    fgets(line, sizeof(line), f_in);
    int Q = atoi(line);
    fgets(line, sizeof(line), f_in);
    int C = atoi(line);
    Sched *sch = createSched(Q, C, f_out);
    while (fgets(line, sizeof(line), f_in))
    {
        char *cuvant = strtok(line, " \n");
        if (strcmp(cuvant, "add_tasks") == 0)
        {
            char *nr_1 = strtok(NULL, " \n");
            int nr_of_tasks = atoi(nr_1);
            char *nr_2 = strtok(NULL, " \n");
            int exec_time = atoi(nr_2);
            char *nr_3 = strtok(NULL, " \n");
            int priority = atoi(nr_3);
            ins_Q_wait(nr_of_tasks, exec_time, priority, sch);
        }
        else if (strcmp(cuvant, "get_task") == 0)
        {
            char *task = strtok(NULL, " \n");
            int task_id = atoi(task);
            get_task(task_id, sch);
        }
        else if (strcmp(cuvant, "get_thread") == 0)
        {
            char *thread = strtok(NULL, " \n");
            int thread_id = atoi(thread);
            get_thread(thread_id, sch);
        }
        else if (strcmp(cuvant, "print") == 0)
        {
            char *tip_print = strtok(NULL, " \n");
            print_Q(sch, tip_print);
        }
        else if (strcmp(cuvant, "run") == 0)
        {
            char *nr_unit_timp = strtok(NULL, " \n");
            int T = atoi(nr_unit_timp);
            fprintf(f_out,"Running tasks for %d ms...\n", T);
            run(sch, T);

        }
        else if (strcmp(cuvant, "finish") == 0)
            finish(sch);
    }
    destroySched(&sch);
    fclose(f_out);
    fclose(f_in);
    return 0;
}