#include <iostream>
#include <kernel/framebuffer.hpp>

extern "C" void kernel_main(void) {
    FBuf::terminal_initialize();
    std::ostream cout;
    cout << "Helllo, this is me";
    // FBuf::test(1, 0);
    // std::string s = "Hello";
    // if (s == "Hello") FBuf::terminal_writestring("Hello, World\033[0;4H, ");
    __asm__ __volatile__("movl $0, %eax\n int $0x80\n");
    __asm__ __volatile__("movl $1, %eax\n int $0x80\n");
    while (1);
    // __asm__ __volatile__("movl $2, %eax\n int $0x80\n");
}
