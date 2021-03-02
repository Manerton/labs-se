#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"
#include <memory>

class Memory
{
    using byte_t = VM_types::byte_t;
    using data_t = VM_types::data_t;
    using address_t = VM_types::address_t;
    using cmd_t = VM_types::cmd_t;
    constexpr static auto memory_size = std::numeric_limits<address_t>::max();
    std::unique_ptr<byte_t[]> memory;
public:
    Memory() { memory = std::make_unique<byte_t[]>(memory_size); }
    ~Memory() {}

    Memory(const Memory &)  = delete;
    Memory(const Memory &&) = delete;
    Memory& operator=(const Memory &)  = delete;
    Memory& operator=(const Memory &&) = delete;

    void push (byte_t byte, address_t address) noexcept
    {
        memory[address] = byte;
    }

    void push (cmd_t cmd, address_t address) noexcept
    {
        size_t size = VM_types::cmd_length;
        for (size_t i = 0; i < size; ++i)
        {
            push(cmd.b[i],static_cast<address_t>(address+i));
        }
    }

    void push (data_t data, address_t address) noexcept
    {
        size_t size = VM_types::data_length;
        for (size_t i = 0; i < size; ++i)
        {
            push(data.b[i],static_cast<address_t>(address+i));
        }
    }

    byte_t get_byte (address_t address) const noexcept
    {
        return memory[address];
    }

    cmd_t get_cmd (address_t address) const noexcept
    {
        cmd_t tmp;
        size_t size = VM_types::cmd_length;
        for (size_t i = 0; i < size; ++i)
        {
            tmp.b[i] = get_byte(static_cast<address_t>(address+i));
        }
        return tmp;
    }

    data_t get_data (address_t address) const noexcept
    {
        data_t tmp;
        size_t size = VM_types::data_length;
        for (size_t i = 0; i < size; ++i)
        {
            tmp.b[i] = get_byte(static_cast<address_t>(address+i));
        }
        return tmp;
    }

    byte_t* get_memory() const noexcept
    {
        return memory.get();
    }
};

#endif // MEMORY_H
