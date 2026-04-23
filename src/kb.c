#include "common.h"
#include "isr.h"
#include "monitor.h"

#define LEFT_SHIFT 0x2A
#define RIGHT_SHIFT 0x36
#define LEFT_SHIFT_RELEASE 0xAA
#define RIGHT_SHIFT_RELEASE 0xB6

/* KBDUS means US Keyboard Layout. This is a scancode table
 * used to layout a standard US keyboard. */
unsigned char kbdus[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b', /* Backspace */
    '\t', /* Tab */
    'q', 'w', 'e', 'r', /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0, /* 29 - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
    '\'', '`', 0, /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', /* 49 */
    'm', ',', '.', '/', 0, /* Right shift */
    '*',
    0, /* Alt */
    ' ', /* Space bar */
    0, /* Caps lock */
    0, /* 59 - F1 key ... > */
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, /* < ... F10 */
    0, /* 69 - Num lock*/
    0, /* Scroll Lock */
    0, /* Home key */
    0, /* Up arrow */
    0, /* Page up */
    '-',
    0, /* Left arrow */
    0,
    0, /* Right arrow */
    '+',
    0, /* 79 - End key*/
    0, /* Down arrow */
    0, /* Page down */
    0, /* Insert key */
    0, /* Delete key */
    0, 0, 0,
    0, /* F11 key */
    0, /* F12 key */
    0, /* All other keys are undefined */
};

unsigned char kbdus_shift[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', /* 9 */
    '(', ')', '_', '+', '\b', /* Backspace */
    '\t', /* Tab */
    'Q', 'W', 'E', 'R', /* 19 */
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', /* Enter key */
    0, /* 29 - Control */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
    '"', '~', 0, /* Left shift */
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', /* 49 */
    'M', '<', '>', '?', 0, /* Right shift */
    '*',
    0, /* Alt */
    ' ', /* Space bar */
    0, /* Caps lock */
    0, /* 59 - F1 key ... > */
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, /* < ... F10 */
    0, /* 69 - Num lock*/
    0, /* Scroll Lock */
    0, /* Home key */
    0, /* Up arrow */
    0, /* Page up */
    '-',
    0, /* Left arrow */
    0,
    0, /* Right arrow */
    '+',
    0, /* 79 - End key*/
    0, /* Down arrow */
    0, /* Page down */
    0, /* Insert key */
    0, /* Delete key */
    0, 0, 0,
    0, /* F11 key */
    0, /* F12 key */
    0, /* All other keys are undefined */
};

static char buffer[256];
static int position = 0;
static int ready = 0;
static int shift_pressed = 0;

/* Handles the keyboard interrupt */
static void keyboard_handler(registers_t r) {
    unsigned char scancode;

    /* Read from the keyboard's data buffer */
    scancode = inb(0x60);

    /* If the top bit of the byte we read from the keyboard is set,
     * that means that a key has just been released */
    if (scancode & 0x80) {
        // Key release
        scancode &= 0x7F; // Mask out the top bit
        if (scancode == LEFT_SHIFT || scancode == RIGHT_SHIFT) {
            shift_pressed = 0;
        }
    } else {
        // Key press
        if (scancode == LEFT_SHIFT || scancode == RIGHT_SHIFT) {
            shift_pressed = 1;
        } else if (scancode == 0x0E) { // Backspace
            if (position > 0) {
                position--;
                monitor_put(kbdus[scancode], 0, 15);
        }
        } else if (kbdus[scancode] == '\n') { // Enter key
            buffer[position] = '\0';
            position = 0;
            ready = 1;
            monitor_write("\n", 0, 15);
        } else {
            if (shift_pressed) {
                buffer[position++] = kbdus_shift[scancode];
                monitor_put(kbdus_shift[scancode], 0, 15);
            } else {
                buffer[position++] = kbdus[scancode];
                monitor_put(kbdus[scancode], 0, 15);
            }
        }
    }
}

void gets(char *input) {
    while (!ready);
    strcpy(input, buffer);
    position = 0;
    ready = 0;
}

/* Installs the keyboard handler into IRQ1 */
void keyboard_install() {
    register_interrupt_handler(IRQ1, keyboard_handler);
}
