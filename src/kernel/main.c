#include "descriptor_tables.h"
#include "timer.h"
#include "isr.h"
#include "shell.h"
#include "task.h"
#include "monitor.h"

/*
static void isr3_callback(registers_t regs)
{
    monitor_write("This is interrupt service routine 3 \n");

}*/

void Task1() {
    //set_timer_periodic(40000);
    //while (1) {
        monitor_write("Task1 running...", 0, 15);
    //    wait_timer_event();
    //}
}


void Task2() {
    //set_timer_periodic(60000); // 60 milliseconds
    //while (1) {
        monitor_write("Task2 running...", 0, 15);
    //    wait_timer_event();
    //}
}
/*
void Task3() {
    while (1) {
        monitor_write("Task3 running...", 0, 15);
    }
}
*/

int main(struct multiboot *mboot_ptr) {
    init_descriptor_tables();
    
    //register_interrupt_handler(3, &isr3_callback);
    // asm volatile("int $0x3");
    // asm volatile("int $0x4");

    
    monitor_clear();
    monitor_write("Welcome to OSx383 Kernel.\n\n", 0, 10);
    
/*

    task_t t1, t2, t3;

    t1.stack_size = 256;
    t1.period = 40000; 
    t1.priority = 4;
    task_create(&t1, Task1);

    t2.stack_size = 256;
    t2.period = 50000;
    t2.priority = 3;
    task_create(&t2, Task2);


    t3.stack_size = 256;
    t3.period = 0; // aperiodic task
    t3.priority = 2;
    task_create(&t3, Task3);
    
    scheduler();
    

*/

    asm volatile("sti");    
    init_shell();

    

    return 0xDEADBABA;
}
