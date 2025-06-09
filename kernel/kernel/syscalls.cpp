#include <kernel/syscalls.hpp>

extern "C" void *syscalls[MAX_SYSCALLS] = {
    (void *)syscall_0,
    (void *)syscall_1,
};

void syscall_0() { FBuf::terminal_writestring("SYSCALL 0\n"); }
void syscall_1() { FBuf::terminal_writestring("SYSCALL 1\n"); }
