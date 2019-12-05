#include "cpu.h"
#include "command.h"

CPU::CPU()
{
    // -- создать все команды -- //
    command[stop] = nullptr;
    command[empty] = new class empty();

// -- команды пересылки данных -- //
    //command[exchange] = new
    //command[move] =  new
    command[load] = new class load();
    command[save] = new class save();
    //command[load_address] = new
    // -- продумать еще команды для разных видов адресаций -- //
// -- команды целой арифметики -- //
    command[iAdd] = new class iAdd();
    //command[sub] = new
    //command[mul] = new
    //command[div] = new
// -- команды дробной арифметики -- //
    //command[fadd] = new
    //command[fsub] = new
    //command[fmul] = new
    //command[fdiv] = new
// -- команды ввода и вывода -- //
    //command[in_int] = new
    command[out_int] = new class out_int();
    //command[in_uint] = new
    //command[out_uint] = new
    //command[in_float] = new
    //command[out_float] = new
// -- команды перехода -- //
    //command[jmp] = new
    //command[jmp_offset] = new
    //command[jmp_ptr] = new
    //command[jzf] = new
    //command[jnzf = new
    //command[jsf] = new
    //command[jnsf] = new
    //command[call] = new
    //command[ret] = new

}

CPU::~CPU()
{
    delete command[load];
    delete command[save];
    delete command[iAdd];
    delete command[out_int];
}

void CPU::run() noexcept
{
    loadCommand(); // -- загружаем из памяти команду -- //
    while (cmd.c32.cop != stop) // -- если еще не встретили операцию "стоп" -- //
    {
        command[cmd.c32.cop]->operator()(*this); // -- выполняем 32-битную команду или первую 16-битную -- //
        if (cmd.c32.t == 0) // -- если текущая команда в слове короткая -- //
        {
            cmd.c16[0] = cmd.c16[1];
            command[cmd.c32.cop] -> operator()(*this); // -- выполняем вторую 16-битную команду -- //
        }
        ++PSW.IP;
        loadCommand(); // -- загрузка следующей команды из памяти -- //
    }
}
