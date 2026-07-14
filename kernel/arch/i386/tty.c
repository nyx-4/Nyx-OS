#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"

static const size_t    VGA_WIDTH  = 80;
static const size_t    VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *)0xB8000;

static size_t    terminal_row;
static size_t    terminal_col;
static uint8_t   terminal_color;
static uint16_t *terminal_buffer;

void terminal_initialize() {
    terminal_row    = 0;
    terminal_col    = 0;
    terminal_color  = vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_WHITE);
    terminal_buffer = VGA_MEMORY;

    // empty terminal_buffer (i.e., clear screen)
    for (size_t row = 0; row < VGA_HEIGHT; row++) {
        for (size_t col = 0; col < VGA_WIDTH; col++) {
            const size_t INDEX     = (row * VGA_WIDTH) + col;
            terminal_buffer[INDEX] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

void terminal_putentryat(unsigned char chr, uint8_t color, size_t col,
                         size_t row) {
    const size_t INDEX     = (row * VGA_WIDTH) + col;
    terminal_buffer[INDEX] = vga_entry(chr, color);
}

void terminal_putchar(char chr) {
    terminal_putentryat(chr, terminal_color, terminal_col, terminal_row);
    if (++terminal_col == VGA_WIDTH) {  // at end of line
        terminal_col = 0;
        if (++terminal_row == VGA_HEIGHT) {  // at end of page
            terminal_row = 0;                // so, start again at top
        }
    }
}

void terminal_write(const char *data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char *str) {
    terminal_write(str, strlen(str));
}
