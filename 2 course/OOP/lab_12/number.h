#ifndef NUMBER_H
#define NUMBER_H

#include "array.h"
#include <cinttypes>
class Binary;
class Thirteen;
class Seven;

class Number : public Array
{
    Number& toAdditionalCode(size_type new_N);
public:
    static uint8_t num_sys; // -- система счисления -- //
    // -- мультиметоды -- //
    virtual std::string MultMet_dynCast([[maybe_unused]] const Number &b) {return "Number class";};    // требуемая операция
    virtual std::string MultMet_typeId([[maybe_unused]] const Number &b) {return "Number class";} ;
    virtual std::string MultMet_virtFunc([[maybe_unused]] const Number &b) const {return "Number class";};
    virtual std::string MultMet_virtFunc([[maybe_unused]] const Binary &b) const {return "Number class";};
    virtual std::string MultMet_virtFunc([[maybe_unused]] const Thirteen &b) const {return "Number class";};
    virtual std::string MultMet_virtFunc([[maybe_unused]] const Seven &b) const {return "Number class";};
    // -- проверки -- //
    void CheckForNumSys(const Array &arr) const;
    // -- конструкторы и присваивание -- //
    Number() : Array() { }
    Number(const size_type& n, value_type t = 0) : Array(n,t) { }
    Number(const std::initializer_list<value_type> &t) : Array(t) { }
    Number(const std::string &str) : Array(str) { }
    // эти тоже, чтобы работал конструктор перемещения
    Number(const Number& other) : Array(other) { }
    Number(Number&& other) : Array(std::move(other)) {}
    Number& operator=(const Number& other) = default;
    Number& operator=(Number&& other) = default;
    virtual ~Number() {}
    // -- основные операции -- //
    Number& operator+=(const Number &b);
    Number& operator-=(const Number &b);
    Number& operator*=(const Number &b);
    Number& operator/=(const Number &b);
    // -- дружественные функции -- //
    friend Number operator+(const Number &a, const Number &b);
    friend Number operator-(const Number &a, const Number &b);
    friend Number operator*(const Number &a, const Number &b);
    friend Number operator/(const Number &a, const Number &b);
    // -- исключения -- //
    class wrong_num_sys_exception {}; // -- числа не той системы счисления-- //
    class a_is_less_than_b_when_subtracted {};  // -- левое число меньше правого при вычитании -- //
    class zero_divide_exception {}; // -- деление на ноль -- //
};

#endif // NUMBER_H
