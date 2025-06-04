#include <iostream>
#include <kernel/framebuffer.hpp>

extern "C" void kernel_main(void) {
    FBuf fb;
    fb.terminal_initialize();
    string s = "Hello";
    if (s == "Hello") fb.terminal_writestring("Hello, World\033[0;4H, ");
}
