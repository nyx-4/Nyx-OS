#ifndef ARCH_I386_VGA_H
#define ARCH_I386_VGA_H

#include <stdint.h>

#include <kernel/screen.h>

struct vga {
    struct screen  base;
    uint16_t      *memory;
    unsigned short width;
    unsigned short height;
};

extern const struct screen_ops vga_ops;

struct screen *vga_init(struct vga *self);

#endif
