#ifndef ARCH_I386_TTYS_H
#define ARCH_I386_TTYS_H

#include <stdint.h>

#include <kernel/tty.h>

struct ttyS {
    struct tty     base;
    uint16_t       port;
    unsigned short width;
    unsigned short height;
};

extern const struct tty_ops ttyS_ops;

void serial_write(uint16_t port, unsigned char byte);

struct tty *ttyS_init(struct ttyS *self, uint16_t port, unsigned short width,
                      unsigned short height);

#endif
