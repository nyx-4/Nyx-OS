#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

void console_initialize(void);
void console_putchar(char chr);
void console_write(const char *data, size_t size);
void console_writestring(const char *str);

#endif
