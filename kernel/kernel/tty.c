#include <kernel/tty.h>

// The /dev/console kernel console.
struct tty *dev_console;

size_t tty_write(struct tty *self, const void *buf, size_t len) {
    for (size_t i = 0; i < len; i++) {
        unsigned char chr = ((unsigned char *)buf)[i];
        if (chr == '\n') {
            self->ops->transmit(self, '\r');
            self->ops->transmit(self, '\n');
        } else {
            self->ops->transmit(self, chr);
        }
    }
    return len;
}

size_t console_write(const void *data, size_t size) {
    return tty_write(dev_console, data, size);
}

int console_tcgetwinsize(struct winsize *ws) {
    return dev_console->ops->tcgetwinsize(dev_console, ws);
}
