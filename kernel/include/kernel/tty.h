#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <abi/winsize.h>

// Base structure for the tty interface.
struct tty {
    const struct tty_ops *ops;
};

// A tty is a terminal implemented by a driver.
//
// Terminals including the tty_console, which graphically renders to a local
// console framebuffer, but can also be a serial driver that communicates with
// a hardware device subject to line discipline, or a pty pseudoterminal
// entirely in software.
struct tty_ops {
    bool (*transmit)(struct tty *self, unsigned char chr);
    int (*tcgetwinsize)(struct tty *self, struct winsize *ws);
};

size_t tty_write(struct tty *self, const void *buf, size_t len);

extern struct tty *dev_console;

size_t console_write(const void *data, size_t size);
int console_tcgetwinsize(struct winsize *ws);

#endif
