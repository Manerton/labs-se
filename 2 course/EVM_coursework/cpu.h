#ifndef CPU_H
#define CPU_H

#include <cinttypes>
#include "types.h"
#include "memory.h"

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
        } FLAGS;

        uint16_t IP = 0;

    };

    cmd cmd;

public:
    CPU();
    PSW PSW;
};

#endif // CPU_H
