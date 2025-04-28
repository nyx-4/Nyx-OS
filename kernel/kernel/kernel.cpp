#include <iostream>
#include <kernel/tty.h>
#include <stdio.h>
#include <string.h>

extern "C" void kernel_main(void) {
    terminal_initialize();
    printf("Hello, kernel World!\n");
}
