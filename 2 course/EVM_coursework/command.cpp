#include <iostream>
#include <cmath>
#include <limits> // для numeric_limits<float>::epsilon(), чтобы сравнивать результат дробных вычислений с 0
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
    enum load_mode {  // -- значение во 1-м регистре команды является режимом загрузки, здесь перечислены все режимы -- //
        value_mode = 0,     // -- пересылка: значение из памяти -> регистр (в r2) -- //
                            // -- следующие два режима нужны для корректного расширения знака 16-битных операндов (в address) до 32-битных РОН -- //
        direct_mode = 1,    // -- непосредственная загрузка целого числа со знаком вместо адреса в регистр (в r2) -- //
        address_mode = 2,   // -- непосредственная загрузка константы-адреса (беззнаковое число) в регистр (в r2) -- //
    };
    const uint8_t mode = cpu.get_cmd_r1();
    const uint8_t r2_i = cpu.get_cmd_r2();
    const uint16_t address = cpu.get_cmd_address();
    switch (mode)
    {
    case value_mode:
        cpu.r[r2_i].u = cpu.ram.get_word(address).data.u;
        break;
    case direct_mode:
        cpu.r[r2_i].i = int16_t(address);  // -- беру целочисленное значение из команды и помещаю его в регистр -- //
        break;
    case address_mode:
        cpu.r[r2_i].u = address;
        break;
    }
}

void save::operator()(CPU &cpu) noexcept    // -- пересылка регистр (r1) - память [16][32], от длины команды зависит способ получения адреса -- //
{
    const uint8_t t = cpu.get_cmd_t(); // -- получаю бит длины команды -- //
    const uint8_t r1_i = cpu.get_cmd_r1(); // -- получаю номер регистра с данными -- //
    const word data = {cpu.r[r1_i]};    // -- получаю данные из 1-го регистра -- //
    uint16_t address = 0;   // -- адрес, по которому сохраняются данные из 1-го регистра -- //
    if (t == 0) // -- если команда короткая -- //
    {
        const uint8_t r2_i = cpu.get_cmd_r2();  // -- получаю номер регистра с адресом -- //
        address = uint16_t(cpu.r[r2_i].u);   // -- получаю адрес из 2-го регистра -- //
    }
    else    // -- если команда длинная -- //
    {
        address = cpu.get_cmd_address();    // -- получаю адрес из команды -- //
    }
    cpu.ram.push(data,address);
}

