#include <iostream>
#include <cmath>
#include <limits> // для numeric_limits<float>::epsilon(), чтобы сравнивать результат дробных вычислений с 0
#include "cpu.h"
#include "command.h"

using VM_types::cmd_t;
using VM_types::data_t;
using VM_types::data_length;
using VM_types::cmd_length;

// -- КОМАНДЫ ПЕРЕСЫЛКИ ДАННЫХ -- //
void exchange::operator()(CPU &cpu) noexcept // -- обмен значений ST[SP] и ST[SP-i] -- //
{
    const uint16_t diff = cpu.get_cmd_address();
    const size_t SP = cpu.PSW.SP;
    const size_t i = SP - diff;
    std::swap(cpu.ST[SP].u,cpu.ST[i].u);
}

void move::operator()(CPU &cpu) noexcept // -- пересылка ST[SP] -> ST[SP-i] -- //
{
    const uint16_t diff = cpu.get_cmd_address();
    const size_t SP = cpu.PSW.SP;
    const size_t i = SP - diff;
    cpu.ST[i].u = cpu.ST[SP].u;
}

void sign_value_load::operator()(CPU &cpu) noexcept // -- пересылка память - стек -- //
{
    const int16_t value = int16_t(cpu.get_cmd_address());
    const size_t SP = ++cpu.PSW.SP;
    cpu.ST[SP].i = value;  // -- беру целочисленное значение из команды и помещаю его в стек -- //
}

void unsign_value_load::operator()(CPU &cpu) noexcept // -- пересылка память - стек -- //
{
    const uint16_t value = cpu.get_cmd_address();
    const size_t SP = ++cpu.PSW.SP;
    cpu.ST[SP].u = value;  // -- беру значение из команды и помещаю его в стек -- //
}

void direct_load::operator()(CPU &cpu) noexcept // -- пересылка память - стек -- //
{
    const uint16_t address = cpu.get_cmd_address();
    const size_t SP = ++cpu.PSW.SP;
    cpu.ST[SP].u = cpu.ram.get_data(address).u;
}

void save::operator()(CPU &cpu) noexcept // -- пересылка стек -> память (вершина не удаляется) -- //
{
    const data_t data = {cpu.ST[cpu.PSW.SP]};   // -- получаю данные из вершины стека -- //
    const uint16_t address = cpu.get_cmd_address();   // -- адрес, по которому сохраняются данные из стека -- //
    cpu.ram.push(data,address);
}

void save_pop::operator()(CPU &cpu) noexcept // -- пересылка стек -> память (вершина удаляется) -- //
{
    const data_t data = {cpu.ST[cpu.PSW.SP]};   // -- получаю данные из вершины стека -- //
    const uint16_t address = cpu.get_cmd_address();   // -- адрес, по которому сохраняются данные из стека -- //
    cpu.ram.push(data,address);
    --cpu.PSW.SP;
}

void dereference_ptr::operator()(CPU &cpu) noexcept // -- получить значение по адресу, лежащему в стеке -- //
{
    const uint16_t ptr = uint16_t(cpu.ST[cpu.PSW.SP].u);
    const data_t data = cpu.ram.get_data(ptr);
    const uint16_t address = cpu.get_cmd_address();
    cpu.ram.push(data,address);
    --cpu.PSW.SP;
}
// -- КОНЕЦ КОМАНДЫ ПЕРЕСЫЛКИ ДАННЫХ -- //

// -- КОМАНДЫ ЦЕЛОЙ АРИФМЕТИКИ -- //
void iMath::set_flags(CPU &cpu) noexcept
{
    const size_t SP = cpu.PSW.SP;
    if (cpu.ST[SP].i == 0) cpu.PSW.ZF = 1;    // -- если результат = 0, то флаг ZF = 1 -- //
    else cpu.PSW.ZF = 0;  // -- иначе ZF = 0 -- //

    if (cpu.ST[SP].i < 0) cpu.PSW.SF = 1; // -- если результат < 0, то флаг SF = 1 -- //
    else cpu.PSW.SF = 0;  // -- иначе SF = 0 -- //
}

