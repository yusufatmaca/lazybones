// cmos.h
#ifndef CMOS_H
#define CMOS_H

#include "common.h"

// CMOS ports
#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

// CMOS registers for time
#define CMOS_SECOND 0x00
#define CMOS_MINUTE 0x02
#define CMOS_HOUR 0x04

u8int read_cmos(u8int reg);
void read_rtc(u8int* hour, u8int* minute, u8int* second);
void write_rtc(u8int* hour, u8int minute, u8int* second);
void clear_cmos();

#endif