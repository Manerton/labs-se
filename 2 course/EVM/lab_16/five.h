#ifndef Five_H
#define Five_H
#include "array.h"
#include <cinttypes>


class Five : public Array
{
    Five& toAdditionalCode(size_type new_N);
public:
    static constexpr uint8_t num_sys = 5; // -- система счисления -- //
    // -- проверки -- //
    void CheckForFivefold(const Array &arr) const;   // -- цифры должны быть пятиричными -- //
    // -- конструкторы и присваивание -- //
    Five() = default;
    explicit Five(const size_type& n, value_type t = 0) : Array(n,t) { CheckForFivefold(*this); }
    Five(const std::initializer_list<value_type> &t) : Array(t) { CheckForFivefold(*this); }
    Five(const std::string &str) : Array(str) { CheckForFivefold(*this); }
    // -- основные операции -- //
    Five& operator+=(const Five &b);
    Five& operator-=(const Five &b);
    Five& operator*=(const Five &b);
    Five& operator/=(const Five &b);
    // -- дружественные функции -- //
    friend Five operator+(const Five &a, const Five &b);
    friend Five operator-(const Five &a, const Five &b);
    friend Five operator*(const Five &a, const Five &b);
    friend Five operator/(const Five &a, const Five &b);
    // -- операции сравнения -- //

    // -- исключения -- //
    class not_fivefold_exception {}; // -- не пятеричные числа -- //
    class a_is_less_than_b_when_subtracted {};  // -- левое число меньше правого при вычитании -- //
    class zero_divide_exception {}; // -- деление на ноль -- //
};

#endif // Five_H
