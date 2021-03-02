#ifndef TST_TESTMEMORY_H
#define TST_TESTMEMORY_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <algorithm>
#include "../../spo_lab/lab01/types.h"
#include "../../spo_lab/lab01/memory.h"

using namespace testing;

TEST(testMemory, creating)
{
    Memory mem;
    // проверяем, чтобы указатель на динамический массив был ненулевым
    EXPECT_TRUE(mem.get_memory());
}

TEST(testMemory, pushByte)
{
    // Подготовка
    Memory mem;
    const VM_types::byte_t byte = 0b11111110;
    const VM_types::address_t address = 1000;
    // Действие
    mem.push(byte,address);
    // Проверка
    EXPECT_EQ(mem.get_memory()[address], byte);
}

TEST(testMemory, pushCmd)
{
    // Подготовка
    Memory mem;
    const VM_types::cmd_t cmd = {{10, 1000}};
    const VM_types::address_t address = 1000;
    const size_t size = sizeof(cmd);
    // Действие
    mem.push(cmd,address);
    // Проверка
    for (size_t i = 0; i < size; ++i)
    {
        EXPECT_EQ(mem.get_memory()[address+i], cmd.b[i]);
    }
}

TEST(testMemory, pushData)
{
    // Подготовка
    Memory mem;
    const VM_types::data_t data = {4'000'000'000};
    const VM_types::address_t address = 1000;
    const size_t size = sizeof(data);
    // Действие
    mem.push(data,address);
    // Проверка
    for (size_t i = 0; i < size; ++i)
    {
        EXPECT_EQ(mem.get_memory()[address+i], data.b[i]);
    }
}

TEST(testMemory, getByte)
{
    // Подготовка
    Memory mem;
    const VM_types::byte_t byte = 0b11111110;
    const VM_types::address_t address = 1000;
    mem.push(byte,address);
    // Действие
    const auto Actual = mem.get_byte(address);
    // Проверка
    EXPECT_EQ(byte, Actual);
}

TEST(testMemory, getCmd)
{
    // Подготовка
    Memory mem;
    const VM_types::cmd_t cmd = {{10, 1000}};
    const VM_types::address_t address = 1000;
    mem.push(cmd,address);
    // Действие
    const auto Actual = mem.get_cmd(address);
    // Проверка
    EXPECT_EQ(cmd.b, Actual.b);
}

TEST(testMemory, getData)
{
    // Подготовка
    Memory mem;
    const VM_types::data_t data = {4'000'000'000};
    const VM_types::address_t address = 1000;
    mem.push(data,address);
    // Действие
    const auto Actual = mem.get_data(address);
    // Проверка
    EXPECT_EQ(data.b, Actual.b);
}

#endif // TST_TESTMEMORY_H
