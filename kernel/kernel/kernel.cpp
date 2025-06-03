#include <kernel/framebuffer.hpp>
#include <stdio.h>

extern "C" void kernel_main(void) {
    // clrscr();
    FBuf fb;
    fb.terminal_initialize();
    fb.terminal_writestring("Hello");
    // printf("Hello, kernel World!\n");
}
