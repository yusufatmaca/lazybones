#include "cmos.h"
#include "common.h"

#define CMOS_ADDRESS 0x70
#define CMOS_DATA 0x71

u8int read_cmos(u8int reg) {
    outb(CMOS_ADDRESS, reg);
    return inb(CMOS_DATA);
}

u8int write_cmos(u8int reg, u8int val) {
    outb(CMOS_ADDRESS, reg);
    outb(CMOS_DATA, val);
}

static u8int bcd_to_binary(u8int bcd) {
    return ((bcd / 16) * 10) + (bcd & 0x0F);
}

static u8int bin_to_bcd(u8int val) {
    return ((val / 10) << 4) | (val % 10);
}

void read_rtc(u8int* hour, u8int* minute, u8int* second) {
    // Wait for update to complete
    outb(CMOS_ADDRESS, 0x0A);
    while (inb(CMOS_DATA) & 0x80);

    // Read time
    *hour = read_cmos(0x04);
    *minute = read_cmos(0x02);
    *second = read_cmos(0x00);

    // Convert BCD to binary if needed
    outb(CMOS_ADDRESS, 0x0B);
    u8int status_b = inb(CMOS_DATA);
    if (!(status_b & 0x04)) {
        *hour = bcd_to_binary(*hour);
        *minute = bcd_to_binary(*minute);
        *second = bcd_to_binary(*second);
    }
}

void write_rtc(u8int* hour, u8int minute, u8int* second) {
    outb(CMOS_ADDRESS, 0x0A);
    while (inb(CMOS_DATA) & 0x80);

    write_cmos(0x00, bin_to_bcd(second));
    write_cmos(0x02, bin_to_bcd(minute));
    write_cmos(0x04, bin_to_bcd(hour));
}