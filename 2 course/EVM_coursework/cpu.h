#ifndef CPU_H
#define CPU_H

#include <cinttypes>
#include "types.h"
#include "memory.h"

enum COP : uint8_t // -- uint8_t ограничивает размер enum -- //
{
    stop = 0, // -- команда остановки [32] -- //
    empty = 1, // -- пустая команда-заглушка [16] -- //
// -- команды пересылки данных -- //
    exchange = 2, // -- обменять содержимое регистров [16] -- //
    move = 3, // -- пересылка <регистр источник> <регистр назначения> [16] -- //
    load = 4, // -- пересылка память - регистр (в r2) [32] -- //
    save = 5, // -- пересылка регистр (r1) - память [32] -- //
    load_address = 6, // -- загрузка константы-адреса в регистр [32] -- //
    // -- продумать еще команды для разных видов адресаций -- //
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
    in_int = 15,
    out_int = 16, // -- вывод целого числа [16][32] -- //
    in_uint = 17,
    out_uint = 18,
    in_float = 19,
    out_float = 20,
// -- команды перехода -- //
    jmp = 21,   // -- прямой безусловный переход -- //
    jmp_offset = 22,    // -- относительный безусловный переход -- //
    jmp_ptr = 23,   // -- косвенный безусловный переход -- //
    jzf = 24, // -- jump if ZF = 1 -- //
    jnzf = 25, // -- jump if ZF = 0 -- //
    jsf = 26,   // -- jump if SF = 1 -- //
    jnsf = 27,  // -- jump if SF = 0 -- //
    call = 28,  // -- вызов подпрограммы -- //
    ret = 29,  // -- возврат из подпрограммы -- //
// -- ещё команды пересылки данных -- //
    direct_int_load = 30, // -- загрузка целочисленного значения вместо адреса в регистр (в r2) [32] (непосредственная адресация) -- //
    indirect_save = 31, // -- сохранение результата из r1 в память по адресу, который хранится в r2 [16] -- //
    dereference_ptr = 32 // -- разыменовать указатель (т.е получить значение по адресу, лежащему в регистре r1) [16][32] -- //
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

    CPU();
    ~CPU();

    CPU(const CPU &) = delete;
    CPU(const CPU &&) = delete;
    CPU& operator=(const CPU &)  = delete;
    CPU& operator=(const CPU &&) = delete;

    uint8_t get_cmd_t() const noexcept { return cmd.c32.t; }    // -- получить бит длины команды -- //
    uint8_t get_cmd_cop() const noexcept { return cmd.c32.cop; }    // -- получить код операции -- //
    uint8_t get_cmd_r1() const noexcept { return cmd.c32.r1; }  // -- получить номер первого регистра -- //
    uint8_t get_cmd_r2() const noexcept { return cmd.c32.r2; }  // -- получить номер второго регистра -- //
    uint16_t get_cmd_address() const noexcept { return cmd.c32.address; }   // -- получить адрес-константу (для 32-битной команды) -- //

    void loadCommand() noexcept { cmd = ram.get_data(PSW.IP).c; } // -- загрузить команду: получаю из памяти по адресу IP слово и принимаю его как команду -- //
    void reset() noexcept { PSW.IP = PSW.FLAGS.SF = PSW.FLAGS.ZF = 0; }
    void run () noexcept;
};

#endif // CPU_H
