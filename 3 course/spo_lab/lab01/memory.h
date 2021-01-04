#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"
#include <memory>

class Memory
{
    using byte_t = VM_types::byte_t;
    using data_t = VM_types::data_t;
    using cmd_t = VM_types::cmd_t;

    std::unique_ptr<byte_t[]> memory;
public:
    Memory() { memory = std::make_unique<byte_t[]>(UINT16_MAX); }
    ~Memory() {}

    Memory(const Memory &)  = delete;
    Memory(const Memory &&) = delete;
    Memory& operator=(const Memory &)  = delete;
    Memory& operator=(const Memory &&) = delete;

    void push (byte_t byte, uint16_t address) noexcept
    {
        memory[address] = byte;
    }

    void push (cmd_t cmd, uint16_t address) noexcept
    {
        size_t size = VM_types::cmd_length;
        for (size_t i = 0; i < size; ++i)
        {
            push(cmd.b[i],uint16_t(address+i));
        }
    }

    void push (data_t data, uint16_t address) noexcept
    {
        size_t size = VM_types::data_length;
        for (size_t i = 0; i < size; ++i)
        {
            push(data.b[i],uint16_t(address+i));
        }
    }

    byte_t get_byte (uint16_t address) const noexcept
    {
        return memory[address];
    }

    cmd_t get_cmd (uint16_t address) const noexcept
    {
        cmd_t tmp;
        size_t size = VM_types::cmd_length;
        for (size_t i = 0; i < size; ++i)
        {
            tmp.b[i] = get_byte(uint16_t(address+i));
        }
        return tmp;
    }

    data_t get_data (uint16_t address) const noexcept
    {
        data_t tmp;
        size_t size = VM_types::data_length;
        for (size_t i = 0; i < size; ++i)
        {
            tmp.b[i] = get_byte(uint16_t(address+i));
        }
        return tmp;
    }
};

#endif // MEMORY_H
