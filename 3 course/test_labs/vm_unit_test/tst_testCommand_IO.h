#ifndef TST_testCommand_IO_H
#define TST_testCommand_IO_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <algorithm>
#include <sstream>
#include "../../spo_lab/lab01/types.h"
#include "../../spo_lab/lab01/cpu.h"
#include "../../spo_lab/lab01/command.h"

using namespace testing;
using VM_types::cmd_t;
using VM_types::address_t;
using VM_types::data_t;

void prepareCPU(CPU& cpu, CPU::COP oper, address_t address = 0)
{
    // -- инициализируем команду в cpu -- //
    const address_t IP = 0;
    cpu.PSW.IP = IP;
    const cmd_t cmd = {{oper, address}};
    cpu.ram.push(cmd, IP);
    cpu.loadCommand();
}

TEST(testCommandIO, st_in_int_signed)
{
    // Подготовка
    std::istringstream iss("-35");
    std::ostringstream oss;
    CPU cpu(iss, oss);
    prepareCPU(cpu,CPU::COP::st_in_int);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class st_in_int>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_EQ(cpu.ST[cpu.PSW.SP].i, -35);
}

TEST(testCommandIO, st_in_int_unsigned)
{
    // Подготовка
    std::istringstream iss("4294966854");
    std::ostringstream oss;
    CPU cpu(iss, oss);
    prepareCPU(cpu,CPU::COP::st_in_int);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class st_in_int>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_EQ(cpu.ST[cpu.PSW.SP].u, 4294966854);
}

TEST(testCommandIO, st_in_float)
{
    // Подготовка
    std::istringstream iss("37.34");
    std::ostringstream oss;
    CPU cpu(iss, oss);
    prepareCPU(cpu,CPU::COP::st_in_float);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class st_in_float>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_FLOAT_EQ(cpu.ST[cpu.PSW.SP].f, 37.34f);
}

TEST(testCommandIO, st_out_int)
{
    // Подготовка
    std::istringstream iss;
    std::ostringstream oss;
    CPU cpu(iss, oss);
    prepareCPU(cpu,CPU::COP::st_out_int);
    // -- загружаем в стек выводимый элемент -- //
    data_t elem;
    elem.i = -35;
    cpu.ST[++cpu.PSW.SP] = elem;
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class st_out_int>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_EQ(oss.str(), "\n" "int: -35" "\n");
}

TEST(testCommandIO, st_out_uint)
{
    // Подготовка
    std::istringstream iss;
    std::ostringstream oss;
    CPU cpu(iss, oss);
    prepareCPU(cpu,CPU::COP::st_out_uint);
    // -- загружаем в стек выводимый элемент -- //
    data_t elem;
    elem.u = 4294966854;
    cpu.ST[++cpu.PSW.SP] = elem;
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class st_out_uint>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_EQ(oss.str(), "\n" "unsigned int: 4294966854" "\n");
}

TEST(testCommandIO, st_out_float)
{
    // Подготовка
    std::istringstream iss;
    std::ostringstream oss;
    CPU cpu(iss, oss);
    prepareCPU(cpu,CPU::COP::st_out_float);
    // -- загружаем в стек выводимый элемент -- //
    data_t elem;
    elem.f = 37.34f;
    cpu.ST[++cpu.PSW.SP] = elem;
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class st_out_float>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_EQ(oss.str(), "\n" "float: 37.34" "\n");
}

TEST(testCommandIO, mem_in_int_signed)
{
    // Подготовка
    std::istringstream iss("-35");
    std::ostringstream oss;
    CPU cpu(iss, oss);
    address_t address = 1000;
    prepareCPU(cpu,CPU::COP::mem_in_int, address);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class mem_in_int>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_EQ(cpu.ram.get_data(1000).i, -35);
}

TEST(testCommandIO, mem_in_int_unsigned)
{
    // Подготовка
    std::istringstream iss("4294966854");
    std::ostringstream oss;
    CPU cpu(iss, oss);
    address_t address = 1000;
    prepareCPU(cpu,CPU::COP::mem_in_int, address);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class mem_in_int>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_EQ(cpu.ram.get_data(1000).u, 4294966854);
}

TEST(testCommandIO, mem_in_float)
{
    // Подготовка
    std::istringstream iss("37.34");
    std::ostringstream oss;
    CPU cpu(iss, oss);
    address_t address = 1000;
    prepareCPU(cpu,CPU::COP::mem_in_float, address);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class mem_in_float>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_FLOAT_EQ(cpu.ram.get_data(1000).f, 37.34f);
}

TEST(testCommandIO, mem_out_int)
{
    // Подготовка
    std::istringstream iss;
    std::ostringstream oss;
    CPU cpu(iss, oss);
    address_t address = 1000;
    prepareCPU(cpu,CPU::COP::mem_out_int, address);
    // -- загружаем в память выводимый элемент -- //
    data_t elem;
    elem.i = -35;
    cpu.ram.push(elem, address);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class mem_out_int>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_EQ(oss.str(), "\n" "int: -35" "\n");
}

TEST(testCommandIO, mem_out_uint)
{
    // Подготовка
    std::istringstream iss;
    std::ostringstream oss;
    CPU cpu(iss, oss);
    address_t address = 1000;
    prepareCPU(cpu,CPU::COP::mem_out_uint, address);
    // -- загружаем в память выводимый элемент -- //
    data_t elem;
    elem.u = 4294966854;
    cpu.ram.push(elem, address);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class mem_out_uint>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_EQ(oss.str(), "\n" "unsigned int: 4294966854" "\n");
}

TEST(testCommandIO, mem_out_float)
{
    // Подготовка
    std::istringstream iss;
    std::ostringstream oss;
    CPU cpu(iss, oss);
    address_t address = 1000;
    prepareCPU(cpu,CPU::COP::mem_out_float, address);
    // -- загружаем в память выводимый элемент -- //
    data_t elem;
    elem.f = 37.34f;
    cpu.ram.push(elem, address);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class mem_out_float>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_EQ(oss.str(), "\n" "float: 37.34" "\n");
}
#endif // TST_testCommand_IO_H
