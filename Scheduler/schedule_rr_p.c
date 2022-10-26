#include <stdlib.h>
#include <stdio.h>
#include "schedulers.h"
#include "list.h"
#include "CPU.h"

int contTid = 0;
struct node **list[5];

// Add a new task to the queue
void add(char *name, int priority, int burst) {
    if(contTid == 0) {
        for(int i = 0; i < 5; ++i) {
            list[i] = malloc(sizeof(struct node));
        }
    }

    struct task *newTask = malloc(sizeof(struct task));

    newTask->name = name;
    newTask->tid = ++contTid;
    newTask->priority = priority;
    newTask->burst = burst;
    insertOnEnd(list[newTask->priority-1], newTask);
}

// Get the next element in the queue
Task * getNextTask() {
    Task *next = NULL;

    for(int i = 4; i >= 0; --i) {
        if(*list[i] != NULL) {
            next = (*list[i])->task;
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
            delete(list[current->priority - 1], current);
            continue;
        }

        insertOnEnd(list[current->priority-1], current);
        delete(list[current->priority-1], current);
    }
}