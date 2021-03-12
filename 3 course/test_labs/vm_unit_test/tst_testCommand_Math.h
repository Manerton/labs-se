#ifndef TST_TESTCOMMAND_MATH_H
#define TST_TESTCOMMAND_MATH_H

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

class testCommandIntMath : public ::testing::TestWithParam<std::pair<int32_t, int32_t>> {};
class testCommandIntMathDiv : public testCommandIntMath {};
class testCommandIntMathAddOF : public testCommandIntMath {};
class testCommandIntMathSubOF : public testCommandIntMath {};
class testCommandIntMathMulOF : public testCommandIntMath {};
class testCommandFloatMath : public ::testing::TestWithParam<std::pair<float, float>> {};
class testCommandFloatMathDiv : public testCommandFloatMath {};

INSTANTIATE_TEST_SUITE_P(CorrectValues, testCommandIntMath, ::testing::Values(
                             std::make_pair( 3,   7),
                             std::make_pair(-3,  -7),
                             std::make_pair(-3,   7),
                             std::make_pair( 3,  -7),
                             std::make_pair( 3,   0),
                             std::make_pair(-3,   0),
                             std::make_pair( 0,   3),
                             std::make_pair( 0,  -3),
                             std::make_pair( 155,  80000)
                             ));

INSTANTIATE_TEST_SUITE_P(CorrectValues, testCommandIntMathDiv, ::testing::Values(
                             std::make_pair( 3,   7),
                             std::make_pair( 7,   3),
                             std::make_pair(-3,  -7),
                             std::make_pair(-7,  -3),
                             std::make_pair(-3,   7),
                             std::make_pair( 7,  -3),
                             std::make_pair( 3,  -7),
                             std::make_pair(-7,   3),
                             std::make_pair( 0,   3),
                             std::make_pair( 0,  -3),
                             std::make_pair( 155,  80000),
                             std::make_pair( 80000,  155)
                             ));

INSTANTIATE_TEST_SUITE_P(OverflowValues, testCommandIntMathAddOF, ::testing::Values(
                             std::make_pair( std::numeric_limits<int32_t>::min(), -1 ),
                             std::make_pair( std::numeric_limits<int32_t>::max(),  1 )
                             ));
INSTANTIATE_TEST_SUITE_P(OverflowValues, testCommandIntMathSubOF, ::testing::Values(
                             std::make_pair( std::numeric_limits<int32_t>::min(), 1 ),
                             std::make_pair( std::numeric_limits<int32_t>::max(), -1 )
                             ));
INSTANTIATE_TEST_SUITE_P(OverflowValues, testCommandIntMathMulOF, ::testing::Values(
                             std::make_pair( std::numeric_limits<int32_t>::min(), -1 ),
                             std::make_pair( std::numeric_limits<int32_t>::max(),  2 ),
                             std::make_pair( 1'000'000, 1'000'000 )
                             ));

INSTANTIATE_TEST_SUITE_P(CorrectValues, testCommandFloatMath, ::testing::Values(
                             std::make_pair( 3,   7),
                             std::make_pair(-3,  -7),
                             std::make_pair(-3,   7),
                             std::make_pair( 3,  -7),
                             std::make_pair( 3,   0),
                             std::make_pair(-3,   0),
                             std::make_pair( 0,   3),
                             std::make_pair( 0,  -3),
                             std::make_pair( 155,  80000),
                             std::make_pair( 3.34,   7.21),
                             std::make_pair(-3.34,  -7.21),
                             std::make_pair(-3.34,   7.21),
                             std::make_pair( 3.34,  -7.21),
                             std::make_pair( 3.34,   0),
                             std::make_pair(-3.34,   0),
                             std::make_pair( 3.34,   0.001),
                             std::make_pair(-3.34,   0.001),
                             std::make_pair( 0,   3.34),
                             std::make_pair( 0,  -3.34),
                             std::make_pair( 0.001,   3.34),
                             std::make_pair( 0.001,  -3.34),
                             std::make_pair( 155.2,  80000.787)
                             ));

