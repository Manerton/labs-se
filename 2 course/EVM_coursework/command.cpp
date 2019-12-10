#include <iostream>
#include <cmath>
#include <cfloat> // для FLT_EPSILON, чтобы сравнивать результат дробных вычислений с 0
#include "command.h"

// -- КОМАНДЫ ПЕРЕСЫЛКИ ДАННЫХ -- //
void exchange::operator()(CPU &cpu) noexcept
{
    const uint8_t r1_i = cpu.get_cmd_r1(); // -- получаю номера регистров -- //
    const uint8_t r2_i = cpu.get_cmd_r2();
    std::swap(cpu.r[r1_i].u,cpu.r[r2_i].u);
}

void move::operator()(CPU &cpu) noexcept
{
    const uint8_t r1_i = cpu.get_cmd_r1(); // -- получаю номера регистров -- //
    const uint8_t r2_i = cpu.get_cmd_r2();
    cpu.r[r2_i].u = cpu.r[r1_i].u;  // -- пересылка <регистр источник> -> <регистр назначения> -- //
}

void load::operator()(CPU &cpu) noexcept // -- пересылка память - регистр (в r2) -- //
{
    const uint8_t r2_i = cpu.get_cmd_r2();
    const uint16_t address = cpu.get_cmd_address();
    cpu.r[r2_i].u = cpu.ram.get_data(address).u;
}

void direct_int_load::operator()(CPU &cpu) noexcept // -- загрузка целочисленного значения вместо адреса в регистр (в r2) (непосредственная адресация) -- //
{
    const uint8_t r2_i = cpu.get_cmd_r2();
    const int16_t value = int16_t(cpu.get_cmd_address());   // -- беру целочисленное значение из команды -- //
    cpu.r[r2_i].i = value;  // -- помещаю его в регистр -- //
}

void save::operator()(CPU &cpu) noexcept    // -- пересылка регистр (r1) - память [32] -- //
{
    const uint8_t r1_i = cpu.get_cmd_r1();
    const data_t data = cpu.r[r1_i];
    const uint16_t address = cpu.get_cmd_address();
    cpu.ram.push(data,address);
}

void load_address::operator()(CPU &cpu) noexcept    // -- загрузка константы-адреса в регистр [32] -- //
{
    const uint8_t r2_i = cpu.get_cmd_r2();
    const uint16_t address = cpu.get_cmd_address();
    cpu.r[r2_i].u = address;
}

void indirect_save::operator()(CPU &cpu) noexcept // -- сохранение результата из r1 в память по адресу, который хранится в r2 [16] -- //
{
    const uint8_t r1_i = cpu.get_cmd_r1();
    const uint8_t r2_i = cpu.get_cmd_r2();
    const data_t data = cpu.r[r1_i];
    const uint16_t address = uint16_t(cpu.r[r2_i].u);
    cpu.ram.push(data,address);
}

void dereference_ptr::operator()(CPU &cpu) noexcept
{
    const uint8_t t = cpu.get_cmd_t();
    const uint8_t r1_i = cpu.get_cmd_r1(); // -- получаю номер регистра с адресом -- //
    const uint16_t ptr = uint16_t(cpu.r[r1_i].u);
    const data_t data = cpu.ram.get_data(ptr);
    if (t == 0) // -- если команда короткая, то сохраняю значение во 2-й регистр -- //
    {
        const uint8_t r2_i = cpu.get_cmd_r2();
        cpu.r[r2_i] = data;
    }
    else    // -- если команда длинная, то сохраняю значение по адресу команды -- //
    {
        const uint16_t address = cpu.get_cmd_address();
        cpu.ram.push(data,address);
    }
}


// -- КОНЕЦ КОМАНДЫ ПЕРЕСЫЛКИ ДАННЫХ -- //

// -- КОМАНДЫ ЦЕЛОЙ АРИФМЕТИКИ -- //
void iMath::set_flags(CPU &cpu) noexcept
{
    const uint8_t r_i = cpu.get_cmd_r2();   // -- результат помещается во 2-й операнд (регистр), поэтому получаю номер этого регистра -- //
    if (cpu.r[r_i].i == 0) cpu.PSW.FLAGS.ZF = 1;    // -- если результат = 0, то флаг ZF = 1 -- //
    else cpu.PSW.FLAGS.ZF = 0;  // -- иначе ZF = 0 -- //

    if (cpu.r[r_i].i < 0) cpu.PSW.FLAGS.SF = 1; // -- если результат < 0, то флаг SF = 1 -- //
    else cpu.PSW.FLAGS.SF = 0;  // -- иначе SF = 0 -- //
}

