#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

class Memory
{
    uint32_t *memory;
public:
    Memory() { memory = new uint32_t [UINT16_MAX]; }
    ~Memory() { delete[] memory; }

    Memory(const Memory &)  = delete;
    Memory(const Memory &&) = delete;
    Memory& operator=(const Memory &)  = delete;
    Memory& operator=(const Memory &&) = delete;

    void push (word word, uint16_t address) noexcept;

    word get_word (uint16_t address) const noexcept;
};

#endif // MEMORY_H
