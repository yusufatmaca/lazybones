#ifndef TASK_H
#define TASK_H

#include "common.h"

#define MAX_TASKS 10

#define TASK_STATE_RUNNING 0
#define TASK_STATE_WAITING 1

typedef struct {
    u32int *stack_pointer;     // Stack pointer
    u32int stack_size;            // Stack size
    void (*task_func)();   // Pointer to task function
    u32int period;                // Period of the task in microseconds
    u32int priority;              // Priority of the task
    u8int state;
} task_t;

void task_create(task_t *task, void (*task_func)(void));
void scheduler();
void context_switch(task_t *current_task, task_t *next_task);




#endif