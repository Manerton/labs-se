#include "cpu.h"
#include <iostream>

using std::make_unique;
CPU::CPU()
{
// -- команды пересылки данных -- //
    command[exchange] = make_unique<class exchange>();
    command[move]   = make_unique<class move>();
    command[sign_value_load] = make_unique<class sign_value_load>();
    command[unsign_value_load] = make_unique<class unsign_value_load>();
    command[direct_load] = make_unique<class direct_load>();
    command[save] = make_unique<class save>();
    command[save_pop] = make_unique<class save_pop>();
    command[dereference_ptr] = make_unique<class dereference_ptr>();

// -- команды целой арифметики -- //
    command[iAdd]   = make_unique<class iAdd>();
    command[iSub]   = make_unique<class iSub>();
    command[iMul]   = make_unique<class iMul>();
    command[iDiv]   = make_unique<class iDiv>();

// -- команды дробной арифметики -- //
    command[fAdd]   = make_unique<class fAdd>();
    command[fSub]   = make_unique<class fSub>();
    command[fMul]   = make_unique<class fMul>();
    command[fDiv]   = make_unique<class fDiv>();

// -- команды ввода и вывода -- //
    command[st_in_int]      = make_unique<class st_in_int>();
    command[st_in_float]    = make_unique<class st_in_float>();
    command[st_out_int]     = make_unique<class st_out_int>();
    command[st_out_uint]    = make_unique<class st_out_uint>();
    command[st_out_float]   = make_unique<class st_out_float>();
    command[mem_in_int]     = make_unique<class mem_in_int>();
    command[mem_in_float]   = make_unique<class mem_in_float>();
    command[mem_out_int]    = make_unique<class mem_out_int>();
    command[mem_out_uint]   = make_unique<class mem_out_uint>();
    command[mem_out_float]  = make_unique<class mem_out_float>();
// -- команды перехода -- //
    command[direct_jmp]         = make_unique<class direct_jmp>();
    command[offset_plus_jmp]    = make_unique<class offset_plus_jmp>();
    command[offset_minus_jmp]   = make_unique<class offset_minus_jmp>();
    command[jzf]    = make_unique<class jzf>();
    command[jnzf]   = make_unique<class jnzf>();
    command[jsf]    = make_unique<class jsf>();
    command[jnsf]   = make_unique<class jnsf>();
    command[call]   = make_unique<class call>();
    command[ret]    = make_unique<class ret>();
}

CPU::~CPU() = default;

void CPU::run() noexcept
{
    loadCommand(); // -- загружаем из памяти команду -- //
    while (cmd.c.cop != stop) // -- если еще не встретили операцию "стоп" -- //
    {
        PSW.IP += VM_types::cmd_length;
        command[cmd.c.cop]->operator()(*this); // -- выполняем команду -- //
        loadCommand(); // -- загрузка следующей команды из памяти -- //
    }
}
