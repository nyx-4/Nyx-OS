#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

void terminal_initialize(void);
void terminal_putchar(char chr);
void terminal_write(const char *data, size_t size);
void terminal_writestring(const char *str);

#endif
