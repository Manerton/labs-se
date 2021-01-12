#ifndef TYPES_H
#define TYPES_H
#include <cinttypes>
#include <array>
#include <vector>
#include <functional>
#include <map>
#include <string>

// характеристики архитектуры, типы и их размеры
namespace VM_types
{
    using byte_t = uint8_t;
    using address_t = uint16_t;

    union cmd_t   // -- команда 3 байта -- //
    {
        #pragma pack(push, 1)
        struct cmd24_struct {
            uint8_t cop : 8;
            address_t address : 16;  // -- адрес, константа, смещение -- //
        };
        #pragma pack(pop)
        cmd24_struct c{0,0};
        std::array<byte_t,3> b;  // -- 3 байта -- //
    };

    union data_t {  // -- тип данных (размер 4 байта, 32-бита) -- //
        uint32_t u = 0; // -- беззнаковое число -- //
        int32_t i;  // -- знаковое число -- //
        float f;    // -- дробное число -- //
        std::array<byte_t,4> b;     // -- слово, 4 байта -- //
    };

    static constexpr uint8_t cmd_length = sizeof(cmd_t);    // -- длина команды - 3 байта, увеличиваем IP на эту величину -- //
    static constexpr uint8_t data_length = sizeof(data_t);   // -- 4 байта для данных (целые, дробные) -- //

    // коды операций
    enum COP : uint8_t // -- uint8_t ограничивает размер enum -- //
    {
        nop = 0,  // -- не операция -- //
        stop = 0, // -- команда остановки -- //
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

    // префиксы для загрузчика
    enum Prefix : char
    {
        integer = 'i',      // целое
        real = 'f',         // дробное
        execute = 'e',      // адрес старта программы
        loadAddress = 'a',  // установить адрес размещения программы
        cmd = 'c'           // команда
    };

}

// различные типы, относящиеся к ассемблеру
namespace ASM_types {
    // символ счетчика размещения, он будет в таблице имен
    static constexpr char LC_Symbol[] = "$";

    enum class Error: uint8_t {
        noError = 0,                // ошибки нет
        // ошибки при парсе оператора
        illSymbol,					// недопустимый символ в одном из полей оператора
        noColon,                    // нет двоеточия после метки
        // ошибки выражений
        illInteger,					// ошибка в записи целого числа
        illFloatValue,              // ошибка в записи дробного числа
        divideByZero,               // деление на ноль
        illExpression,				// неверный синтаксис математического выражения
        undefLabel,					// неизвестное имя в выражении
        // общие ошибки оператора
        reuseLabel,					// повторно определенная метка
        illLabel,                   // использование зарезервированного слова (операции) в качестве метки
        illCountArgument,		    // неправильное количество аргументов
        illOperation,               // недопустимый код операции
        noLabel,					// нет метки в equ, proc
        noEnd						// нет директивы end
    };
    struct Operator
    {
        bool work = true;                       // подлежит ли обработке
        VM_types::address_t LC = 0;             // смещение - счетчик размещения
        std::vector<VM_types::byte_t> binary;	// двоичная команда
        uint32_t number = 0;                    // номер исходной строки
        std::string label;                      // метка оператора
        std::string code;                       // код операции или директивы
        std::string argument;                   // поле с аргументом
        std::string comment;                    // комметарий (для листинга)
        Error nError = Error::noError;		    // номер ошибки
    };

    using LabelValue = decltype(VM_types::data_t::u);
    using TableNames_t = std::map<std::string, LabelValue>;

    struct Operation
    {
        using Handler = std::function<void(Operator&,TableNames_t&)>;

        Handler function;                           // транслирующая функция
        VM_types::byte_t COP = VM_types::COP::nop;  // у директив nop (код 0)
        bool wLabel = true;                         // использовать общий механизм обработки меток
    };

    using TableOperations_t = std::map<std::string_view, Operation>;

}
#endif // TYPES_H
