#ifndef MATHEXPRESSION_H
#define MATHEXPRESSION_H
#include "types.h"
#include <memory>
#include <string>

// исключение - недопустимое число
class illInteger : public std::exception
{
public:
    const char *what() const noexcept {
        return "Illegal integer";
    }
};

// абстрактный класс математического выражения
class MathExpression
{
public:
    using Context = ASM_types::TableNames_t;
    using Value = ASM_types::LabelValue;
    using sharedPtrExp = std::shared_ptr<MathExpression>;
    // вычисляет выражение
    virtual Value eval(Context &context) = 0;
    virtual ~MathExpression() {}
};
// константа
class NumberExpression : public MathExpression
{
    Value val;
public:
    explicit NumberExpression(Value _val) : val(_val) {}
    inline Value eval(Context&) override { return val; }
};
// нетерминальное выражение (правило грамматики)
class NonTermExpression : public MathExpression
{
    sharedPtrExp leftOperand;
    sharedPtrExp rightOperand;
protected:
    // -- соответствующее вычисление -- //
    virtual Value calculate (Value left, Value right) = 0;
public:
    NonTermExpression(const sharedPtrExp &left, const sharedPtrExp &right)
        : leftOperand(left), rightOperand(right) {}
    inline Value eval (Context& context) override
    { return calculate(leftOperand->eval(context), rightOperand->eval(context)); }
};
// сложение
class AddExpression : public NonTermExpression
{
    inline Value calculate (Value left, Value right) override
    { return left + right; }
public:
    AddExpression(const sharedPtrExp &left, const sharedPtrExp &right)
        : NonTermExpression(left,right) {}
};
// вычитание
class SubExpression : public NonTermExpression
{
    inline Value calculate (Value left, Value right) override
    { return left - right; }
public:
    SubExpression(const sharedPtrExp &left, const sharedPtrExp &right)
        : NonTermExpression(left,right) {}
};
// умножение
class MulExpression : public NonTermExpression
{
    virtual inline Value calculate (Value left, Value right) override
    { return left * right; }
public:
    MulExpression(const sharedPtrExp &left, const sharedPtrExp &right)
        : NonTermExpression(left,right) {}
};
// деление
class DivExpression : public NonTermExpression
{
    inline Value calculate (Value left, Value right) override
    {
        if (right == 0) throw DivideByZero();
        return left / right;
    }

public:
    DivExpression(const sharedPtrExp &left, const sharedPtrExp &right)
        : NonTermExpression(left,right) {}

    class DivideByZero : public std::exception
    {
    public:
        const char *what() const noexcept {
            return "Divide by zero";
        }
    };

};

// деление по модулю
class ModExpression : public NonTermExpression
{
    inline Value calculate (Value left, Value right) override
    { return left % right; }
public:
    ModExpression(const sharedPtrExp &left, const sharedPtrExp &right)
        : NonTermExpression(left,right) {}
};
// унарный минус
class UnaryExpression : public MathExpression
{
    sharedPtrExp operand;
public:
    UnaryExpression(const sharedPtrExp &_operand)
        : operand(_operand) {}
    Value eval (Context& context) override
    {
        int64_t temp = -static_cast<int64_t>((operand->eval(context)));
        if (temp < std::numeric_limits<int32_t>::min()) throw illInteger();
        return static_cast<Value>(temp);
    }
};

#endif // MATHEXPRESSION_H

