#include "cpu.h"
#include "command.h"

CPU::CPU()
{
    // -- создать все команды -- //
// -- особые команды -- //
    command[stop] = nullptr;
    command[empty] = new class empty();

// -- команды пересылки данных -- //
    command[exchange] = new class exchange();
    command[move] =  new class move();
    command[load] = new class load();
    command[direct_int_load] = new class direct_int_load();
    command[save] = new class save();
    command[load_address] = new class load_address();
    command[indirect_save] = new class indirect_save();
    command[dereference_ptr] = new class dereference_ptr();
    // -- продумать еще команды для разных видов адресаций -- //

// -- команды целой арифметики -- //
    command[iAdd] = new class iAdd();
    command[iSub] = new class iSub();
    command[iMul] = new class iMul();
    command[iDiv] = new class iDiv();

// -- команды дробной арифметики -- //
    command[fAdd] = new class fAdd();
    command[fSub] = new class fSub();
    command[fMul] = new class fMul();
    command[fDiv] = new class fDiv();

// -- команды ввода и вывода -- //
    //command[in_int] = new
    command[out_int] = new class out_int();
    //command[in_uint] = new
    command[out_uint] = new class out_uint();
    //command[in_float] = new
    command[out_float] = new class out_float();

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
    for (Command* ptr : command)
    {
        delete ptr;
    }
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
