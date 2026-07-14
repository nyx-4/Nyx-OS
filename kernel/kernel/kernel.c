#include <stdio.h>

#include <kernel/tty.h>

void kmain(void) {
    terminal_initialize();
    printf("Hello, kernel!\n");
}
