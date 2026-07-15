#include <stdio.h>

#include <kernel/tty.h>

void kmain(void) {
    console_initialize();
    printf("Hello, kernel!\n");
}
