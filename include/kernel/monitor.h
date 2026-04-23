#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"


// Write a single character out to the screen.
void monitor_put(char c, u8int bC, u8int fC);

// Clear the screen to all black.
void monitor_clear();

// Output a null-terminated (`\n`) ASCII string to the monitor.
void monitor_write(char *c, u8int bC, u8int fC);

void monitor_write_dec(u32int n, u8int bC, u8int fC);

void monitor_display_clock(u8int hour, u8int minute, u8int second);

#endif
