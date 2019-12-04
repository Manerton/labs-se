#ifndef CPU_H
#define CPU_H

#include <cinttypes>
#include "types.h"
#include "memory.h"

enum class COP
{
    stop = 0, // -- команда остановки -- //
    empty = 1, // -- пустая команда-заглушка -- //
// -- команды пересылки данных -- //
    exchange = 2, // -- обменять содержимое регистров -- //
    move = 3, // -- пересылка <регистр источник> <регистр назначения> -- //
    load = 4, // -- пересылка память - регистр (в r2) -- //
    save = 5, // -- пересылка регистр (r1) - память -- //
    loadaddress = 6, // -- загрузка константы-адреса в регистр -- //
    // -- продумать еще команды для разных видов адресаций -- //
// -- команды целой арифметики -- //
    inc = 7,
    add = 8,
    dec = 9,
    sub = 10,
    mul = 11,
    div = 12,
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
    jmp = 25,
    jmp_offset = 26,
    jmp_ptr = 27,
    je = 28, // -- jump if equal, ZF = 1 -- //
    jne = 29, // -- jump if not equal, ZF = 0 -- //

    call = ,
    ret = ,

// -- команды управления состоянием ЦП -- //
};

class CPU
{
    struct PSW
    {
        struct FLAGS
        {
            uint16_t ZF : 1;        // флаг нуля
            uint16_t SF : 1;        // флаг знака
            uint16_t ZDF : 1;       // флаг деления на ноль
            uint16_t NanF : 1;      // флаг - результат не является числом
            uint16_t PINF : 1;      // флаг - результат +бесконечность
            uint16_t NINF : 1;      // флаг - результат -бесконечность
            uint16_t reserved : 10;  // в резерве
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

};

#endif // CPU_H
