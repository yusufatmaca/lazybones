#include "monitor.h"
#include "common.h"

u16int *video_memory = (u16int *)0xB8000;

u8int cursor_x = 0;
u8int cursor_y = 0;


// Updates the hardware cursor.
static void move_cursor() {
    // The screen is 80 characters wide.
    u16int cursorLocation = cursor_y * 80 + cursor_x;
    outb(0x3D4, 14); // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
    outb(0x3D4, 15); // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, cursorLocation); // Send the low cursor byte.
}

static void scroll() {
    u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    u16int blank = 0x20 /* space */ | (attributeByte << 8);

    // Checks if the cursor has moved beyond the last row of the screen (row 24, as indexing starts from 0).
    if(cursor_y >= 25) {
        int i;
        for (i = 0*80; i < 24*80; i++) {
            video_memory[i] = video_memory[i+80]; // Copies the character from the position `(i+80)` (which is one row below the current position `i`) to the current position `i`.
        }

        for (i = 24*80; i < 25*80; i++) {
            video_memory[i] = blank;
        }

        cursor_y = 24; // Sets the cursor back to the last row so that new text will start at the beginning of the last row.
    }
}

void monitor_put(char c, u8int bC, u8int fC) {
    u8int attributeByte = (bC << 4) | (fC & 0x0F);
    u16int attribute = attributeByte << 8;
    u16int *location;

    u8int blankAttributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    u16int blank = 0x20 /* space */ | (blankAttributeByte << 8);

    if (c == 0x08 && cursor_x) { // Handle backspace
        cursor_x--;
        location = video_memory + (cursor_y * 80 + cursor_x);
        *location = blank;
    } else if (c == 0x09) { // Handle tab
        cursor_x = (cursor_x+8) & ~(8-1);
    } else if (c == '\r') { // Handle carriage return
        cursor_x = 0;
    } else if (c == '\n') { // Handle newline
        cursor_x = 0;
        cursor_y++;
    } else if(c >= ' ') { // Handle a printable character
        location = video_memory + (cursor_y * 80 + cursor_x);
        *location = c | attribute;
        cursor_x++;
    }
    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }

    scroll();
    move_cursor();
}

void monitor_clear() {
    u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    u16int blank = 0x20 /* space */ | (attributeByte << 8);

    int i;
    for (i = 0; i < 80*25; i++) {
        video_memory[i] = blank;
    }

    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

void monitor_write(char *c, u8int bC, u8int fC) {
    int i = 0;
    while (c[i]) {
        monitor_put(c[i++], bC, fC);
    }
}

void monitor_write_hex(u32int n, u8int bC, u8int fC) {
    s32int tmp;

    monitor_write("0x", bC, fC);

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0) {
            continue;
        }

        if (tmp >= 0xA) {
            noZeroes = 0;
            monitor_put(tmp-0xA+'a', bC, fC);
        } else {
            noZeroes = 0;
            monitor_put(tmp+'0', bC, fC);
        }
    }

    tmp = n & 0xF;
    if (tmp >= 0xA) {
        monitor_put(tmp-0xA+'a', bC, fC);
    } else {
        monitor_put(tmp+'0', bC, fC);
    }
}

void monitor_write_dec(u32int n, u8int bC, u8int fC) {
    if (n == 0) {
        monitor_put('0', bC, fC);
        return;
    }

    s32int acc = n;
    char c[32];
    int i = 0;
    while (acc > 0) {
        c[i] = '0' + acc % 10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while (i >= 0) {
        c2[i--] = c[j++];
    }
    monitor_write(c2, bC, fC);
}

void monitor_display_clock(u8int hour, u8int minute, u8int second) {
    char time[9];
    // Format time as HH:MM:SS with zero padding
    time[0] = '0' + (hour / 10);
    time[1] = '0' + (hour % 10);
    time[2] = ':';
    time[3] = '0' + (minute / 10);
    time[4] = '0' + (minute % 10);
    time[5] = ':';
    time[6] = '0' + (second / 10);
    time[7] = '0' + (second % 10);
    time[8] = '\0';

    // Print the clock at the top right corner with dark gray background and yellow foreground
    u8int row = 0;
    u8int col = 80 - 8; // Assuming 80 columns width
    for (int i = 0; i < 8; i++) {
        monitor_put_at(time[i], row, col + i, 7 /* Dark Gray */, 14 /* Yellow */);
    }
}

void monitor_put_at(char c, u8int row, u8int col, u8int bC, u8int fC) {
    u16int attribute = ((bC << 4) | (fC & 0x0F)) << 8;

    u16int *location;
    location = (u16int *)0xB8000 + (row * 80 + col);
    *location = c | attribute;
}
