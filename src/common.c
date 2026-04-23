#include "common.h"

// Write a byte out to the specified port.
void outb(u16int port, u8int value) {
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}
/*
    asm -> inline assembly
    volatile -> tells the compiler that this part should not be optimized
    outb -> output byte
    %1, %0 -> placeholders for the operands, which will be replaced by the actual values specified later in the code.
    : : -> The double colons separate the assembly code from the input and output operands.
    dN -> means the `port` variable will be placed in a dx register (or other port registers).
    a ->  means the `value` variable will be placed in the al register.
*/

u8int inb(u16int port) {
    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Read byte from port
u16int inw(u16int port) {
    u16int ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Copy len bytes from src to dest
void memcpy(u8int *dest, const u8int *src, u32int len)
{
    const u8int *sp = (const u8int *)src;
    u8int *dp = (u8int *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

// Fill dest to dest+len with val
void memset(u8int *dest, u8int val, u32int len) {
    u8int *temp = (u8int *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

#define MEM_POOL_SIZE 65536  // Total size of the memory pool (e.g., 64KB)
#define BLOCK_SIZE 256       // Size of each memory block (e.g., 256 bytes)
#define MAX_BLOCKS (MEM_POOL_SIZE / BLOCK_SIZE)  // Maximum number of blocks

char memory_pool[MEM_POOL_SIZE];  // Memory pool
char block_allocated[MAX_BLOCKS];  // Allocation table

// Initializes the memory allocation system
void mem_init() {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        block_allocated[i] = 0;  // Mark all blocks as free
    }
}


void* malloc(u32int size) {
    if (size == 0) {
        return '\0';
    }

    int num_blocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;  // Calculate number of blocks needed, rounded up
    int free_blocks = 0, start_block = -1;

    // Find a contiguous area of free blocks that fits the requested size
    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (block_allocated[i] == 0) {
            if (start_block == -1) {
                start_block = i;  // Start of a new free area
            }
            if (++free_blocks == num_blocks) {
                // Enough blocks found
                for (int j = start_block; j < start_block + num_blocks; j++) {
                    block_allocated[j] = 1;  // Mark blocks as allocated
                }
                return &memory_pool[start_block * BLOCK_SIZE];
            }
        } else {
            // Reset the search
            free_blocks = 0;
            start_block = -1;
        }
    }

    return '\0';  // Not enough memory
}

void free(void* ptr) {
    if (!ptr) {
        return;
    }

    int block_index = ((char*) ptr - memory_pool) / BLOCK_SIZE;
    // Assume single block deallocation for simplicity
    if (block_index >= 0 && block_index < MAX_BLOCKS) {
        block_allocated[block_index] = 0;  // Mark block as free
    }
}

// Compare two strings
u8int strcmp(char *str1, char *str2) {
    u8int i = 0;
    u8int failed = 0;
    while(str1[i] != '\0' && str2[i] != '\0') {
        if(str1[i] != str2[i]) {
            failed = 1;
            break;
        }
        i++;
}
    if((str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
        failed = 1;

    return failed;
}

// Copy src to dest
char* strcpy(char *dest, const char *src) {
    char *ret = dest;
    while ((*dest++ = *src++));
    return ret;
}

// Concatenate dest with src
char *strcat(char *dest, const char *src) {
    while (*dest != 0)
    {
        *dest = *dest++;
    }

    do {
        *dest++ = *src++;
    } while (*src != 0);
    return dest;
}

// Function to find the first occurrence of a character in a string
char *strchr(const char *str, int c) {
    while (*str != '\0') {
        if (*str == (char)c) {
            return (char *)str;
        }
        str++;
    }
    return 0; // Return 0 instead of NULL
}

// Tokenizer function
char *strtok(char *str, const char *delim) {
    static char *last = '\0';

    // If str is NULL, continue from the last position
    if (str == '\0') {
        str = last;
    }

    // Return NULL if str is NULL or if we reached the end of the string
    if (str == '\0') {
        return '\0';
    }

    // Skip leading delimiters
    while (*str != '\0' && strchr(delim, *str) != '\0') {
        str++;
    }

    // If we reached the end of the string, return NULL
    if (*str == '\0') {
        last = '\0';
        return '\0';
    }

    // Save the start of the token
    char *token_start = str;

    // Find the end of the token
    while (*str != '\0' && strchr(delim, *str) == '\0') {
        str++;
    }

    // If we reached the end of the string, set last to NULL
    if (*str == '\0') {
        last = '\0';
    } else {
        // Null-terminate the token and update last
        *str = '\0';
        last = str + 1;
    }

    return token_start;
}

u8int strlen(const char *str) {
    u8int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

int atoi(char* str) {
    // Initialize result
    int res = 0;
    // Iterate through all characters
    // of input string and update result
    // take ASCII character of corresponding digit and
    // subtract the code from '0' to get numerical
    // value and multiply res by 10 to shuffle
    // digits left to update running total
    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';
    // return result.
    return res;
}

u8int validate_time_format(char *time) {
    u8int failed = 1;
    if (strlen(time) != 8){  return failed; }
    if (time[2] != ':' || time[5] != ':') { return failed; }
    failed = 0;
    return failed;
}

