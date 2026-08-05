#ifndef _KERNEL_FB_H
#define _KERNEL_FB_H

#include <stdint.h>

#include <kernel/screen.h>

struct fb {
    struct screen       base;
    uint64_t            addr;
    uint32_t            width;
    uint32_t            height;
    uint32_t            pitch;
    uint32_t            bpp;
    uint32_t           *buffer;
    unsigned short      columns;
    unsigned short      rows;
    struct screen_char *grid;
};

extern const struct screen_ops fb_ops;

struct screen *fb_init(struct fb *self, uint64_t addr, uint32_t width,
                       uint32_t height, uint32_t pitch, uint32_t bpp);

#endif
