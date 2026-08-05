#include "abi/winsize.h"
#include <string.h>

#include <kernel/kernel.h>
#include <kernel/screen.h>
#include <kernel/tty.h>
#include <kernel/vga.h>
#include <kernel/vt.h>

// RGB palette for the first 16 ANSI colors.
static const uint32_t PALETTE[16] = {
    0x000000, 0xcc0000, 0x3e9a06, 0xc4a000, 0x3465a4, 0x75507b,
    0x06989a, 0xbfbfbf, 0x555753, 0xef2929, 0x8ae234, 0xfce94f,
    0x729fcf, 0xad7fa8, 0x34e2e2, 0xffffff,
};

// VGA colors for the first 8 ANSI colors. The high bit is used for bright.
static const uint8_t ANSI_TO_VGA[8] = {
    VGA_COLOR_BLACK, VGA_COLOR_RED,     VGA_COLOR_GREEN, VGA_COLOR_BROWN,
    VGA_COLOR_BLUE,  VGA_COLOR_MAGENTA, VGA_COLOR_CYAN,  VGA_COLOR_LIGHT_GREY,
};

static const uint8_t DEFAULT_VGACOLOR =
    VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4);

static const unsigned int DEFAULT_FOREGROUND = 7;
static const unsigned int DEFAULT_BACKGROUND = 0;

// Initialize a vt (virtual terminal) object connected to a screen driver.
struct tty *vt_init(struct vt *self, struct screen *screen) {
    memset(self, 0, sizeof(*self));
    self->base.ops = &vt_ops;
    self->screen   = screen;

    self->row      = 0;
    self->column   = 0;
    self->vgacolor = DEFAULT_VGACOLOR;
    self->fgcolor  = PALETTE[DEFAULT_FOREGROUND];
    self->bgcolor  = PALETTE[DEFAULT_BACKGROUND];

    // get winsize
    struct winsize ws;
    self->screen->ops->tcgetwinsize(self->screen, &ws);
    self->width  = ws.ws_col;
    self->height = ws.ws_row;

    // clear screen
    struct screen_char fill = {
        .chr      = ' ',
        .vgacolor = self->vgacolor,
        .attr     = self->attr,
        .fg       = self->fgcolor,
        .bg       = self->bgcolor,
    };
    self->screen->ops->clear(self->screen, &fill);

    return &self->base;
}

// Move to the next line and scroll if needed.
static void vt_nextline(struct vt *self) {

    if (self->row >= self->height - 1) {  // if last line, then scroll
        struct screen_char fill = {
            .chr      = ' ',
            .vgacolor = self->vgacolor,
            .attr     = self->attr,
            .fg       = self->fgcolor,
            .bg       = self->bgcolor,
        };
        self->screen->ops->scroll(self->screen, &fill);

    } else {  // else, just update row to row+1
        self->row++;
    }
}

