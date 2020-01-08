#ifndef LONGL_H
#define LONGL_H

#include <cstdint>
#include <string>
#include <algorithm>

class LongL
{
    uint32_t high;
    uint64_t low;
    static constexpr uint8_t bit_size = 96; // -- размер в битах -- //
    bool CheckForDecimal(const std::string &str) const noexcept; // -- проверка содержимого строки, чтобы она состояла из десятичных цифр -- //
    void CallCheck(const std::string &str) const; // -- проверка строки, чтобы она состояла из цифр и не была длиннее 29 цифр (это максимум для 2 ^ 96) -- //
    friend void fraction (LongL& a, const LongL& in_b, LongL& IntPart);    // -- деление, которое помещает результат целочисл. деления в IntPart, а остаток в параметр a -- //

public:
    LongL(uint64_t _low = 0) : high(0), low(_low) { } // -- с одним числом -- //
    LongL(uint32_t _high, uint64_t _low) : high(_high), low(_low) {} // -- с двумя числами -- //
    LongL(const std::string &str) : high(0), low(0) // со строкой
    {
        CallCheck(str);
        for (char c : str)
        {
            c = c - '0';    // -- вытаскиваю из символа цифру -- //
            (*this) *= 10;
            (*this) += uint32_t(c);
        }

    }
    inline uint32_t get_high() { return high; } // -- нужны для testcase, в секции проверки сдвигов -- //
    inline uint64_t get_low() { return low; }
    std::string toString() const noexcept; // -- перевод в строку -- //
    // -- унарные операции-методы -- //
    LongL operator~() const noexcept;   // -- инвертировать число -- //
    LongL operator-() const noexcept;   // -- перевести в доп.код -- //
    // -- побитовые операции-методы присваивания -- //
    LongL& operator|=(const LongL &b);
    LongL& operator&=(const LongL &b);
    LongL& operator^=(const LongL &b);
    LongL& operator<<=(const uint8_t &i);
    LongL& operator>>=(const uint8_t &i);    
    // -- арифметические операции-методы присваивания -- //
    LongL& operator+=(const LongL &b);
    LongL& operator-=(const LongL &b);
    LongL& operator*=(const LongL &b);
    LongL& operator/=(const LongL &b);
    LongL& operator%=(const LongL &b);
    // -- операции инкремента/декремента -- //
    LongL& operator++() noexcept;
    LongL operator++(int) noexcept;
    LongL& operator--() noexcept;
    LongL operator--(int) noexcept;
    // -- побитовые дружественные операции -- //
    friend LongL operator|(const LongL &a, const LongL &b);
    friend LongL operator&(const LongL &a, const LongL &b);
    friend LongL operator^(const LongL &a, const LongL &b);
    friend LongL operator<<(const LongL &a, const uint8_t &i); // -- побитовый сдвиг влево -- //
    friend LongL operator>>(const LongL &a, const uint8_t &i); // -- побитовый сдвиг вправо -- //
    // -- арифметические дружественные операции -- //
    friend LongL operator+(const LongL &a, const LongL &b);
    friend LongL operator-(const LongL &a, const LongL &b);
    friend LongL operator*(const LongL &a, const LongL &b);
    friend LongL operator/(const LongL &a, const LongL &b);
    friend LongL operator%(const LongL &a, const LongL &b);
    // -- операции сравнения -- //
    friend bool operator==(const LongL &a, const LongL &b);
    friend bool operator!=(const LongL &a, const LongL &b);
    friend bool operator<(const LongL &a, const LongL &b);
    friend bool operator>=(const LongL &a, const LongL &b);
    friend bool operator>(const LongL &a, const LongL &b);
    friend bool operator<=(const LongL &a, const LongL &b);
    friend std::ostream& operator<<(std::ostream& t, const LongL &r); // -- вывод -- //
    friend std::istream& operator>>(std::istream& t, LongL &r); // -- ввод -- //
    // -- исключения -- //
    class not_decimal_exception {}; // -- недесятичное число -- //
    class overflow_exception {};    // -- переполнение (попытка записать слишком большое число для 96-битного представления) -- //
    class zero_divide_exception {};
};

#endif // LONGL_H
