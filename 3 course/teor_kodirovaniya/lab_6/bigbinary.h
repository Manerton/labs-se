#ifndef BIGBINARY_H
#define BIGBINARY_H

#include <algorithm>
#include <vector>
#include <string>

class BigBinary
{
    using value_type = char;

    std::vector<value_type> digits;
    void removeLeadingZeros();
public:
    BigBinary() = default;
    BigBinary(const uint64_t c)
    {
        size_t bitLen = 0;
        uint64_t copy_c = c;
        while (copy_c != 0)
        {
            copy_c >>= 1;
            ++bitLen;
        }
        digits.resize(bitLen,0);
        uint64_t m = 1;
        for (size_t i = 0; i < bitLen; ++i)
        {
            digits[i] = ((c & (m << i)) ? 1 : 0); // иду с конца числа c и пишу 1 или 0 в зависимости от бита числа на позиции i
        }

        reverse(digits.begin(),digits.end()); // Так как шел с конца, то переворачиваю вектор
    }
    BigBinary(const std::string& s) : digits(s.begin(),s.end())
    {
        for (auto &c : digits) c -= '0';
    }

    size_t size() const { return digits.size(); }
    // получить количество единичных разрядов
    size_t get_W() const { return std::count(digits.begin(),digits.end(),1); };
    std::string toString() const;

    value_type& operator[](const size_t i);

    BigBinary& operator>>=(const int i);
    BigBinary& operator<<=(const int i);
    BigBinary& operator+=(const BigBinary &b);
    friend BigBinary operator+(const BigBinary &a, const BigBinary &b);
    BigBinary& operator%=(const BigBinary &b);
    friend BigBinary operator%(const BigBinary &a, const BigBinary &b);
    BigBinary& operator*=(const BigBinary &b);
    friend bool operator==(const BigBinary &a, const BigBinary &b);
    friend bool operator!=(const BigBinary &a, const BigBinary &b);
};

#endif // BIGBINARY_H