void dereference_ptr::operator()(CPU &cpu) noexcept
{
    const uint8_t t = cpu.get_cmd_t(); // -- получаю бит длины команды -- //
    const uint8_t r1_i = cpu.get_cmd_r1(); // -- получаю номер регистра с адресом -- //
    const uint16_t ptr = uint16_t(cpu.r[r1_i].u);
    const word word = cpu.ram.get_word(ptr);
    if (t == 0) // -- если команда короткая, то сохраняю значение во 2-й регистр -- //
    {
        const uint8_t r2_i = cpu.get_cmd_r2();
        cpu.r[r2_i] = word.data;
    }
    else    // -- если команда длинная, то сохраняю значение по адресу команды -- //
    {
        const uint16_t address = cpu.get_cmd_address();
        cpu.ram.push(word,address);
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
    const uint8_t t = cpu.get_cmd_t(); // -- получаю бит длины команды -- //
    const uint8_t r1_i = cpu.get_cmd_r1(); // -- получаю номера регистров -- //
    const uint8_t r2_i = cpu.get_cmd_r2();
    if (t == 0) // -- если команда короткая (двухадресная арифметика) -- //
    {
        cpu.r[r2_i].i = calculate(cpu.r[r1_i].i, cpu.r[r2_i].i);    // -- помещаю результат во второй операнд (регистр) -- //
    }
    else    // -- если команда длинная (трехадресная арифметика) -- //
    {
        const uint16_t base_address = uint16_t(cpu.r[r1_i].u);    // -- базовый адрес в первом регистре -- //
        const uint16_t offset = cpu.get_cmd_address();    // -- смещение в команде -- //
        const uint16_t address = base_address + offset;
        int32_t value = cpu.ram.get_word(address).data.i;
        cpu.r[r2_i].i = calculate(value, cpu.r[r2_i].i);
    }
    set_flags(cpu); // -- устанавливаю флаги -- //
}
// -- КОНЕЦ КОМАНДЫ ЦЕЛОЙ АРИФМЕТИКИ -- //

// -- КОМАНДЫ ДРОБНОЙ АРИФМЕТИКИ -- //
void fMath::set_flags(CPU &cpu) noexcept
{
    const uint8_t r_i = cpu.get_cmd_r2();   // -- результат помещается во 2-й операнд (регистр), поэтому получаю номер этого регистра -- //
    if (fabsf(cpu.r[r_i].f) <= std::numeric_limits<float>::epsilon()) cpu.PSW.FLAGS.ZF = 1;    // -- если результат = 0, то флаг ZF = 1 -- //
    else cpu.PSW.FLAGS.ZF = 0;  // -- иначе ZF = 0 -- //

    if (cpu.r[r_i].f < 0) cpu.PSW.FLAGS.SF = 1; // -- если результат < 0, то флаг SF = 1 -- //
    else cpu.PSW.FLAGS.SF = 0;  // -- иначе SF = 0 -- //
}

void fMath::operator()(CPU &cpu) noexcept
{
    const uint8_t t = cpu.get_cmd_t(); // -- получаю бит длины команды -- //
    const uint8_t r1_i = cpu.get_cmd_r1(); // -- получаю номера регистров -- //
    const uint8_t r2_i = cpu.get_cmd_r2();
    if (t == 0) // -- если команда короткая (двухадресная арифметика) -- //
    {
        cpu.r[r2_i].f = calculate(cpu.r[r1_i].f, cpu.r[r2_i].f);    // -- помещаю результат во второй операнд (регистр) -- //
    }
    else    // -- если команда длинная (трехадресная арифметика) -- //
    {
        const uint16_t base_address = uint16_t(cpu.r[r1_i].u);    // -- базовый адрес в первом регистре -- //
        const uint16_t offset = cpu.get_cmd_address();    // -- смещение в команде -- //
        const uint16_t address = base_address + offset;
        float value = cpu.ram.get_word(address).data.f;
        cpu.r[r2_i].f = calculate(value, cpu.r[r2_i].f);
    }
    set_flags(cpu); // -- устанавливаю флаги -- //
}
// -- КОНЕЦ КОМАНДЫ ДРОБНОЙ АРИФМЕТИКИ -- //

// -- КОМАНДЫ ВВОДА И ВЫВОДА -- //
void io::operator()(CPU &cpu) noexcept
{
    enum io_mode {      // -- значение во 2-м регистре команды является режимом ввода-вывода, здесь перечислены все режимы -- //
        in_int = 0,     // -- ввод целого числа -- //
        in_uint = 1,    // -- ввод беззнакового числа -- //
        in_float = 2,   // -- ввод дробного числа -- //
        out_int = 3,    // -- вывод целого числа -- //
        out_uint = 4,   // -- вывод беззнакового числа -- //
        out_float = 5   // -- вывод дробного числа -- //
    };

    const uint8_t t = cpu.get_cmd_t();  // -- получаю бит длины команды -- //
    const uint8_t r1_i = cpu.get_cmd_r1(); // -- получаю номер регистра с результатом -- //
    const uint8_t mode = cpu.get_cmd_r2(); // -- получаю режим ввода-вывода -- //
    const uint16_t address = cpu.get_cmd_address(); // -- получаю адрес, находящийся в команде -- //


    switch (mode)
    {
    case in_int:
        std::cout << std::endl << "input int: ";
        if (t == 0) // -- если команда короткая -- //
        {
            std::cin >> cpu.r[r1_i].i;
        }
        else    // -- если команда длинная -- //
        {
            word data;
            std::cin >> data.data.i;
            cpu.ram.push(data,address);
        }
        break;
    case in_uint:
        std::cout << std::endl << "input unsigned int: ";
        if (t == 0) // -- если команда короткая -- //
        {
            std::cin >> cpu.r[r1_i].u;
        }
        else    // -- если команда длинная -- //
        {
            word data;
            std::cin >> data.data.u;
            cpu.ram.push(data,address);
        }
        break;
    case in_float:
        std::cout << std::endl << "input float: ";
        if (t == 0) // -- если команда короткая -- //
        {
            std::cin >> cpu.r[r1_i].f;
        }
        else    // -- если команда длинная -- //
        {
            word data;
            std::cin >> data.data.f;
            cpu.ram.push(data,address);
        }
        break;
    case out_int:
        std::cout << std::endl << "int: ";
        if (t == 0) // -- если команда короткая -- //
        {
            std::cout << cpu.r[r1_i].i << std::endl;
        }
        else    // -- если команда длинная -- //
        {
            std::cout << cpu.ram.get_word(address).data.i << std::endl;
        }
        break;
    case out_uint:
        std::cout << std::endl << "unsigned int: ";
        if (t == 0) // -- если команда короткая -- //
        {
            std::cout << cpu.r[r1_i].u << std::endl;
        }
        else    // -- если команда длинная -- //
        {
            std::cout << cpu.ram.get_word(address).data.u << std::endl;
        }
        break;
    case out_float:
        std::cout << std::endl << "float: " << std::fixed;
        if (t == 0) // -- если команда короткая -- //
        {
            std::cout << cpu.r[r1_i].f << std::endl;
        }
        else    // -- если команда длинная -- //
        {
            std::cout << cpu.ram.get_word(address).data.f << std::endl;
        }
        break;
    }
}
// -- КОНЕЦ КОМАНДЫ ВВОДА И ВЫВОДА -- //

// -- КОМАНДЫ ПЕРЕХОДОВ -- //

void Jump::operator()(CPU &cpu) noexcept
{
    call_go_to(cpu);
}

void Jump::go_to(CPU &cpu) noexcept
{
    enum jmp_mode {  // -- значение во 1-м регистре команды является режимом перехода, здесь перечислены все режимы -- //
        direct_mode = 0,            // -- прямой переход -- //
        offset_plus_mode = 1,       // -- относительный переход вперед -- //
        offset_minus_mode = 2      // -- относительный переход назад -- //
    };

    const uint8_t t = cpu.get_cmd_t();  // -- получаю бит длины команды -- //
    uint16_t address = cpu.PSW.IP; // -- адрес, для операции go_to, изначально пусть адрес равен IP -- //
    if (t == 0) // -- если команда короткая - значит косвенный переход -- //
    {
        const uint8_t r2_i = cpu.get_cmd_r2();  // -- получаю номер 2-ого регистра -- //
        address = uint16_t(cpu.r[r2_i].u);
    }
    else    // -- если команда длинная -- //
    {
        const uint8_t mode = cpu.get_cmd_r1();  // -- получаю режим перехода -- //
        const uint16_t cmd_address = cpu.get_cmd_address(); // -- получаю адрес из команды -- //
        switch (mode)
        {
        case direct_mode:
            address = cmd_address;
            break;
        case offset_plus_mode:
            address += cmd_address;
            break;
        case offset_minus_mode:
            address -= cmd_address;
            break;
        }
    }
    cpu.PSW.IP = address;
    set_was_transition(cpu);
}

void jmp::call_go_to(CPU &cpu) noexcept
{
    go_to(cpu);
}

void jzf::call_go_to(CPU &cpu) noexcept
{
    if (cpu.PSW.FLAGS.ZF) go_to(cpu);
}
void jnzf::call_go_to(CPU &cpu) noexcept
{
    if (!cpu.PSW.FLAGS.ZF) go_to(cpu);
}
void jsf::call_go_to(CPU &cpu) noexcept
{
    if (cpu.PSW.FLAGS.SF) go_to(cpu);
}
void jnsf::call_go_to(CPU &cpu) noexcept
{
    if (!cpu.PSW.FLAGS.SF) go_to(cpu);
}
void call::call_go_to(CPU &cpu) noexcept
{
    cpu.r[15].u = cpu.PSW.IP;
    go_to(cpu);
}
void ret::call_go_to(CPU &cpu) noexcept
{
    cpu.PSW.IP = uint16_t(cpu.r[15].u);
}

// -- КОНЕЦ КОМАНДЫ ПЕРЕХОДОВ -- //



