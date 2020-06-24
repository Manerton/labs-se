#ifndef SEVEN_H
#define SEVEN_H
#include "array.h"
#include <cinttypes>


class Seven : public Array
{
    Seven& toAdditionalCode(size_type new_N);
public:
    static constexpr uint8_t num_sys = 7; // -- система счисления -- //
    // -- проверки -- //
    void CheckForSevenfold(const Array &arr) const;    // -- цифры должны быть семеричными -- //
    // -- конструкторы и присваивание -- //
    Seven() = default;
    explicit Seven(const size_type& n, value_type t = 0) : Array(n,t) { CheckForSevenfold(*this); }
    Seven(const std::initializer_list<value_type> &t) : Array(t) { CheckForSevenfold(*this); }
    Seven(const std::string &str) : Array(str) { CheckForSevenfold(*this); }
    // -- основные операции -- //
    Seven& operator+=(const Seven &b);
    Seven& operator-=(const Seven &b);
    Seven& operator*=(const Seven &b);
    Seven& operator/=(const Seven &b);
    // -- дружественные функции -- //
    friend Seven operator+(const Seven &a, const Seven &b);
    friend Seven operator-(const Seven &a, const Seven &b);
    friend Seven operator*(const Seven &a, const Seven &b);
    friend Seven operator/(const Seven &a, const Seven &b);
    // -- операции сравнения -- //

    // -- исключения -- //
    class not_sevenfold_exception {}; // -- не семеричные числа -- //
    class a_is_less_than_b_when_subtracted {};  // -- левое число меньше правого при вычитании -- //
    class zero_divide_exception {}; // -- деление на ноль -- //
};

#endif // SEVEN_H
