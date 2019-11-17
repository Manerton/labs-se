#ifndef CPU_H
#define CPU_H

#include <cinttypes>

class CPU
{
    struct PSW {
        uint16_t IP = 0;
        struct FLAGS {
            uint16_t CF : 1;
            uint16_t PF : 1;
            uint16_t ZF : 1;
            uint16_t SF : 1;
            uint16_t OF : 1;
            uint16_t ZDF : 1;
            uint16_t NanF : 1;
            uint16_t PINF : 1;
            uint16_t NINF : 1;
        };
    };
public:
    CPU();
};

#endif // CPU_H
