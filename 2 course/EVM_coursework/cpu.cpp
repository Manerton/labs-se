#include <iostream>
#include "cpu.h"
#include "command.h"


CPU::CPU()
{
// -- особые команды -- //
    command[stop] = nullptr;
    command[nop] = new class nop();

// -- команды пересылки данных -- //
    command[exchange] = new class exchange();
    command[move] =  new class move();
    command[load] = new class load();
    command[save] = new class save();
    command[dereference_ptr] = new class dereference_ptr();

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
    command[io] = new class io();

// -- команды перехода -- //
    command[jmp] = new class jmp();
    command[jzf] = new class jzf();
    command[jnzf] = new class jnzf();
    command[jsf] = new class jsf();
    command[jnsf] = new class jnsf();
    command[call] = new class call();
    command[ret] = new class ret();

}

CPU::~CPU()
{
    for (Command* &ptr : command)
    {
        delete ptr;
        ptr = nullptr;
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
        if (!was_transition) ++PSW.IP;
        else was_transition = false;
        loadCommand(); // -- загрузка следующей команды из памяти -- //
    }
}