void iMath::operator()(CPU &cpu) noexcept
{
    const size_t SP = cpu.PSW.SP;
    const uint16_t address = cpu.get_cmd_address();
    const data_t data = cpu.ram.get_data(address);
    cpu.ST[SP].i = calculate(cpu.ST[SP].i, data.i);
    set_flags(cpu); // -- устанавливаю флаги -- //
}
// -- КОНЕЦ КОМАНДЫ ЦЕЛОЙ АРИФМЕТИКИ -- //

// -- КОМАНДЫ ДРОБНОЙ АРИФМЕТИКИ -- //
void fMath::set_flags(CPU &cpu) noexcept
{
    const size_t SP = cpu.PSW.SP;  // -- результат помещается в стек в вершину стека -- //
    if (fabsf(cpu.ST[SP].f) <= std::numeric_limits<float>::epsilon()) cpu.PSW.ZF = 1;    // -- если результат = 0, то флаг ZF = 1 -- //
    else cpu.PSW.ZF = 0;  // -- иначе ZF = 0 -- //

    if (cpu.ST[SP].f < 0) cpu.PSW.SF = 1; // -- если результат < 0, то флаг SF = 1 -- //
    else cpu.PSW.SF = 0;  // -- иначе SF = 0 -- //
}

void fMath::operator()(CPU &cpu) noexcept
{
    const size_t SP = cpu.PSW.SP;
    const uint16_t address = cpu.get_cmd_address();
    const data_t data = cpu.ram.get_data(address);
    cpu.ST[SP].f = calculate(cpu.ST[SP].f, data.f);
    set_flags(cpu); // -- устанавливаю флаги -- //
}
// -- КОНЕЦ КОМАНДЫ ДРОБНОЙ АРИФМЕТИКИ -- //

// -- КОМАНДЫ ВВОДА И ВЫВОДА -- //
void IO::st_io(CPU &cpu, IO::io_mode mode) noexcept
{
    const size_t SP = cpu.PSW.SP; // -- получаю индекс вершины стека -- //
    switch (mode)
    {
    case io_mode::in_int:
        std::cout << std::endl << "input int: ";
        std::cin >> cpu.ST[++cpu.PSW.SP].u;
        break;
    case io_mode::in_float:
        std::cout << std::endl << "input float: ";
        std::cin >> cpu.ST[++cpu.PSW.SP].f;
        break;
    case io_mode::out_int:
        std::cout << std::endl << "int: ";
        std::cout << cpu.ST[SP].i << std::endl;
        break;
    case io_mode::out_uint:
        std::cout << std::endl << "unsigned int: ";
        std::cout << cpu.ST[SP].u << std::endl;
        break;
    case io_mode::out_float:
        std::cout << std::endl << "float: ";
        std::cout << cpu.ST[SP].f << std::endl;
        break;
    }
}

void IO::mem_io(CPU &cpu, IO::io_mode mode) noexcept
{
    const uint16_t address = cpu.get_cmd_address(); // -- получаю адрес, находящийся в команде -- //
    data_t tmp;
    switch (mode)
    {
    case io_mode::in_int:
        std::cout << std::endl << "input int: ";
        std::cin >> tmp.u;
        cpu.ram.push(tmp, address);
        break;
    case io_mode::in_float:
        std::cout << std::endl << "input float: ";
        std::cin >> tmp.f;
        cpu.ram.push(tmp, address);
        break;
    case io_mode::out_int:
        std::cout << std::endl << "int: ";
        tmp = cpu.ram.get_data(address);
        std::cout << tmp.i << std::endl;
        break;
    case io_mode::out_uint:
        std::cout << std::endl << "unsigned int: ";
        tmp = cpu.ram.get_data(address);
        std::cout << tmp.u << std::endl;
        break;
    case io_mode::out_float:
        std::cout << std::endl << "float: ";
        tmp = cpu.ram.get_data(address);
        std::cout << tmp.f << std::endl;
        break;
    }
}

