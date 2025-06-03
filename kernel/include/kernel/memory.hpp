#ifndef _KERNEL_MEMORY_HPP
#define _KERNEL_MEMORY_HPP

#include <stdint.h>

typedef struct mem_info {
    uint32_t base_addr;
    uint32_t length;
} mem_info_t;

class Memory {
  private:
    static mem_info_t mem[5];
    static uint32_t   mem_len;

  public:
    Memory(uint32_t base_addr, uint32_t length, bool isValidMemInfo = false);
};

#endif