#ifndef TST_TESTCPU_H
#define TST_TESTCPU_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <algorithm>
#include "../../spo_lab/lab01/types.h"
#include "../../spo_lab/lab01/cpu.h"

using namespace testing;
using VM_types::cmd_t;
using VM_types::address_t;

TEST(testCPU, creating)
{
    // Подготовка
    CPU sut;
    const uint32_t expected_ip_value = 0;
    const uint32_t expected_sp_value = sut.stack_size-1;
    // Проверка
    EXPECT_EQ(sut.PSW.IP, expected_ip_value);
    EXPECT_EQ(sut.PSW.SP, expected_sp_value);
}

TEST(testCPU, changeIpAfterRun)
{
    // Подготовка
    CPU sut;
    const cmd_t cmd = {{CPU::COP::unsign_value_load, 0}};
    const cmd_t stop = {{CPU::COP::stop, 0}};

    const address_t address = 1000;
    const address_t expected = address + VM_types::cmd_length;

    sut.ram.push(cmd,address);
    sut.ram.push(stop,expected);

    sut.PSW.IP = address;
    // Действие
    sut.run();
    // Проверка
    EXPECT_EQ(sut.PSW.IP, expected);
}

TEST(testCPU, wrongCommandCop)
{
    // Подготовка
    CPU sut;
    const uint8_t wrongCop = 255;
    const cmd_t cmd = {{wrongCop, 0}};
    const address_t address = 1000;
    sut.ram.push(cmd,address);
    sut.PSW.IP = address;
    // Действие и проверка
    EXPECT_THROW(sut.run(), std::invalid_argument);
}

TEST(testCPU, stopCommand)
{
    // Подготовка
    CPU sut;
    const cmd_t stop = {{CPU::COP::stop, 0}};
    const address_t address = 1000;
    sut.ram.push(stop,address);
    sut.PSW.IP = address;
    // Действие
    sut.run();
    // Проверка
    EXPECT_EQ(sut.PSW.IP, address); // ip не изменился
}

#endif // TST_TESTCPU_H
