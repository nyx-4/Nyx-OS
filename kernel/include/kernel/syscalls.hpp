#ifndef _KERENEL_SYSCALLS_HPP
#define _KERENEL_SYSCALLS_HPP

#include <kernel/framebuffer.hpp>
#include <stdint.h>

extern "C" const uint32_t MAX_SYSCALLS = 2;

void syscall_0();
void syscall_1();

#endif