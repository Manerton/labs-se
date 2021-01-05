#ifndef COMMAND_H
#define COMMAND_H
#include <cinttypes>

// -- предварительное объявление класса CPU, чтобы избежать циклических зависимостей -- //
class CPU;

class Command   // -- абстрактный класс команд -- //
{
public:
    virtual void operator() (CPU& cpu) noexcept = 0;    // -- чистая виртуальная функция - функтор -- //
    virtual ~Command() {}
};

// -- КОМАНДЫ ПЕРЕСЫЛКИ ДАННЫХ -- //
class exchange : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
class move : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
class sign_value_load : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
class unsign_value_load : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
class direct_load : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
class save : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
class save_pop : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
class dereference_ptr : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
// -- КОНЕЦ КОМАНДЫ ПЕРЕСЫЛКИ ДАННЫХ -- //

// -- КОМАНДЫ ЦЕЛОЙ АРИФМЕТИКИ -- //
class iMath : public Command
{
    void set_flags (CPU& cpu) noexcept; // -- устанавливаем флаги в соответствии с результатом вычисления -- //
    virtual int32_t calculate (int32_t left, int32_t right) noexcept = 0; // -- соответствующее вычисление -- //
public:
    virtual void operator() (CPU& cpu) noexcept override;   // -- переопределяем функтор в cpp файле -- //
};

// --- сложение --- //
class iAdd : public iMath
{
    virtual int32_t calculate (int32_t left, int32_t right) noexcept override
    { return left + right; }
};

// --- вычитание --- //
class iSub : public iMath
{
    virtual int32_t calculate (int32_t left, int32_t right) noexcept override
    { return left - right; }
};

// --- умножение --- //
class iMul : public iMath
{
    virtual int32_t calculate (int32_t left, int32_t right) noexcept override
    { return left * right; }
};

// --- деление --- //
class iDiv : public iMath
{
    virtual int32_t calculate (int32_t left, int32_t right) noexcept override
    { return left / right; }
};
// -- КОНЕЦ КОМАНДЫ ЦЕЛОЙ АРИФМЕТИКИ -- //

// -- КОМАНДЫ ДРОБНОЙ АРИФМЕТИКИ -- //
class fMath : public Command
{
    void set_flags (CPU& cpu) noexcept; // -- устанавливаем флаги в соответствии с результатом вычисления -- //
    virtual float calculate (float left, float right) noexcept = 0; // -- соответствующее вычисление -- //
public:
    virtual void operator() (CPU& cpu) noexcept override;   // -- переопределяем функтор в cpp файле -- //
};

// --- сложение --- //
class fAdd : public fMath
{
    virtual float calculate (float left, float right) noexcept override
    { return left + right; }
};

// --- вычитание --- //
class fSub : public fMath
{
    virtual float calculate (float left, float right) noexcept override
    { return left - right; }
};

// --- умножение --- //
class fMul : public fMath
{
    virtual float calculate (float left, float right) noexcept override
    { return left * right; }
};

// --- деление --- //
class fDiv : public fMath
{
    virtual float calculate (float left, float right) noexcept override
    { return left / right; }
};
// -- КОНЕЦ КОМАНДЫ ЦЕЛОЙ АРИФМЕТИКИ -- //

// -- КОМАНДЫ ВВОДА И ВЫВОДА -- //
class IO : public Command
{
    virtual void call_io (CPU& cpu) noexcept = 0; // -- вызов операции st_io или mem_io -- //
protected:
    enum class io_mode : uint8_t {
        in_int = 0,     // -- ввод целого числа -- //
        in_float = 1,   // -- ввод дробного числа -- //
        out_int = 2,    // -- вывод целого числа -- //
        out_uint = 3,   // -- вывод беззнакового числа -- //
        out_float = 4   // -- вывод дробного числа -- //
    };

    void st_io(CPU& cpu, io_mode mode) noexcept;
    void mem_io(CPU& cpu, io_mode mode) noexcept;
public:
    virtual void operator() (CPU& cpu) noexcept override;
};

class st_in_int : public IO
{
public:
    virtual void call_io (CPU& cpu) noexcept override;
};
class st_in_float : public IO
{
public:
    virtual void call_io (CPU& cpu) noexcept override;
};
class st_out_int : public IO
{
public:
    virtual void call_io (CPU& cpu) noexcept override;
};
class st_out_uint : public IO
{
public:
    virtual void call_io (CPU& cpu) noexcept override;
};
class st_out_float : public IO
{
public:
    virtual void call_io (CPU& cpu) noexcept override;
};
class mem_in_int : public IO
{
public:
    virtual void call_io (CPU& cpu) noexcept override;
};
class mem_in_float : public IO
{
public:
    virtual void call_io (CPU& cpu) noexcept override;
};
class mem_out_int : public IO
{
public:
    virtual void call_io (CPU& cpu) noexcept override;
};
class mem_out_uint : public IO
{
public:
    virtual void call_io (CPU& cpu) noexcept override;
};
class mem_out_float : public IO
{
public:
    virtual void call_io (CPU& cpu) noexcept override;
};
// -- КОНЕЦ КОМАНДЫ ВВОДА И ВЫВОДА -- //

// -- КОМАНДЫ ПЕРЕХОДОВ -- //
class Jump : public Command
{
    virtual void call_go_to (CPU& cpu) noexcept = 0; // -- вызов операции go_to -- //
protected:
    enum class jmp_mode : uint8_t {
        direct_mode = 0,            // -- прямой переход -- //
        offset_plus_mode = 1,       // -- относительный переход вперед -- //
        offset_minus_mode = 2,      // -- относительный переход назад -- //
        indirect_mode = 3           // -- косвенный прямой переход -- //
    };
    void go_to (CPU& cpu, jmp_mode mode = jmp_mode::direct_mode) noexcept;
public:
    virtual void operator() (CPU& cpu) noexcept override;
};

class direct_jmp : public Jump
{
    virtual void call_go_to (CPU& cpu) noexcept override;
};
class indirect_jmp : public Jump
{
    virtual void call_go_to (CPU& cpu) noexcept override;
};
class offset_plus_jmp : public Jump
{
    virtual void call_go_to (CPU& cpu) noexcept override;
};
class offset_minus_jmp : public Jump
{
    virtual void call_go_to (CPU& cpu) noexcept override;
};
class jzf : public Jump
{
    virtual void call_go_to (CPU& cpu) noexcept override;
};
class jnzf : public Jump
{
    virtual void call_go_to (CPU& cpu) noexcept override;
};
class jsf : public Jump
{
    virtual void call_go_to (CPU& cpu) noexcept override;
};
class jnsf : public Jump
{
    virtual void call_go_to (CPU& cpu) noexcept override;
};
class call : public Jump
{
    virtual void call_go_to (CPU& cpu) noexcept override;
};
class ret : public Jump
{
    virtual void call_go_to (CPU& cpu) noexcept override;
};
// -- КОНЕЦ КОМАНДЫ ПЕРЕХОДОВ -- //
#endif // COMMAND_H
