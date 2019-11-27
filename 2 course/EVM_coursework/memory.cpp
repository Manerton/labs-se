#include "memory.h"

void Memory::push(const data_t &data, const uint16_t &address) noexcept
{
    memory[address] = data.u;
}

void Memory::push(const cmd16_t &cmd1, const cmd16_t &cmd2, const uint16_t &address) noexcept
{
    word_t tmp;
    tmp.hw.low = cmd1.hw;
    tmp.hw.high = cmd2.hw;
    memory[address] = tmp.w;
}

data_t Memory::get_data(const uint16_t &address) const noexcept
{
    data_t tmp;
    tmp.u = memory[address];
}

cmd16_t Memory::get_cmd16(const uint16_t &address) const noexcept
{
}
