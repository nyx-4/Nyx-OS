#ifndef _KERNEL_SCREEN_H
#define _KERNEL_SCREEN_H

#include <stddef.h>
#include <stdint.h>

#include <abi/winsize.h>

// A generic screen character with attributes and colors, which can be
// rendered with the best approximation possible by the console drivers.
struct screen_char {
    unsigned char chr;
    uint8_t       vgacolor;
    uint16_t      attr;
    uint32_t      fg;
    uint32_t      bg;
};

// Base structure for the screen interface.
struct screen {
    const struct screen_ops *ops;
};

// A console provides a two-dimensional grid of characters that can be updated
// and re-rendered by drivers using these efficient functions.
// kinda like C++'s `class`
struct screen_ops {
    void (*tcgetwinsize)(struct screen *self, struct winsize *ws);
    void (*clear)(struct screen *self, const struct screen_char *fill);
    void (*set_char)(struct screen *self, const struct screen_char *s_chr,
                     unsigned short pos_x, unsigned short pos_y);
    void (*scroll)(struct screen *self, const struct screen_char *fill);
};

#endif
