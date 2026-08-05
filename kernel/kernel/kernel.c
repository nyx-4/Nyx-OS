#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
    printf("Hello, \e[31mkernel \e[91;44mWorld\e[m!\n");
}
