#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/kernel.h>
#include <kernel/tty.h>

#include "ioport.h"
#include "ttyS.h"

// Check whether the serial port is ready to transmit another byte.
static bool serial_can_transmit(uint16_t port) {
    return (inport8(port + 5) & 0x20) != 0;
}

// Transmit a byte on a serial port when it becomes possible to transmit.
void serial_write(uint16_t port, unsigned char byte) {
    while (!serial_can_transmit(port)) { }  // wait
    outport8(port, byte);
}

// Initialize a ttyS object connected to a console driver.
struct tty *ttyS_init(struct ttyS *self, uint16_t port, unsigned short width,
                      unsigned short height) {
    memset(self, 0, sizeof(*self));
    self->base.ops = &ttyS_ops;
    self->port     = port;
    self->width    = width;
    self->height   = height;
    // Real hardware may require real initialization of settings such as baud.
    return &self->base;
}

// Process the next output byte.
static bool ttyS_transmit(struct tty *base, unsigned char chr) {
    struct ttyS *self = container_of(base, struct ttyS, base);
    serial_write(self->port, chr);
    return true;
}

// Get the terminal window size.
static int ttyS_tcgetwinsize(struct tty *base, struct winsize *ws) {
    struct ttyS *self = container_of(base, struct ttyS, base);
    ws->ws_col        = self->width;
    ws->ws_row        = self->height;
    return 0;
}

const struct tty_ops ttyS_ops = {
    .transmit     = ttyS_transmit,
    .tcgetwinsize = ttyS_tcgetwinsize,
};
