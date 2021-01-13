#ifndef CPU_H
#define CPU_H

#include "types.h"
#include "memory.h"
#include "command.h"
#include <cinttypes>
#include <array>
#include <memory>
#include <vector>

class CPU
{
    using byte_t = VM_types::byte_t;
    using address_t = VM_types::address_t;
public:
    enum COP : uint8_t // -- uint8_t ограничивает размер enum -- //
    {
        stop = 0, // -- команда остановки [32] -- //
    // -- команды пересылки данных -- //
        exchange = 1,           // -- обмен значений ST[SP] и ST[SP-i] -- //
        move = 2,               // -- пересылка ST[SP] -> ST[SP-i] -- //
        sign_value_load   = 3,  // -- пересылка память -> стек (непосредственная загрузка целого числа со знаком) -- //
        unsign_value_load = 4,  // -- пересылка память -> стек (непосредственная загрузка беззнакового числа) -- //
        direct_load = 5,        // -- пересылка память -> стек (прямая адресация, загружается значение по адресу (смещению) в команде) -- //
        save = 6,               // -- пересылка стек -> память (вершина не удаляется) -- //
        save_pop = 7,           // -- пересылка стек -> память (вершина удаляется) -- //
        dereference_ptr = 8,    // -- получить значение по адресу, лежащему в стеке и save_pop по адресу из команды -- //
    // -- арифметика: первый операнд в стеке, второй в памяти по адресу в команде -- //
    // -- команды целой арифметики -- //
        iAdd = 10,  // -- сложение -- //
        iSub = 11,  // -- вычитание -- //
        iMul = 12,  // -- умножение -- //
        iDiv = 13, // -- деление -- //
    // -- команды дробной арифметики -- //
        fAdd = 20,
        fSub = 21,
        fMul = 22,
        fDiv = 23,
    // -- команды ввода и вывода -- //
    // st - i/o со стеком, mem - i/o с переменной в памяти по адресу в команде //
        st_in_int       = 30,
        st_in_float     = 31,
        st_out_int      = 32,
        st_out_uint     = 33,
        st_out_float    = 34,
        mem_in_int      = 35,
        mem_in_float    = 36,
        mem_out_int     = 37,
        mem_out_uint    = 38,
        mem_out_float   = 39,
    // -- команды перехода -- //
        direct_jmp = 40,        // -- прямой безусловный переход -- //
        offset_plus_jmp = 41,   // -- относительный безусловный переход вперед -- //
        offset_minus_jmp = 42,  // -- относительный переход назад -- //
        jzf = 43, // -- прямой jump if ZF = 1 -- //
        jnzf = 44, // -- jump if ZF = 0 -- //
        jsf = 45,   // -- jump if SF = 1 -- //
        jnsf = 46,  // -- jump if SF = 0 -- //
        call = 47,  // -- вызов подпрограммы и сохранение в стеке адреса возврата -- //
        ret = 48,  // -- возврат из подпрограммы и удаление вершины из стека -- //
        indirect_jmp = 49   // -- косвенный прямой безусловный переход -- //
    };
    static constexpr uint8_t stack_size = 33;   // -- 32 элемента по 4 байта в стеке; первый элемент нулевой, когда стек пуст -- //
    static constexpr uint16_t cmd_count = 256;  // -- так как КОП - 8 бит, 2^8 = 256 -- //
private:
    struct PSW
    {
        uint32_t IP : 16;
        uint32_t SP : 5;
        // флаги
        uint32_t ZF : 1;        // флаг нуля
        uint32_t SF : 1;        // флаг знака
        uint32_t TF : 1;        // флаг трассировки (флаг ловушки)
        uint32_t EF : 1;        // конец работы
        uint32_t BPF : 1;       // Breakpoint flag
        uint32_t reserved : 6;  // в резерве
        // SP изначально 31, чтобы при загрузке в стек первый элемент попал на 0
        PSW() : IP(0), SP(0) {}
    };
// поля
    // текущая выполняемая команда
    VM_types::cmd_t cmd = {};
    // массив из 256 указателей на команды
    std::array<VM_types::commandTableElem,cmd_count> commandTable = {};
    // лог трассировки
    std::vector<std::string> traceLog;
    // отладочные регистры с точками прерываний
    std::array<VM_types::Breakpoint,VM_types::BPCount> BP;
// методы
    // трассировка
    std::string trace() const noexcept;
    // установить и удалить точку прерывания
    void setBP(size_t id_BP, VM_types::AccessMode _mode, address_t _addr)
    {
        BP[id_BP].address = _addr;
        BP[id_BP].mode = _mode;
    }
    void removeBP(size_t id_BP)
    {
        // обнулить адрес, режим Disabled
        BP[id_BP] = {};
    }
    // достигли точки останова?
    bool reachBP();
public:
    friend class Command;
    friend class Debugger;

    PSW PSW;                            // -- PSW = IP + SP + Flags -- //
    Memory ram;                         // -- память, байтовая, размер адреса 16 бит -- //
    std::array<VM_types::data_t,stack_size> ST = {}; // -- стек -- //

    CPU(bool debug = false);
    ~CPU();
    CPU(const CPU &) = delete;
    CPU(const CPU &&) = delete;
    CPU& operator=(const CPU &)  = delete;
    CPU& operator=(const CPU &&) = delete;

    VM_types::address_t get_cmd_address() const noexcept { return cmd.c.address; }   // -- получить адрес-константу -- //

    inline void loadCommand() noexcept { cmd = ram.get_cmd(PSW.IP); } // -- загрузить команду: получаю из памяти по адресу IP команду (3 байта) -- //
    inline void reset() noexcept { PSW.IP = PSW.EF = PSW.BPF = PSW.SF = PSW.ZF = PSW.TF = 0; }
    void run ();
    inline void setIP(address_t ip) { PSW.IP = ip; }
};

#endif // CPU_H
