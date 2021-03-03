#ifndef TST_TESTCOMMAND_H
#define TST_TESTCOMMAND_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <algorithm>
#include "../../spo_lab/lab01/types.h"
#include "../../spo_lab/lab01/cpu.h"
#include "../../spo_lab/lab01/command.h"

using namespace testing;
using VM_types::cmd_t;
using VM_types::address_t;
using VM_types::data_t;

TEST(testCommand, exchange)
{
    // Подготовка
    CPU cpu;
    // -- добавляем в стек элементы -- //
    const data_t first_elem = {10};
    const data_t second_elem = {20};
    cpu.ST[++cpu.PSW.SP] = first_elem;
    cpu.ST[++cpu.PSW.SP] = second_elem;
    // -- инициализируем команду в cpu -- //
    const address_t ip = 0;
    cpu.PSW.IP = ip;
    const address_t diff = 1;
    const cmd_t cmd = {{CPU::COP::exchange, diff}};
    cpu.ram.push(cmd, ip);
    cpu.loadCommand();
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class exchange>();
    // Действие
    sut->operator()(cpu);
    // Проверка
    EXPECT_EQ(cpu.ST[cpu.PSW.SP].b, first_elem.b);
    EXPECT_EQ(cpu.ST[cpu.PSW.SP-diff].b, second_elem.b);
}

TEST(testCommand, move)
{
    // Подготовка
    CPU cpu;
    // -- добавляем в стек элементы -- //
    const data_t first_elem = {10};
    const data_t second_elem = {20};
    const data_t third_elem = {30};
    cpu.ST[++cpu.PSW.SP] = first_elem;
    cpu.ST[++cpu.PSW.SP] = second_elem;
    cpu.ST[++cpu.PSW.SP] = third_elem;
    // -- инициализируем команду в cpu -- //
    const address_t ip = 0;
    cpu.PSW.IP = ip;
    const address_t diff = 2;
    const cmd_t cmd = {{CPU::COP::move, diff}};
    cpu.ram.push(cmd, ip);
    cpu.loadCommand();
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class move>();
    // Действие
    sut->operator()(cpu);
    // Проверка
    EXPECT_EQ(cpu.ST[cpu.PSW.SP].b, cpu.ST[cpu.PSW.SP-diff].b);
}

TEST(testCommand, sign_value_load)
{
    // Подготовка
    CPU cpu;
    // -- инициализируем команду в cpu -- //
    const address_t ip = 0;
    cpu.PSW.IP = ip;
    const int16_t elem = -10;
    const cmd_t cmd = {{CPU::COP::sign_value_load, static_cast<address_t>(elem)}};
    cpu.ram.push(cmd, ip);
    cpu.loadCommand();
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class sign_value_load>();
    // Действие
    sut->operator()(cpu);
    // Проверка
    EXPECT_EQ(cpu.ST[cpu.PSW.SP].i, elem);
}

TEST(testCommand, unsign_value_load)
{
    // Подготовка
    CPU cpu;
    // -- инициализируем команду в cpu -- //
    const address_t ip = 0;
    cpu.PSW.IP = ip;
    const uint16_t elem = 50000;
    const cmd_t cmd = {{CPU::COP::unsign_value_load, elem}};
    cpu.ram.push(cmd, ip);
    cpu.loadCommand();
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class unsign_value_load>();
    // Действие
    sut->operator()(cpu);
    // Проверка
    EXPECT_EQ(cpu.ST[cpu.PSW.SP].u, elem);
}

TEST(testCommand, direct_load)
{
    // Подготовка
    CPU cpu;
    // -- инициализируем команду в cpu -- //
    const address_t ip = 0;
    cpu.PSW.IP = ip;

    const address_t elem_address = 1000;
    const data_t elem = {4'000'000'000};
    cpu.ram.push(elem,elem_address);

    const cmd_t cmd = {{CPU::COP::direct_load, elem_address}};
    cpu.ram.push(cmd, ip);
    cpu.loadCommand();
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class direct_load>();
    // Действие
    sut->operator()(cpu);
    // Проверка
    EXPECT_EQ(cpu.ST[cpu.PSW.SP].b, elem.b);
}

TEST(testCommand, save)
{
    // Подготовка
    CPU cpu;
    // -- добавляем в стек элемент -- //
    const data_t elem = {10};
    cpu.ST[++cpu.PSW.SP] = elem;
    const auto oldSP = cpu.PSW.SP;
    // -- инициализируем команду в cpu -- //
    const address_t ip = 0;
    cpu.PSW.IP = ip;
    const address_t toSave = 1000;
    const cmd_t cmd = {{CPU::COP::save, toSave}};
    cpu.ram.push(cmd, ip);
    cpu.loadCommand();
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class save>();
    // Действие
    sut->operator()(cpu);
    // Проверка
    EXPECT_EQ(cpu.ram.get_data(toSave).b, elem.b);
    EXPECT_EQ(cpu.PSW.SP, oldSP);
}

TEST(testCommand, save_pop)
{
    // Подготовка
    CPU cpu;
    // -- добавляем в стек элемент -- //
    const auto startSP = cpu.PSW.SP;
    const data_t elem = {10};
    cpu.ST[++cpu.PSW.SP] = elem;
    const auto oldSP = cpu.PSW.SP;
    // -- инициализируем команду в cpu -- //
    const address_t ip = 0;
    cpu.PSW.IP = ip;
    const address_t toSave = 1000;
    const cmd_t cmd = {{CPU::COP::save_pop, toSave}};
    cpu.ram.push(cmd, ip);
    cpu.loadCommand();
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class save_pop>();
    // Действие
    sut->operator()(cpu);
    // Проверка
    EXPECT_EQ(cpu.ram.get_data(toSave).b, elem.b);
    EXPECT_NE(cpu.PSW.SP, oldSP);
    EXPECT_EQ(cpu.PSW.SP, startSP);
}

TEST(testCommand, dereference_ptr)
{
    // Подготовка
    CPU cpu;
    // -- добавляем в стек элемент -- //
    const auto startSP = cpu.PSW.SP;
    const data_t elem_ref = {1000};
    cpu.ST[++cpu.PSW.SP] = elem_ref;
    const auto oldSP = cpu.PSW.SP;
    // -- добавляем элемент в память по адресу elem_ref -- //
    const data_t elem = {10};
    cpu.ram.push(elem,elem_ref.u);
    // -- инициализируем команду в cpu -- //
    const address_t ip = 0;
    cpu.PSW.IP = ip;
    const address_t toSave = 2000;
    const cmd_t cmd = {{CPU::COP::dereference_ptr, toSave}};
    cpu.ram.push(cmd, ip);
    cpu.loadCommand();
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class dereference_ptr>();
    // Действие
    sut->operator()(cpu);
    // Проверка
    EXPECT_EQ(cpu.ram.get_data(toSave).b, elem.b);
    EXPECT_NE(cpu.PSW.SP, oldSP);
    EXPECT_EQ(cpu.PSW.SP, startSP);
}

#endif // TST_TESTCOMMAND_H
