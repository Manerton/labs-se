#ifndef CPU_H
#define CPU_H

#include <cinttypes>
#include "types.h"
#include "memory.h"

enum COP : uint8_t // -- uint8_t ограничивает размер enum -- //
{
    stop = 0, // -- команда остановки [32] -- //
    nop = 1, // -- пустая команда-заглушка [16] -- //
// -- команды пересылки данных -- //
    exchange = 2, // -- обменять содержимое регистров [16] -- //
    move = 3, // -- пересылка <регистр источник> <регистр назначения> [16] -- //
    load = 4, // -- пересылка память - регистр (в r2) [32] /!!!/ все режимы загрузки перечислены в enum load_mode в реализации операции load в файле "command.cpp" -- ///
    save = 5,  // -- пересылка регистр (r1) - память [16][32], от длины команды зависит способ получения адреса -- //
    dereference_ptr = 6, // -- разыменовать указатель (т.е получить значение по адресу, лежащему в регистре r1) [16][32] -- //
// -- команды целой арифметики -- //
    iAdd = 7, // -- сложение [16][32] -- //
    iSub = 8,   // -- вычитание [16][32] -- //
    iMul = 9,   // -- умножение [16][32] -- //
    iDiv = 10,   // -- деление [16][32] -- //
// -- команды дробной арифметики -- //
    fAdd = 11,
    fSub = 12,
    fMul = 13,
    fDiv = 14,
// -- команды ввода и вывода -- //
    io = 15,    // -- все режимы ввода-вывода перечислены в enum io_mode в реализации операции io в файле "command.cpp" -- ///
// -- команды перехода -- //
    jmp = 40,   // -- прямой безусловный переход -- //
    jzf = 41, // -- jump if ZF = 1 -- //
    jnzf = 42, // -- jump if ZF = 0 -- //
    jsf = 43,   // -- jump if SF = 1 -- //
    jnsf = 44,  // -- jump if SF = 0 -- //
    call = 45,  // -- вызов подпрограммы -- //
    ret = 46,  // -- возврат из подпрограммы -- //
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
    cmd cmd; // -- текущая выполняемая команда -- //

    class Command* command[128]; // -- предварительное объявление класса Command, чтобы избежать циклических зависимостей -- //
                                 // -- массив из 128 указателей на команды -- //
public:
    PSW PSW;      // -- PSW = IP + Flags -- //
    Memory ram;   // -- память, слова - 32 бита, размер адреса 16 бит -- //
    data_t r[16]; // -- регистры общего назначения -- //
    bool was_transition = false; // -- был ли переход -- //

    CPU();
    ~CPU();

    CPU(const CPU &) = delete;
    CPU(const CPU &&) = delete;
    CPU& operator=(const CPU &)  = delete;
    CPU& operator=(const CPU &&) = delete;

    uint8_t get_cmd_t() const noexcept { return cmd.c32.t; }    // -- получить бит длины команды -- //
    uint8_t get_cmd_r1() const noexcept { return cmd.c32.r1; }  // -- получить номер первого регистра -- //
    uint8_t get_cmd_r2() const noexcept { return cmd.c32.r2; }  // -- получить номер второго регистра -- //
    uint16_t get_cmd_address() const noexcept { return cmd.c32.address; }   // -- получить адрес-константу (для 32-битной команды) -- //

    void loadCommand() noexcept { cmd = ram.get_word(PSW.IP).c; } // -- загрузить команду: получаю из памяти по адресу IP слово и принимаю его как команду -- //
    void reset() noexcept { PSW.IP = PSW.FLAGS.SF = PSW.FLAGS.ZF = 0; }
    void run () noexcept;
};

#endif // CPU_H
