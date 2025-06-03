#include <kernel/memory.hpp>

mem_info_t Memory::mem[5];
uint32_t   Memory::mem_len = 0;

Memory::Memory(uint32_t base_addr, uint32_t length, bool isValidMemInfo) {
    if (isValidMemInfo && mem_len < 5) {
        mem[mem_len].base_addr = base_addr;
        mem[mem_len].length    = length;
        mem_len++;
    }
}
