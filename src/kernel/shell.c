// shell.c
#include "common.h"
#include "monitor.h"
#include "kb.h"
#include "cmos.h"
#include "timer.h"

void init_shell() {
    keyboard_install();
    init_timer(18);
    
    while (1) {
        char input[128];
        monitor_write("OSx383> ", 0, 3);
        gets(input);
        execute_command(input);
    }
}

void execute_command(char *input) {
    char org_input[256];
    strcpy(org_input, input);
    char *command = strtok(org_input, " ");
    char *arguments = input + strlen(command) + 1;

    if (strcmp(command, "help") == 0) {
        help_command();
    } else if (strcmp(command, "settime") == 0) {
        settime_command(arguments);
    } else if (strcmp(org_input, "reboot") == 0) {
        reboot_command();
    } else {
        invalid_command();
    }
}

void help_command() {
    monitor_write("Available commands: help, settime, reboot\n", 0, 15);
}

void settime_command(char *input) {
    if (validate_time_format(input) == 1) {
        monitor_write("Invalid time format. Use HH:MM:SS.\n", 0, 15);
        return;
    }
    u8int hours, minutes, seconds;
    parse_time(input, &hours, &minutes, &seconds);

    write_rtc(hours, minutes, seconds);
    monitor_write("Time has been set to ", 0, 15);
    monitor_write_dec(hours, 0, 15);
    monitor_write(":", 0, 15);
    monitor_write_dec(minutes, 0, 15);
    monitor_write(":", 0, 15);
    monitor_write_dec(seconds, 0, 15);
    monitor_write("\n", 0, 15);
}

void parse_time(const char *time, u8int *hours, u8int *minutes, u8int *seconds) {
    char hours_str[3] = {time[0], time[1], '\0'};
    char minutes_str[3] = {time[3], time[4], '\0'};
    char seconds_str[3] = {time[6], time[7], '\0'};
    
    *hours = (u8int) atoi(hours_str);
    *minutes = (u8int) atoi(minutes_str);
    *seconds = (u8int) atoi(seconds_str);
}

void invalid_command() {
    monitor_write("Invalid command. Type 'help' for a list of available commands.\n", 0, 15);
}

void reboot_command() {
    monitor_write("Rebooting...\n", 0, 15);
    outb(0x64, 0xFE); /* pulse CPU reset line */
}
