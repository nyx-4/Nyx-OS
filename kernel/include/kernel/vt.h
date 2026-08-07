#ifndef _KERNEL_VT_H
#define _KERNEL_VT_H

#include <math.h>
#include <stdint.h>

#include <kernel/tty.h>

enum ansi_state : uint8_t {
    ANSI_STATE_NONE,
    ANSI_STATE_CSI,
    ANSI_STATE_COMMAND,
};

#define ANSI_PARAM_MAX 16

// A vt (virtual terminal) implements a tty connected to a screen.
//
// It handles escape code processing and allows efficient re-renders using an
// arbitrary screen driver.
struct vt {
    struct tty      base;
    unsigned short  row;
    unsigned short  column;
    unsigned short  width;
    unsigned short  height;
    uint8_t         vgacolor;
    uint16_t        attr;
    uint32_t        fgcolor;
    uint32_t        bgcolor;
    struct screen  *screen;
    enum ansi_state ansi_state;
    unsigned int    ansi_params[ANSI_PARAM_MAX];
    size_t          ansi_count;
};

extern const struct tty_ops vt_ops;

struct tty *vt_init(struct vt *self, struct screen *screen);

#endif
