#include <stdlib.h>
#include <stdio.h>
#include "schedulers.h"
#include "list.h"
#include "CPU.h"

int contTid = 0;
struct node **list;

void add(char *name, int priority, int burst) {
    if(contTid == 0) {
        list = malloc(sizeof(struct node));
    }

    struct task *newTask = malloc(sizeof(struct task));

    newTask->name = name;
    newTask->tid = ++contTid;
    newTask->priority = priority;
    newTask->burst = burst;
    insertOnEnd(list, newTask);
}

void schedule() {
    struct node *atual = *list;

    while(atual != NULL) {
        run(atual->task, QUANTUM);
        atual->task->burst -= QUANTUM;
        if(atual->task->burst <= 0) {
            delete(list, atual->task);
        } else {
            insertOnEnd(list, atual->task);
            delete(list, atual->task);
        }
        atual = atual->next;
    }
}