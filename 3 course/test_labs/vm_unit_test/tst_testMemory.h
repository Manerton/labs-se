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
    Memory sut;
    // проверяем, чтобы указатель на динамический массив был ненулевым
    EXPECT_TRUE(sut.get_memory());
}

TEST(testMemory, pushByte)
{
    // Подготовка
    Memory sut;
    const VM_types::byte_t byte = 0b11111110;
    const VM_types::address_t address = 1000;
    // Действие
    sut.push(byte,address);
    // Проверка
    EXPECT_EQ(sut.get_memory()[address], byte);
}

TEST(testMemory, pushCmd)
{
    // Подготовка
    Memory sut;
    const VM_types::cmd_t cmd = {{10, 1000}};
    const VM_types::address_t address = 1000;
    const size_t size = sizeof(cmd);
    // Действие
    sut.push(cmd,address);
    // Проверка
    for (size_t i = 0; i < size; ++i)
    {
        EXPECT_EQ(sut.get_memory()[address+i], cmd.b[i]);
    }
}

TEST(testMemory, pushData)
{
    // Подготовка
    Memory sut;
    const VM_types::data_t data = {4'000'000'000};
    const VM_types::address_t address = 1000;
    const size_t size = sizeof(data);
    // Действие
    sut.push(data,address);
    // Проверка
    for (size_t i = 0; i < size; ++i)
    {
        EXPECT_EQ(sut.get_memory()[address+i], data.b[i]);
    }
}

TEST(testMemory, getByte)
{
    // Подготовка
    Memory sut;
    const VM_types::byte_t byte = 0b11111110;
    const VM_types::address_t address = 1000;
    sut.push(byte,address);
    // Действие
    const auto Actual = sut.get_byte(address);
    // Проверка
    EXPECT_EQ(byte, Actual);
}

TEST(testMemory, getCmd)
{
    // Подготовка
    Memory sut;
    const VM_types::cmd_t cmd = {{10, 1000}};
    const VM_types::address_t address = 1000;
    sut.push(cmd,address);
    // Действие
    const auto Actual = sut.get_cmd(address);
    // Проверка
    EXPECT_EQ(cmd.b, Actual.b);
}

TEST(testMemory, getData)
{
    // Подготовка
    Memory sut;
    const VM_types::data_t data = {4'000'000'000};
    const VM_types::address_t address = 1000;
    sut.push(data,address);
    // Действие
    const auto Actual = sut.get_data(address);
    // Проверка
    EXPECT_EQ(data.b, Actual.b);
}

#endif // TST_TESTMEMORY_H
