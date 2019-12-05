#ifndef COMMAND_H
#define COMMAND_H

#include <cinttypes>
#include "cpu.h"
#include "cpuid.h"

class Command
{
public:
    virtual void operator() (CPU& cpu) noexcept = 0;
    virtual ~Command() {}
};

class empty : public Command
{
public:
    virtual void operator() (CPU&) noexcept override {}
};

// -- команды целой арифметики -- //
class iMath : public Command
{
    void set_flags (CPU& cpu) noexcept;
    virtual int32_t work (const int32_t &left, const int32_t &right) noexcept = 0;
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
    // -- Сложение -- //
class iAdd : public iMath {
    virtual int32_t work(const int32_t &left, const int32_t &right) noexcept override
    { return left + right; }
};
// -- команды пересылки данных -- //
class load : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
class save : public Command
{
public:
    virtual void operator() (CPU& cpu) noexcept override;
};
// -- команды ввода и вывода -- //
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
// -- //
#endif // COMMAND_H
