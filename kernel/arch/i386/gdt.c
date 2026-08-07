#include "gdt.h"
#include <stdint.h>

uint64_t gdt[5];

uint64_t create_descriptor(uint32_t base, uint32_t limit, uint16_t flag) {
    const uint64_t HIGHER_HALF =       // Create the high 32 bit segment
        ((base >> 16) & 0x000000FF) |  // set base bits 23:16
        ((flag << 8) & 0x00F0FF00) |   // set flag fields
        (limit & 0x000F0000) |         // set limit bits 19:16
        (base & 0xFF000000);           // set base bits 31:24

    const uint64_t LOWER_HALF =  // Create the low 32 bit segment
        (base << 16) |           // set base bits 15:0
        (limit & 0x0000FFFF);    // set limit bits 15:0

    return (HIGHER_HALF << 32) | LOWER_HALF;
}

void gdt_init() {
    gdt[0] = create_descriptor(0, 0, 0);
    gdt[1] = create_descriptor(0, MAX_LIMIT, (GDT_CODE_PL0));
    gdt[2] = create_descriptor(0, MAX_LIMIT, (GDT_DATA_PL0));
    gdt[3] = create_descriptor(0, MAX_LIMIT, (GDT_CODE_PL3));
    gdt[4] = create_descriptor(0, MAX_LIMIT, (GDT_DATA_PL3));
}
