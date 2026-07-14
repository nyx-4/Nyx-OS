#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__)) void abort(void) {
#if defined(__is_libk)
    // TODO: Add proper kernel panic.
    printf("kernel: panic: abort()\n");
#else
    // TODO: Abnormally terminate the process as if by SIGABRT.
    printf("abort()\n");
#endif

    // infinite loop for now (change later)
    while (1) {
#if defined(__i386__)
        asm volatile("hlt");
#endif
    }

    __builtin_unreachable();
}
