#include "memory.h"

void Memory::push(const data_t &data, const uint16_t &address) noexcept
{
    memory[address] = data.u;
}

data_t Memory::get_data(const uint16_t &address) const noexcept
{
    data_t tmp;
    tmp.u = memory[address];
    return tmp;
}
