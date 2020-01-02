#ifndef FOUR_H
#define FOUR_H
#include "array.h"
#include <cinttypes>


class Four : public Array
{
    Four& toAdditionalCode(size_type new_N);
public:
    static constexpr uint8_t num_sys = 4; // -- система счисления -- //
    // -- проверки -- //
    void CheckForQuaternary(const Array &arr) const;    // -- цифры должны быть четверичными -- //
    // -- конструкторы и присваивание -- //
    Four() = default;
    explicit Four(const size_type& n, value_type t = 0) : Array(n,t) { CheckForQuaternary(*this); }
    Four(const std::initializer_list<value_type> &t) : Array(t) { CheckForQuaternary(*this); }
    Four(const std::string &str) : Array(str) { CheckForQuaternary(*this); }
    // -- основные операции -- //
    Four& operator+=(const Four &b);
    Four& operator-=(const Four &b);
    Four& operator*=(const Four &b);
    Four& operator/=(const Four &b);
    // -- дружественные функции -- //
    friend Four operator+(const Four &a, const Four &b);
    friend Four operator-(const Four &a, const Four &b);
    friend Four operator*(const Four &a, const Four &b);
    friend Four operator/(const Four &a, const Four &b);
    // -- операции сравнения -- //

    // -- исключения -- //
    class not_quaternary_exception {}; // -- не четвертичные числа -- //
    class a_is_less_than_b_when_subtracted {};  // -- левое число меньше правого при вычитании -- //
    class zero_divide_exception {}; // -- деление на ноль -- //
};

#endif // FOUR_H
