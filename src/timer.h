#ifndef TIMER_H
#define TIMER_H

#include "common.h"

void init_timer(u32int frequency);
void set_timer_periodic(u32int period);
u32int get_current_time();

#endif