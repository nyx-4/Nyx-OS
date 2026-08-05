#ifndef ARCH_I386_IOPORT_H
#define ARCH_I386_IOPORT_H

#include <stdint.h>

__attribute__((unused)) static inline uint8_t outport8(uint16_t port,
                                                       uint8_t  value) {
    asm volatile("outb %1, %0" : : "dN"(port), "a"(value));
    return value;
}

__attribute__((unused)) static inline uint8_t inport8(uint16_t port) {
    uint8_t result;
    asm volatile("inb %1, %0" : "=a"(result) : "dN"(port));
    return result;
}

#endif