// Execute an ANSI escape sequence command.
static void vt_run_ansi(struct vt *self, unsigned char chr) {
    switch (chr) {
    case 'm':
        if (self->ansi_count == 0) {
            self->ansi_params[0] = 0;
            self->ansi_count++;
        }
        for (size_t i = 0; i < self->ansi_count; i++) {
            unsigned int cmd = self->ansi_params[i];
            if (cmd == 0) {  // turn off all attributes
                self->vgacolor = DEFAULT_VGACOLOR;
                self->attr     = 0;
                self->fgcolor  = PALETTE[DEFAULT_FOREGROUND];
                self->bgcolor  = PALETTE[DEFAULT_BACKGROUND];

            } else if (30 <= cmd && cmd <= 37) {  // set text color
                unsigned int value = cmd - 30;
                self->fgcolor      = PALETTE[value];

                self->vgacolor &= 0xF0;  // remove fore color and
                self->vgacolor |= ANSI_TO_VGA[value] << 0;  // set new fore

            } else if (cmd == 39) {  // set default text color
                self->fgcolor = PALETTE[DEFAULT_FOREGROUND];

                self->vgacolor &= 0xF0;  // remove fore color and
                self->vgacolor |= DEFAULT_VGACOLOR & 0x0F;  // set new fore

            } else if (40 <= cmd && cmd <= 47) {  // set back color
                unsigned int value = cmd - 40;
                self->bgcolor      = PALETTE[value];

                self->vgacolor &= 0x0F;  // remove back color and
                self->vgacolor |= ANSI_TO_VGA[value] << 4;  // set new back

            } else if (cmd == 49) {  // set default back color
                self->bgcolor = PALETTE[DEFAULT_BACKGROUND];

                self->vgacolor &= 0x0F;  // remove back color and
                self->vgacolor |= DEFAULT_VGACOLOR & 0xF0;  // set new back

            } else if (90 <= cmd && cmd <= 97) {  // Set text color.
                unsigned int value = cmd - 90 + 8;
                self->fgcolor      = PALETTE[value];

                self->vgacolor &= 0xF0;
                self->vgacolor |= (0x8 | ANSI_TO_VGA[value - 8]) << 0;

            } else if (100 <= cmd && cmd <= 107) {  // Set background color.
                unsigned int value = cmd - 100 + 8;
                self->bgcolor      = PALETTE[value];

                self->vgacolor &= 0x0F;
                self->vgacolor |= (0x8 | ANSI_TO_VGA[value - 8]) << 4;
            } else {
                self->ansi_state = ANSI_STATE_NONE;
            }
        }
        break;

    default: break;  // TODO: implement other ANSI escape codes also
    }
}

// Process the next byte in an ANSI escape sequence.
static void vt_escaped(struct vt *self, unsigned char chr) {
    if (self->ansi_state == ANSI_STATE_CSI) {
        if (chr == '[') {
            self->ansi_state = ANSI_STATE_COMMAND;
        } else {
            self->ansi_state = ANSI_STATE_NONE;
        }
        return;
    }

    if (self->ansi_state == ANSI_STATE_COMMAND) {

        if ('0' <= chr && chr <= '9') {
            if (self->ansi_count == 0) {
                self->ansi_count = 1;
            }
            if (self->ansi_count <= ANSI_PARAM_MAX) {
                self->ansi_params[self->ansi_count - 1] *= 10;
                self->ansi_params[self->ansi_count - 1] += chr - '0';
            }

        } else if (chr == ';') {  // parse next parameter
            if (self->ansi_count <= ANSI_PARAM_MAX) {
                self->ansi_count++;
            } else {
                self->ansi_state = ANSI_STATE_NONE;
            }

        } else if ('@' <= chr && chr <= '~') {
            vt_run_ansi(self, chr);
            self->ansi_state = ANSI_STATE_NONE;

        } else {  // ignore unrecognized ANSI escape codes
            self->ansi_state = ANSI_STATE_NONE;
        }
    }
}

// Process the next output byte.
static bool vt_transmit(struct tty *base, unsigned char chr) {
    struct vt *self = container_of(base, struct vt, base);

    if (self->ansi_state) {
        vt_escaped(self, chr);
    } else if (chr == '\e') {
        self->ansi_state = ANSI_STATE_CSI;
        self->ansi_count = 0;
        memset(self->ansi_params, 0, sizeof(self->ansi_params));
    } else if (chr == '\n') {
        vt_nextline(self);
    } else if (chr == '\r') {
        self->column = 0;
    } else {  // otherwise render the `chr`

        if (self->column >= self->width) {  // next line if cur_line is full
            self->column = 0;
            vt_nextline(self);
        }

        struct screen_char s_chr = {
            .chr      = chr,
            .vgacolor = self->vgacolor,
            .attr     = self->attr,
            .fg       = self->fgcolor,
            .bg       = self->bgcolor,
        };

        self->screen->ops->set_char(self->screen, &s_chr, self->column,
                                    self->row);
        self->column++;
    }

    return true;
}

// Get the terminal window size.
static int vt_tcgetwinsize(struct tty *base, struct winsize *ws) {
    struct vt *self = container_of(base, struct vt, base);
    self->screen->ops->tcgetwinsize(self->screen, ws);
    return 0;
}

const struct tty_ops vt_ops = {
    .transmit     = vt_transmit,
    .tcgetwinsize = vt_tcgetwinsize,
};
