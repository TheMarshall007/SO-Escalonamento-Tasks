#include <stdlib.h>
#include <stdio.h>
#include "schedulers.h"
#include "list.h"
#include "CPU.h"

int contTid = 0;
struct node **filas[5];

void allocTask(Task *task) {
    insertOnEnd(filas[task->priority-1], task);
}

void add(char *name, int priority, int burst) {
    if(contTid == 0) {
        for(int i = 0; i < 5; ++i) {
            struct node **list = malloc(sizeof(struct node));
            filas[i] = list;
        }
    }

    struct task *newTask = malloc(sizeof(struct task));

    newTask->name = name;
    newTask->tid = ++contTid;
    newTask->priority = priority;
    newTask->burst = burst;
    insertOnEnd(filas[newTask->priority-1], newTask);
}

Task * getNextTask() {
    Task *next = NULL;

    for(int i = 4; i >= 0; --i) {
        if(*filas[i] != NULL) {
            next = (*filas[i])->task;
            break;
        }
    }
    return next;
}

void schedule() {
    Task *current;
    while( (current = getNextTask()) ) {
        run(current, QUANTUM);
        current->burst -= QUANTUM;

        if(current->burst <= 0) {
            delete(filas[current->priority - 1], current);
            continue;
        }

        insertOnEnd(filas[current->priority-1], current);
        delete(filas[current->priority-1], current);
    }
}