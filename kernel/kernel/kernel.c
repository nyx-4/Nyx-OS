#include <stdio.h>

#include <kernel/tty.h>

__attribute__((constructor)) void foo(void) {
    printf("foo is running and printf is available at this point\n");
}

void kernel_main(void) {
    printf("Hello, \e[31mkernel \e[91;44mWorld\e[m!\n");
}
