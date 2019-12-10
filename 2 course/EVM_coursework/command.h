#ifndef COMMAND_H
#define COMMAND_H

#include <cinttypes>
#include "cpu.h"

class Command   // -- абстрактный класс команд -- //
{
public:
    virtual void operator() (CPU& cpu) noexcept = 0;    // -- чистая виртуальная функция - функтор -- //
    virtual ~Command() {}
};

// -- ОСОБЫЕ КОМАНДЫ -- //
class empty : public Command
{
public:
    virtual void operator() (CPU&) noexcept override {}
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
class load : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
class direct_int_load : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
class save : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
class load_address : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
class indirect_save : public Command
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
class out_int : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
class out_uint : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};

class out_float : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
// -- КОНЕЦ КОМАНДЫ ВВОДА И ВЫВОДА -- //
#endif // COMMAND_H
