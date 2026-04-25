# Lazybones Kernel
Lazybones Kernel is a minimal, educational operating system developed within the scope of a systems programming course.

The project serves as a hands-on exploration of fundamental OS concepts, including the *boot process*, *low-level hardware interaction* (such as CMOS RTC and interrupt handling), basic *command-line interface design*, and *disk image manipulation*.

= = =

## Features

### Task Scheduling and Context Switching
The kernel includes an experimental *task management layer* intended to demonstrate *basic scheduling* and *context switching* concepts.

Tasks can be defined with attributes such as *stack size*, *period*, and *priority*, then registered through the task creation interface.

### Clock
The kernel provides a *real-time clock* that retrieves the current time from the CMOS RTC and updates it using *timer interrupts*, ensuring smooth and consistent progression without relying on *busy-waiting*. The clock is persistently displayed at the top-right corner of the screen in a zero-padded 24-hour format.

### Command Shell
A minimal *command-line shell* is implemented to allow user interaction with the system.

It supports essential commands such as `help`, `settime`, and `reboot`, while also incorporating argument validation, error handling, and basic command parsing to ensure robust and predictable behavior.

= = =

## Getting Started

### Build Kernel
```sh
make
```

### Prepare Floppy Image
Since the kernel runs on bare metal without an underlying operating system, it must be loaded by a bootable medium. In this project, a floppy disk image is used to package the kernel in a format that can be recognized and loaded by the bootloader during system startup.

See detailed instructions here:
➡️ [`scripts/README.md`](scripts/README.md)

= = =

## References
- http://wiki.osdev.org/Main_Page
- https://docs.huihoo.com/help-pc/index.html
