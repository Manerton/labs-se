#ifndef CPU_H
#define CPU_H

#include <cinttypes>
#include "types.h"
#include "memory.h"

enum class COP
{
    stop = 0, // -- команда остановки [32] -- //
    empty = 1, // -- пустая команда-заглушка [16] -- //
// -- команды пересылки данных -- //
    exchange = 2, // -- обменять содержимое регистров [16] -- //
    move = 3, // -- пересылка <регистр источник> <регистр назначения> [16] -- //
    load = 4, // -- пересылка память - регистр (в r2) [32] -- //
    save = 5, // -- пересылка регистр (r1) - память [32] -- //
    loadaddress = 6, // -- загрузка константы-адреса в регистр [32] -- //
    // -- продумать еще команды для разных видов адресаций -- //
// -- команды целой арифметики -- //
    inc = 7, // -- инкремент -- //
    add = 8, // -- сложение -- //
    dec = 9, // -- декремент -- //
    sub = 10,   // -- вычитание -- //
    mul = 11,   // -- умножение -- //
    div = 12,   // -- деление -- //
// -- команды дробной арифметики -- //
    finc = 13,
    fadd = 14,
    fdec = 15,
    fsub = 16,
    fmul = 17,
    fdiv = 18,
// -- команды ввода и вывода -- //
    in_int = 19,
    out_int = 20,
    in_uint = 21,
    out_uint = 22,
    in_float = 23,
    out_float = 24,
// -- команды перехода -- //
    jmp = 25,   // -- прямой безусловный переход -- //
    jmp_offset = 26,    // -- относительный безусловный переход -- //
    jmp_ptr = 27,   // -- косвенный безусловный переход -- //
    jzf = 28, // -- jump if ZF = 1 -- //
    jnzf = 29, // -- jump if ZF = 0 -- //
    jsf = 30,   // -- jump if SF = 1 -- //
    jnsf = 31,  // -- jump if SF = 0 -- //
    call = 32,  // -- call procedure -- //
    ret = 33,   // -- return from procedure -- //
};

class CPU
{
    struct PSW
    {
        struct FLAGS
        {
            uint16_t ZF : 1;        // флаг нуля
            uint16_t SF : 1;        // флаг знака
            uint16_t reserved : 14;  // в резерве
        };

        uint16_t IP = 0;
        FLAGS FLAGS;

    };

    cmd cmd;

public:
    PSW PSW;      // -- PSW = IP + Flags -- //
    Memory ram;   // -- память, слова - 32 бита, размер адреса 16 бит -- //
    data_t r[16]; // -- регистры общего назначения -- //

    CPU();
    ~CPU();

    CPU(const CPU &) = delete;
    CPU(const CPU &&) = delete;
    CPU& operator=(const CPU &)  = delete;
    CPU& operator=(const CPU &&) = delete;

    uint8_t get_cmd_t() const noexcept { return cmd.c32.t; }
    uint8_t get_cmd_cop() const noexcept { return cmd.c32.
};

#endif // CPU_H
