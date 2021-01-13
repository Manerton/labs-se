#include "cpu.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iomanip>

using std::make_unique;
using VM_types::AccessMode;
using namespace std;

CPU::CPU(bool debug)
{
    reset(); // обнулить флаги
    PSW.TF = debug;
    const auto None = AccessMode::None;
    const auto Read = AccessMode::Read;
    const auto Write = AccessMode::Write;
    commandTable[stop]              = {None,  make_unique<class stop>()};
// -- команды пересылки данных -- //
    commandTable[exchange]          = {None,  make_unique<class exchange>()};
    commandTable[move]              = {None,  make_unique<class move>()};
    commandTable[sign_value_load]   = {Read,  make_unique<class sign_value_load>()};
    commandTable[unsign_value_load] = {Read,  make_unique<class unsign_value_load>()};
    commandTable[direct_load]       = {Read,  make_unique<class direct_load>()};
    commandTable[save]              = {Write, make_unique<class save>()};
    commandTable[save_pop]          = {Write, make_unique<class save_pop>()};
    commandTable[dereference_ptr]   = {Write, make_unique<class dereference_ptr>()};

// -- команды целой арифметики -- //
    commandTable[iAdd]   = {Read, make_unique<class iAdd>()};
    commandTable[iSub]   = {Read, make_unique<class iSub>()};
    commandTable[iMul]   = {Read, make_unique<class iMul>()};
    commandTable[iDiv]   = {Read, make_unique<class iDiv>()};

// -- команды дробной арифметики -- //
    commandTable[fAdd]   = {Read, make_unique<class fAdd>()};
    commandTable[fSub]   = {Read, make_unique<class fSub>()};
    commandTable[fMul]   = {Read, make_unique<class fMul>()};
    commandTable[fDiv]   = {Read, make_unique<class fDiv>()};

// -- команды ввода и вывода -- //
    commandTable[st_in_int]      = {None, make_unique<class st_in_int>()};
    commandTable[st_in_float]    = {None, make_unique<class st_in_float>()};
    commandTable[st_out_int]     = {None, make_unique<class st_out_int>()};
    commandTable[st_out_uint]    = {None, make_unique<class st_out_uint>()};
    commandTable[st_out_float]   = {None, make_unique<class st_out_float>()};
    commandTable[mem_in_int]     = {Write, make_unique<class mem_in_int>()};
    commandTable[mem_in_float]   = {Write, make_unique<class mem_in_float>()};
    commandTable[mem_out_int]    = {Read, make_unique<class mem_out_int>()};
    commandTable[mem_out_uint]   = {Read, make_unique<class mem_out_uint>()};
    commandTable[mem_out_float]  = {Read, make_unique<class mem_out_float>()};
// -- команды перехода -- //
    commandTable[direct_jmp]        = {None, make_unique<class direct_jmp>()};
    commandTable[offset_plus_jmp]   = {None, make_unique<class offset_plus_jmp>()};
    commandTable[offset_minus_jmp]  = {None, make_unique<class offset_minus_jmp>()};
    commandTable[jzf]               = {None, make_unique<class jzf>()};
    commandTable[jnzf]              = {None, make_unique<class jnzf>()};
    commandTable[jsf]               = {None, make_unique<class jsf>()};
    commandTable[jnsf]              = {None, make_unique<class jnsf>()};
    commandTable[call]              = {None, make_unique<class call>()};
    commandTable[ret]               = {None, make_unique<class ret>()};
    commandTable[indirect_jmp]      = {Read, make_unique<class indirect_jmp>()};
}

CPU::~CPU() = default;

std::string CPU::trace() const noexcept
{
    stringstream ss;
    ss << "[Trace] IP: " << setfill('0') << setw(5) << PSW.IP << ", SP: " << setw(2) << PSW.SP;
    //ss << "[Flags] " << "SF: " << PSW.SF << ", ZF: " << PSW.ZF << ", TF: " << PSW.TF << endl;
    ss << " -> CMD: " << setw(3) << int(cmd.c.cop) << " " << setw(5) << cmd.c.address << endl;
    return ss.str();
}

bool CPU::reachBP()
{
    if (PSW.BPF == 0) return false;
    auto uslovie = [&](const auto& bp) -> bool
    {
        const auto BP_mode = bp.mode;
        const auto cmd_mode = commandTable[cmd.c.cop].mode;
        // если точка прерывания активирована
        if (BP_mode != AccessMode::None)
        {
            const bool readCheck = (BP_mode == AccessMode::Read &&
                              (cmd_mode == AccessMode::Read || cmd_mode == AccessMode::Write)
                                    && bp.address == get_cmd_address());
            const bool writeCheck = (BP_mode == AccessMode::Write && cmd_mode == AccessMode::Write
                                     && bp.address == get_cmd_address());
            const bool execCheck = (BP_mode == AccessMode::Exec && bp.address == PSW.IP);

            if (readCheck || writeCheck || execCheck) return true;
        }
        return false;
    };
    return std::any_of(BP.begin(), BP.end(), uslovie);
}

void CPU::run()
{
    bool working = true;
    while (!PSW.EF && working) // -- пока еще не конец программы -- //
    {
        // если стоит флаг ловушки (трассировки), то прерываем работу после этой команды
        // это для покомандного выполнения
        working = !PSW.TF;
        // загружаем из памяти команду
        loadCommand();
        // если не наткнулись на точку прерывания
        if (!reachBP())
        {
            // сохраняем в лог трассировку
            traceLog.push_back(trace());
            PSW.IP += VM_types::cmd_length;
            if (commandTable[cmd.c.cop].cmd_ptr)
            {
                commandTable[cmd.c.cop].cmd_ptr->operator()(*this); // -- выполняем команду -- //
            }
            else throw std::invalid_argument("wrong opcode");
        } else working = false;
    }
}
