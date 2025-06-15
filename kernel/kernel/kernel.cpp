#include <iostream>
#include <kernel/framebuffer.hpp>

extern "C" void kernel_main(void) {
    FBuf::terminal_initialize();
    std::ostream cout;
    cout << "Hello, ";
    // FBuf::test();
    std::string s = "World...\n";
    // if (s == "World...\n")
    FBuf::terminal_writestring("World...\n");
    __asm__ __volatile__("movl $0, %eax\n int $0x80\n");
    __asm__ __volatile__("movl $1, %eax\n int $0x80\n");
    __asm__ __volatile__("movl $2, %eax\n int $0x80\n");
    while (1);
}