INSTANTIATE_TEST_SUITE_P(CorrectValues, testCommandFloatMathDiv, ::testing::Values(
                             std::make_pair( 3,   7),
                             std::make_pair(-3,  -7),
                             std::make_pair(-3,   7),
                             std::make_pair( 3,  -7),
                             std::make_pair( 0,   3),
                             std::make_pair( 0,  -3),
                             std::make_pair( 155,  80000),
                             std::make_pair( 3.34,   7.21),
                             std::make_pair(-3.34,  -7.21),
                             std::make_pair(-3.34,   7.21),
                             std::make_pair( 3.34,  -7.21),
                             std::make_pair( 3.34,   0.001),
                             std::make_pair(-3.34,   0.001),
                             std::make_pair( 0,   3.34),
                             std::make_pair( 0,  -3.34),
                             std::make_pair( 0.001,   3.34),
                             std::make_pair( 0.001,  -3.34),
                             std::make_pair( 155.2,  80000.787)
                             ));

bool isFloatZero(float f)
{
    return (fabsf(f) <= std::numeric_limits<float>::epsilon());
}

void prepareCPU(CPU& cpu, CPU::COP oper, int32_t first, int32_t second)
{
    // -- загружаем в стек первое число -- //
    cpu.ST[++cpu.PSW.SP].i = first;
    // -- загружаем в память второе число -- //
    data_t value;
    value.i = second;
    const address_t valueAddress = 1000;
    cpu.ram.push(value, valueAddress);
    // -- инициализируем команду в cpu -- //
    const address_t IP = 0;
    cpu.PSW.IP = IP;
    const cmd_t cmd = {{oper, valueAddress}};
    cpu.ram.push(cmd, IP);
    cpu.loadCommand();
}

void prepareCPU(CPU& cpu, CPU::COP oper, float first, float second)
{
    // -- загружаем в стек первое число -- //
    cpu.ST[++cpu.PSW.SP].f = first;
    // -- загружаем в память второе число -- //
    data_t value;
    value.f = second;
    const address_t valueAddress = 1000;
    cpu.ram.push(value, valueAddress);
    // -- инициализируем команду в cpu -- //
    const address_t IP = 0;
    cpu.PSW.IP = IP;
    const cmd_t cmd = {{oper, valueAddress}};
    cpu.ram.push(cmd, IP);
    cpu.loadCommand();
}

TEST_P(testCommandIntMath, iAdd)
{
    // Подготовка
    int32_t first = GetParam().first;
    int32_t second = GetParam().second;
    int32_t expected = first+second;

    CPU cpu;
    prepareCPU(cpu, CPU::COP::iAdd, first, second);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class iAdd>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_EQ(cpu.ST[cpu.PSW.SP].i, expected);
    EXPECT_EQ(cpu.PSW.ZF, expected == 0);
    EXPECT_EQ(cpu.PSW.SF, expected < 0);
}

TEST_P(testCommandIntMath, iSub)
{
    // Подготовка
    int32_t first = GetParam().first;
    int32_t second = GetParam().second;
    int32_t expected = first-second;

    CPU cpu;
    prepareCPU(cpu, CPU::COP::iSub, first, second);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class iSub>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_EQ(cpu.ST[cpu.PSW.SP].i, expected);
    EXPECT_EQ(cpu.PSW.ZF, expected == 0);
    EXPECT_EQ(cpu.PSW.SF, expected < 0);
}

TEST_P(testCommandIntMath, iMul)
{
    // Подготовка
    int32_t first = GetParam().first;
    int32_t second = GetParam().second;
    int32_t expected = first*second;

    CPU cpu;
    prepareCPU(cpu, CPU::COP::iMul, first, second);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class iMul>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_EQ(cpu.ST[cpu.PSW.SP].i, expected);
    EXPECT_EQ(cpu.PSW.ZF, expected == 0);
    EXPECT_EQ(cpu.PSW.SF, expected < 0);
}

TEST_P(testCommandIntMathDiv, iDiv)
{
    // Подготовка
    int32_t first = GetParam().first;
    int32_t second = GetParam().second;
    int32_t expected = first/second;

    CPU cpu;
    prepareCPU(cpu, CPU::COP::iDiv, first, second);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class iDiv>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_EQ(cpu.ST[cpu.PSW.SP].i, expected);
    EXPECT_EQ(cpu.PSW.ZF, expected == 0);
    EXPECT_EQ(cpu.PSW.SF, expected < 0);
}

TEST_P(testCommandIntMathAddOF, OF)
{
    // Подготовка
    int32_t first = GetParam().first;
    int32_t second = GetParam().second;

    CPU cpu;
    prepareCPU(cpu, CPU::COP::iAdd, first, second);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class iAdd>();

    // Действие Проверка
    EXPECT_THROW(sut->operator()(cpu), std::overflow_error);
}

