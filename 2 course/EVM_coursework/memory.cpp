#include "memory.h"

void Memory::push(word word, uint16_t address) noexcept
{
    memory[address] = word.data.u;
}

word Memory::get_word(uint16_t address) const noexcept
{
    word tmp;
    tmp.data.u = memory[address];
    return tmp;
}