void IO::operator()(CPU &cpu) noexcept
{
    call_io(cpu);
}

void st_in_int::call_io(CPU &cpu) noexcept
{
    st_io(cpu,io_mode::in_int);
}
void st_in_float::call_io(CPU &cpu) noexcept
{
    st_io(cpu,io_mode::in_float);
}
void st_out_int::call_io(CPU &cpu) noexcept
{
    st_io(cpu,io_mode::out_int);
}
void st_out_uint::call_io(CPU &cpu) noexcept
{
    st_io(cpu,io_mode::out_uint);
}
void st_out_float::call_io(CPU &cpu) noexcept
{
    st_io(cpu,io_mode::out_float);
}
void mem_in_int::call_io(CPU &cpu) noexcept
{
    mem_io(cpu,io_mode::in_int);
}
void mem_in_float::call_io(CPU &cpu) noexcept
{
    mem_io(cpu,io_mode::in_float);
}
void mem_out_int::call_io(CPU &cpu) noexcept
{
    mem_io(cpu,io_mode::out_int);
}
void mem_out_uint::call_io(CPU &cpu) noexcept
{
    mem_io(cpu,io_mode::out_uint);
}
void mem_out_float::call_io(CPU &cpu) noexcept
{
    mem_io(cpu,io_mode::out_float);
}

// -- КОНЕЦ КОМАНДЫ ВВОДА И ВЫВОДА -- //

// -- КОМАНДЫ ПЕРЕХОДОВ -- //

void Jump::operator()(CPU &cpu) noexcept
{
    call_go_to(cpu);
}

void Jump::go_to(CPU &cpu, Jump::jmp_mode mode) noexcept
{
    /*  адрес, для операции go_to, изначально пусть адрес равен IP минус размер команды,
        вычитаем размер, так как в основном цикле CPU счетчик команд IP увеличивается еще до выполнения команды
        это сделано для того, чтобы в основном цикле не отслеживать, был ли переход */
    uint16_t newIP = cpu.PSW.IP - cmd_length;
    const uint16_t cmd_address = cpu.get_cmd_address(); // -- получаю адрес из команды -- //
    switch (mode)
    {
    case jmp_mode::direct_mode:
        newIP = cmd_address;
        break;
    case jmp_mode::offset_plus_mode:
        newIP += cmd_address;
        break;
    case jmp_mode::offset_minus_mode:
        newIP -= cmd_address;
        break;
    }
    cpu.PSW.IP = newIP;
}

void direct_jmp::call_go_to(CPU &cpu) noexcept
{
    go_to(cpu,jmp_mode::direct_mode);
}
void offset_plus_jmp::call_go_to(CPU &cpu) noexcept
{
    go_to(cpu,jmp_mode::offset_plus_mode);
}
void offset_minus_jmp::call_go_to(CPU &cpu) noexcept
{
    go_to(cpu,jmp_mode::offset_minus_mode);
}

void jzf::call_go_to(CPU &cpu) noexcept
{
    if (cpu.PSW.ZF) go_to(cpu);
}
void jnzf::call_go_to(CPU &cpu) noexcept
{
    if (!cpu.PSW.ZF) go_to(cpu);
}
void jsf::call_go_to(CPU &cpu) noexcept
{
    if (cpu.PSW.SF) go_to(cpu);
}
void jnsf::call_go_to(CPU &cpu) noexcept
{
    if (!cpu.PSW.SF) go_to(cpu);
}
void call::call_go_to(CPU &cpu) noexcept
{
    const size_t SP = ++cpu.PSW.SP;
    cpu.ST[SP].u = cpu.PSW.IP;
    go_to(cpu);
}
void ret::call_go_to(CPU &cpu) noexcept
{
    const size_t SP = cpu.PSW.SP;
    cpu.PSW.IP = uint16_t(cpu.ST[SP].u);
    --cpu.PSW.SP;
}
// -- КОНЕЦ КОМАНДЫ ПЕРЕХОДОВ -- //