TEST_P(testCommandIntMathSubOF, OF)
{
    // Подготовка
    int32_t first = GetParam().first;
    int32_t second = GetParam().second;

    CPU cpu;
    prepareCPU(cpu, CPU::COP::iSub, first, second);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class iSub>();

    // Действие Проверка
    EXPECT_THROW(sut->operator()(cpu), std::overflow_error);
}

TEST_P(testCommandIntMathMulOF, OF)
{
    // Подготовка
    int32_t first = GetParam().first;
    int32_t second = GetParam().second;

    CPU cpu;
    prepareCPU(cpu, CPU::COP::iMul, first, second);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class iMul>();

    // Действие Проверка
    EXPECT_THROW(sut->operator()(cpu), std::overflow_error);
}

TEST(testCommandIntMathDivErr, OF)
{
    // Подготовка
    int32_t first = std::numeric_limits<int32_t>::min();
    int32_t second = -1;

    CPU cpu;
    prepareCPU(cpu, CPU::COP::iDiv, first, second);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class iDiv>();

    // Действие Проверка
    EXPECT_THROW(sut->operator()(cpu), std::overflow_error);
}

TEST(testCommandIntMathDivErr, DivideByZero)
{
    // Подготовка
    int32_t first = 100;
    int32_t second = 0;

    CPU cpu;
    prepareCPU(cpu, CPU::COP::iDiv, first, second);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class iDiv>();

    // Действие Проверка
    EXPECT_THROW(sut->operator()(cpu), std::invalid_argument);
}

TEST_P(testCommandFloatMath, fAdd)
{
    // Подготовка
    float first = GetParam().first;
    float second = GetParam().second;
    float expected = first+second;

    CPU cpu;
    prepareCPU(cpu, CPU::COP::fAdd, first, second);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class fAdd>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_FLOAT_EQ(cpu.ST[cpu.PSW.SP].f, expected);
    EXPECT_EQ(cpu.PSW.ZF, isFloatZero(expected));
    EXPECT_EQ(cpu.PSW.SF, expected < 0);
}

TEST_P(testCommandFloatMath, fSub)
{
    // Подготовка
    float first = GetParam().first;
    float second = GetParam().second;
    float expected = first-second;

    CPU cpu;
    prepareCPU(cpu, CPU::COP::fSub, first, second);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class fSub>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_FLOAT_EQ(cpu.ST[cpu.PSW.SP].f, expected);
    EXPECT_EQ(cpu.PSW.ZF, isFloatZero(expected));
    EXPECT_EQ(cpu.PSW.SF, expected < 0);
}

TEST_P(testCommandFloatMath, fMul)
{
    // Подготовка
    float first = GetParam().first;
    float second = GetParam().second;
    float expected = first*second;

    CPU cpu;
    prepareCPU(cpu, CPU::COP::fMul, first, second);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class fMul>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_FLOAT_EQ(cpu.ST[cpu.PSW.SP].f, expected);
    EXPECT_EQ(cpu.PSW.ZF, isFloatZero(expected));
    EXPECT_EQ(cpu.PSW.SF, expected < 0);
}

TEST_P(testCommandFloatMathDiv, fDiv)
{
    // Подготовка
    float first = GetParam().first;
    float second = GetParam().second;
    float expected = first/second;

    CPU cpu;
    prepareCPU(cpu, CPU::COP::fDiv, first, second);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class fDiv>();

    // Действие
    sut->operator()(cpu);

    // Проверка
    EXPECT_FLOAT_EQ(cpu.ST[cpu.PSW.SP].f, expected);
    EXPECT_EQ(cpu.PSW.ZF, isFloatZero(expected));
    EXPECT_EQ(cpu.PSW.SF, expected < 0);
}

TEST(testCommandFloatMathDivErr, DivideByZero)
{
    // Подготовка
    float first = 100;
    float second = 0;

    CPU cpu;
    prepareCPU(cpu, CPU::COP::fDiv, first, second);
    // -- подготавливаем тестируемую команду -- //
    const std::unique_ptr<Command> sut = std::make_unique<class fDiv>();

    // Действие Проверка
    EXPECT_THROW(sut->operator()(cpu), std::invalid_argument);
}

#endif // TST_TESTCOMMAND_MATH_H
