#include "timer.h"
#include "isr.h"
#include "monitor.h"
#include "cmos.h"
#include "task.h"

#define QUANTUM_MS 10 // Quantum time in milliseconds
#define FREQUENCY (1000 / QUANTUM_MS) // Timer frequency for 10 ms quantum

u32int tick = 0;
static u32int system_time = 0;

void timer_callback(registers_t regs) {
    tick++;
    system_time += QUANTUM_MS;

    if (tick % 18 == 0) {
        u8int hour, minute, second;
        read_rtc(&hour, &minute, &second);
        monitor_display_clock(hour, minute, second);
    }
    //scheduler();
}

u32int get_current_time() {
    return system_time;
}

void init_timer(u32int frequency)
{
    register_interrupt_handler(IRQ0, &timer_callback);

    u32int divisor = 1193180 / frequency;

    outb(0x43, 0x36);

    u8int l = (u8int)(divisor & 0xFF);
    u8int h = (u8int)( (divisor>>8) & 0xFF );

    outb(0x40, l);
    outb(0x40, h);
}

void set_timer_periodic(u32int period) {
    u32int frequency = 1000000 / period;
    init_timer(frequency);
}