void iMath::operator()(CPU &cpu) noexcept
{
    const uint8_t r1_i = cpu.get_cmd_r1(); // -- получаю номера регистров -- //
    const uint8_t r2_i = cpu.get_cmd_r2();
    const uint8_t t = cpu.get_cmd_t();
    if (t == 0) // -- если команда короткая (двухадресная арифметика) -- //
    {
        cpu.r[r2_i].i = calculate(cpu.r[r1_i].i, cpu.r[r2_i].i);    // -- помещаю результат во второй операнд (регистр) -- //
    }
    else    // -- если команда длинная (трехадресная арифметика) -- //
    {
        const uint16_t base_address = uint16_t(cpu.r[r1_i].u);    // -- базовый адрес в первом регистре -- //
        const uint16_t offset = cpu.get_cmd_address();    // -- смещение в команде -- //
        const uint16_t address = base_address + offset;
        int32_t value = cpu.ram.get_data(address).i;
        cpu.r[r2_i].i = calculate(value, cpu.r[r2_i].i);
    }
    set_flags(cpu); // -- устанавливаю флаги -- //
}
// -- КОНЕЦ КОМАНДЫ ЦЕЛОЙ АРИФМЕТИКИ -- //

// -- КОМАНДЫ ДРОБНОЙ АРИФМЕТИКИ -- //
void fMath::set_flags(CPU &cpu) noexcept
{
    const uint8_t r_i = cpu.get_cmd_r2();   // -- результат помещается во 2-й операнд (регистр), поэтому получаю номер этого регистра -- //
    if (fabsf(cpu.r[r_i].f) <= FLT_EPSILON) cpu.PSW.FLAGS.ZF = 1;    // -- если результат = 0, то флаг ZF = 1 -- //
    else cpu.PSW.FLAGS.ZF = 0;  // -- иначе ZF = 0 -- //

    if (cpu.r[r_i].f < 0) cpu.PSW.FLAGS.SF = 1; // -- если результат < 0, то флаг SF = 1 -- //
    else cpu.PSW.FLAGS.SF = 0;  // -- иначе SF = 0 -- //
}

void fMath::operator()(CPU &cpu) noexcept
{
    const uint8_t r1_i = cpu.get_cmd_r1(); // -- получаю номера регистров -- //
    const uint8_t r2_i = cpu.get_cmd_r2();
    const uint8_t t = cpu.get_cmd_t();
    if (t == 0) // -- если команда короткая (двухадресная арифметика) -- //
    {
        cpu.r[r2_i].f = calculate(cpu.r[r1_i].f, cpu.r[r2_i].f);    // -- помещаю результат во второй операнд (регистр) -- //
    }
    else    // -- если команда длинная (трехадресная арифметика) -- //
    {
        const uint16_t base_address = uint16_t(cpu.r[r1_i].u);    // -- базовый адрес в первом регистре -- //
        const uint16_t offset = cpu.get_cmd_address();    // -- смещение в команде -- //
        const uint16_t address = base_address + offset;
        float value = cpu.ram.get_data(address).f;
        cpu.r[r2_i].f = calculate(value, cpu.r[r2_i].f);
    }
    set_flags(cpu); // -- устанавливаю флаги -- //
}
// -- КОНЕЦ КОМАНДЫ ДРОБНОЙ АРИФМЕТИКИ -- //

// -- КОМАНДЫ ВВОДА И ВЫВОДА -- //
void out_int::operator()(CPU &cpu) noexcept
{

    const uint8_t t = cpu.get_cmd_t();
    std::cout << std::endl << "int: ";

    if (t == 0) // -- если команда короткая -- //
    {
        const uint8_t r1_i = cpu.get_cmd_r1(); // -- получаю номер регистра с результатом -- //
        std::cout << cpu.r[r1_i].i << std::endl;
    }
    else    // -- если команда длинная -- //
    {
        const uint16_t address = cpu.get_cmd_address();
        std::cout << cpu.ram.get_data(address).i << std::endl;
    }

}

void out_uint::operator()(CPU &cpu) noexcept
{
    const uint8_t t = cpu.get_cmd_t();
     std::cout << std::endl << "unsigned int: ";
    if (t == 0) // -- если команда короткая -- //
    {
        const uint8_t r1_i = cpu.get_cmd_r1(); // -- получаю номер регистра с результатом -- //
        std::cout << cpu.r[r1_i].u << std::endl;
    }
    else    // -- если команда длинная -- //
    {
        const uint16_t address = cpu.get_cmd_address();
        std::cout << cpu.ram.get_data(address).u << std::endl;
    }
}

void out_float::operator()(CPU &cpu) noexcept
{

    const uint8_t t = cpu.get_cmd_t();
    std::cout << std::endl << "float: " << std::fixed;
    if (t == 0) // -- если команда короткая -- //
    {
        const uint8_t r1_i = cpu.get_cmd_r1(); // -- получаю номер регистра с результатом -- //
        std::cout << cpu.r[r1_i].f << std::endl;
    }
    else    // -- если команда длинная -- //
    {
        const uint16_t address = cpu.get_cmd_address();
        std::cout << cpu.ram.get_data(address).f << std::endl;
    }
}
// -- КОНЕЦ КОМАНДЫ ВВОДА И ВЫВОДА -- //
