#ifndef BITSTRING_H
#define BITSTRING_H
#include "array.h"

class BitString : public Array
{
    BitString& toAdditionalCode(size_type new_N);
public:
    static constexpr uint8_t num_sys = 2; // -- система счисления -- //
    int64_t count_of_SingleBit() const noexcept; // -- количество битовых единиц -- //
    // -- проверки -- //
    void CheckForBinary(const Array &arr) const;    // -- цифры должны быть бинарными -- //
    // -- конструкторы и присваивание -- //
    BitString() = default;
    explicit BitString(const size_type& n, value_type t = 0) : Array(n,t) { CheckForBinary(*this); }
    BitString(const std::initializer_list<value_type> &t) : Array(t) { CheckForBinary(*this); }
    BitString(const std::string &str) : Array(str) { CheckForBinary(*this); }
    // -- основные операции -- //
    BitString& operator&=(const BitString &b);
    BitString& operator|=(const BitString &b);
    BitString& operator^=(const BitString &b);
    BitString& operator<<=(uint64_t i);
    BitString& operator>>=(uint64_t i);
    BitString operator~();
    BitString operator<<(uint64_t i); // -- побитовый сдвиг влево -- //
    BitString operator>>(uint64_t i); // -- побитовый сдвиг вправо -- //
    // -- дружественные функции -- //
    friend BitString operator&(const BitString &a, const BitString &b);
    friend BitString operator^(const BitString &a, const BitString &b);
    friend BitString operator|(const BitString &a, const BitString &b);
    // -- сравнение происходит по количеству единичных битов -- //
    // -- оператор == и != проверяют на равенство -- //
    friend bool operator<(const BitString &a, const BitString &b);
    friend bool operator>=(const BitString &a, const BitString &b);
    friend bool operator>(const BitString &a, const BitString &b);
    friend bool operator<=(const BitString &a, const BitString &b);
    // -- исключения -- //
    class not_binary_exception {}; // -- не четвертичные числа -- //
};

#endif // BITSTRING_H
