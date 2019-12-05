#include <iostream>
#include "command.h"

void iMath::set_flags(CPU &cpu) noexcept
{
    const uint8_t r_i = cpu.get_cmd_r2();
    if (cpu.r[r_i].i == 0) cpu.PSW.FLAGS.ZF = 1;
    else cpu.PSW.FLAGS.ZF = 0;

    if (cpu.r[cpu.get_cmd_r2()].i < 0) cpu.PSW.FLAGS.SF = 1;
    else cpu.PSW.FLAGS.SF = 0;
}

void iMath::operator()(CPU &cpu) noexcept
{
    const uint8_t r1_i = cpu.get_cmd_r1();
    const uint8_t r2_i = cpu.get_cmd_r2();
    cpu.r[r2_i].i = work(cpu.r[r1_i].i, cpu.r[r2_i].i);
    set_flags(cpu);
}

void load::operator()(CPU &cpu) noexcept
{
    const uint8_t r2_i = cpu.get_cmd_r2();
    const uint16_t address = cpu.get_cmd_address();
    cpu.r[r2_i].u = cpu.ram.get_data(address).u;
}

void save::operator()(CPU &cpu) noexcept
{
    const uint8_t r1_i = cpu.get_cmd_r1();
    const data_t data = cpu.r[r1_i];
    const uint16_t address = cpu.get_cmd_address();
    cpu.ram.push(data,address);
}

void out_int::operator()(CPU &cpu) noexcept
{
    const uint16_t address = cpu.get_cmd_address();
    std::cout << std::endl << "int: " << cpu.ram.get_data(address).i << std::endl;
}
