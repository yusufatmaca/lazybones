/* 
It includes functions for writing to and reading from the I/O bus, and some typedefs that will make it easier for us to write portable code
*/

#ifndef COMMON_H
#define COMMON_H

typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

#define U32INT_MAX  0xFFFFFFFF

/*
Think of your computer's I/O ports as specific mailbox slots (port numbers) that different hardware components (like the keyboard, screen, etc.) use to send and receive messages (data). The outb instruction is like putting a letter (data) into a specific mailbox slot (port) to communicate with a specific hardware device.
*/
void outb(u16int port, u8int value);

/*
This function reads a byte (8 bits) from the specified I/O port.
*/
u8int inb(u16int port);

/*
This function reads a word (16 bits) from the specified I/O port.
*/
u16int inw(u16int port);

/*
These functions (`inb`, and `inw`) provide a simple and direct way to interact with hardware devices by reading data from their I/O ports
*/
char* strcpy(char *dest, const char *src);
u8int strcmp(char *str1, char *str2);
char* strtok(char *str, const char *delim);
u8int strlen(const char *str);
int atoi(char* str);
u8int validate_time_format(char *time);
void memset(u8int *dest, u8int val, u32int len);
void* malloc(u32int size);
void free(void* ptr);
#endif