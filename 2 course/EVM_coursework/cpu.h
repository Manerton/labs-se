#ifndef CPU_H
#define CPU_H

#include <cinttypes>
class CPU
{
    struct PSW
    {
        struct FLAGS
        {
            uint16_t CF : 1;        // флаг переноса
            uint16_t PF : 1;        // флаг чётности
            uint16_t ZF : 1;        // флаг нуля
            uint16_t SF : 1;        // флаг знака
            uint16_t OF : 1;        // флаг переполнения
            uint16_t ZDF : 1;       // флаг деления на ноль
            uint16_t NanF : 1;      // флаг - результат не является числом
            uint16_t PINF : 1;      // флаг - результат +бесконечность
            uint16_t NINF : 1;      // флаг - результат -бесконечность
            uint16_t reserved : 7;  // в резерве
        };

        uint16_t IP = 0;
        FLAGS FLAGS;
    };

public:
    CPU();
    PSW PSW;
};

#endif // CPU_H
