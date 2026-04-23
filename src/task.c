#include "common.h"
#include "task.h"
#include "timer.h"
#include "monitor.h"


task_t tasks[MAX_TASKS];
u8int task_count = 0;
u8int current_task_index = -1;

void task_create(task_t *task, void (*task_func)(void)) {
    task->task_func = task_func;

    task->stack_pointer = (u32int *)malloc(task->stack_size * sizeof(u32int)) - sizeof(void*);

    *((void **)task->stack_pointer) = task_func;
    task->stack_pointer;
    
    tasks[task_count++] = *task;
}

void context_switch(task_t *current_task, task_t *next_task) {
    if (current_task != '\0') {
        // Save the current task's context
        asm volatile (
            "pusha\n"                // Push all general-purpose registers
            "movl %%esp, %0\n"       // Save ESP
            "movl %%ebp, %1\n"       // Save EBP
            : "=m" (current_task->stack_pointer), "=m" (current_task->stack_pointer[-1])
        );
    }

    if (next_task != '\0') {
        // Restore the next task's context
        asm volatile (
            "movl %0, %%esp\n"       // Restore ESP
            "movl %1, %%ebp\n"       // Restore EBP
            "popa\n"                 // Pop all general-purpose registers
            "iret\n"                  // Return to the next task
            :
            : "m" (*next_task->stack_pointer), "m" (*(next_task->stack_pointer - 1))
        );
    }
}

void wait_timer_event() {
    // Set the current task state to waiting
    // Yield control back to the scheduler
    
    // scheduler();
}
/*
void scheduler() {
    u8int next_task_index = (current_task_index + 1) % task_count;
    
    
        if (current_task_index != -1) {
        asm volatile(
            "movl %%esp, %0\n"  // Save current stack pointer
            "movl %%ebp, %1\n"  // Save base pointer
            : "=m"(tasks[current_task_index].stack_pointer)
            : // no input
            : "memory"
        );
    }

    current_task_index = next_task_index;

    asm volatile(
        "movl %0, %%esp\n"  // Load new stack pointer
        "pop %%ebp\n"       // Restore base pointer
        "ret\n"             // Return to the new task
        : // no output
        : "m"(tasks[current_task_index].stack_pointer)
        : "memory"
    );

}

*/
