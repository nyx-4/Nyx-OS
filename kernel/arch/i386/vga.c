#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/kernel.h>
#include <kernel/screen.h>
#include <kernel/vga.h>

#include "vga.h"

static uint16_t *const VGA_MEMORY = (uint16_t *)0xB8000;
static const size_t    VGA_WIDTH  = 80;
static const size_t    VGA_HEIGHT = 25;

// Format a VGA text mode entry with the console character and color
static inline uint16_t vga_entry_from_sc(const struct screen_char *s_chr) {
    return vga_entry(s_chr->chr, s_chr->vgacolor);
}

// Initialize a VGA console
struct screen *vga_init(struct vga *self) {
    memset(self, 0, sizeof(*self));
    self->base.ops = &vga_ops;
    self->memory   = VGA_MEMORY;
    self->width    = VGA_WIDTH;
    self->height   = VGA_HEIGHT;
    return &self->base;
}

// get window's size
static void vga_tcgetwinsize(struct screen *base, struct winsize *ws) {
    struct vga *self = container_of(base, struct vga, base);
    ws->ws_col       = self->width;
    ws->ws_row       = self->height;
}

// Clear the console
static void vga_clear(struct screen *base, const struct screen_char *fill) {
    struct vga *self  = container_of(base, struct vga, base);
    uint16_t    entry = vga_entry_from_sc(fill);

    for (unsigned short p_y = 0; p_y < self->height; p_y++) {
        for (unsigned short p_x = 0; p_x < self->width; p_x++) {
            size_t idx        = (p_y * self->width) + p_x;
            self->memory[idx] = entry;
        }
    }
}

// Set a character on the console
static void vga_set_char(struct screen *base, const struct screen_char *s_chr,
                         unsigned short p_x, unsigned short p_y) {
    struct vga *self  = container_of(base, struct vga, base);
    size_t      idx   = (p_y * self->width) + p_x;
    uint16_t    entry = vga_entry_from_sc(s_chr);
    self->memory[idx] = entry;
}

// Scroll the console one line
static void vga_scroll(struct screen *base, const struct screen_char *fill) {
    struct vga *self = container_of(base, struct vga, base);

    // moves everything one line up
    for (unsigned short p_y = 0; p_y < self->height - 1; p_y++) {
        for (unsigned short p_x = 0; p_x < self->width; p_x++) {
            size_t dst        = (p_y * self->width) + p_x;
            size_t src        = ((p_y + 1) * self->width) + p_x;
            self->memory[dst] = self->memory[src];
        }
    }

    // then fill last line with `fill`
    uint16_t entry = vga_entry_from_sc(fill);

    for (unsigned short p_x = 0; p_x < self->width; p_x++) {
        size_t last_line  = self->height - 1;
        size_t idx        = (last_line * self->width) + p_x;
        self->memory[idx] = entry;
    }
}

const struct screen_ops vga_ops = {
    .tcgetwinsize = vga_tcgetwinsize,
    .clear        = vga_clear,
    .set_char     = vga_set_char,
    .scroll       = vga_scroll,
};
