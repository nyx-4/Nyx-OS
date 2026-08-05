#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/fb.h>
#include <kernel/kernel.h>
#include <kernel/screen.h>

// upper limit on screen size, due to unavailability of dynamic memory
// allocation
#define MAX_COLUMNS 256
#define MAX_ROWS    128

#include "vgafont.h"  // TODO: read font from psf file

struct screen *fb_init(struct fb *self, uint64_t addr, uint32_t width,
                       uint32_t height, uint32_t pitch, uint32_t bpp) {
    memset(self, 0, sizeof(*self));
    self->base.ops = &fb_ops;
    self->addr     = addr;
    self->width    = width;
    self->height   = height;
    self->pitch    = pitch;
    self->bpp      = bpp;
    // Paging is not implemented. addr is a 64-bit physical address. For now,
    // simply truncate it to pointer size and reinterpret it as a pointer.
    self->buffer = (uint32_t *)(uintptr_t)addr;
    // upper limit maximum columns and rows
    self->columns = min(width / FONT_WIDTH, MAX_COLUMNS);
    self->rows    = min(height / FONT_HEIGHT, MAX_ROWS);

    static struct screen_char static_grid[MAX_COLUMNS * MAX_ROWS];
    self->grid = static_grid;
    return &self->base;
}

static void fb_render(struct fb *self, unsigned short col, unsigned short row) {
    const struct screen_char *s_chr = &self->grid[(row * MAX_COLUMNS) + col];
    const uint8_t *charfont = vgafont + (ptrdiff_t)(s_chr->chr * FONT_CHARSIZE);

    for (size_t p_y = 0; p_y < FONT_HEIGHT; p_y++) {
        // TODO: Black magic here; can't understand the logic
        size_t    buffer_pitch = self->pitch / sizeof(self->buffer[0]);
        uint32_t *data         = self->buffer +
                                 (buffer_pitch * ((FONT_HEIGHT * row) + p_y)) +
                                 (col * FONT_WIDTH);

        uint8_t line_bitmap = charfont[p_y];

        for (size_t p_x = 0; p_x < FONT_REALWIDTH; p_x++) {
            data[p_x] = line_bitmap & 1U << (7 - p_x) ? s_chr->fg : s_chr->bg;
        }

        const unsigned char BOX_START = 0xB0;
        const unsigned char BOX_END   = 0xDF;
        int is_box_chr = BOX_START <= s_chr->chr && s_chr->chr <= BOX_END;

        // Repeat the 8th column as the 9th column, except for the
        // Box-drawing characters in Code Page 437.
        uint32_t last_color = s_chr->bg;

        if (is_box_chr && (line_bitmap & 1)) {
            last_color = s_chr->fg;
        }

        data[FONT_REALWIDTH] = last_color;
    }
}

static void fb_tcgetwinsize(struct screen *base, struct winsize *ws) {
    struct fb *self = container_of(base, struct fb, base);
    ws->ws_col      = self->columns;
    ws->ws_row      = self->rows;
}

static void fb_clear(struct screen *base, const struct screen_char *fill) {
    struct fb *self = container_of(base, struct fb, base);
    memset(self->buffer, 0, self->pitch * self->height);
    for (size_t p_y = 0; p_y < self->rows; p_y++) {
        for (size_t p_x = 0; p_x < self->columns; p_x++) {
            self->grid[(p_y * MAX_COLUMNS) + p_x] = *fill;
            fb_render(self, p_x, p_y);
        }
    }
}

static void fb_set_char(struct screen *base, const struct screen_char *s_chr,
                        unsigned short col, unsigned short row) {
    struct fb *self                       = container_of(base, struct fb, base);
    self->grid[(row * MAX_COLUMNS) + col] = *s_chr;
    fb_render(self, col, row);
}

static void fb_scroll(struct screen *base, const struct screen_char *fill) {
    struct fb *self = container_of(base, struct fb, base);

    // moves everything one line up
    for (size_t p_y = 0; p_y < self->rows - 1; p_y++) {
        for (size_t p_x = 0; p_x < self->columns; p_x++) {
            size_t dst      = (p_y * MAX_COLUMNS) + p_x;
            size_t src      = ((p_y + 1) * MAX_COLUMNS) + p_x;
            self->grid[dst] = self->grid[src];
            fb_render(self, p_x, p_y);
        }
    }

    for (size_t p_x = 0; p_x < self->width; p_x++) {
        size_t last_line = self->rows - 1;
        size_t idx       = (last_line * MAX_COLUMNS) + p_x;
        self->grid[idx]  = *fill;
        fb_render(self, p_x, last_line);
    }
}

const struct screen_ops fb_ops = {
    .tcgetwinsize = fb_tcgetwinsize,
    .clear        = fb_clear,
    .set_char     = fb_set_char,
    .scroll       = fb_scroll,
};
