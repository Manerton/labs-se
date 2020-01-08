#include "longl.h"
#include <algorithm>
#include <sstream>

using namespace std;

bool LongL::CheckForDecimal(const std::string &str) const noexcept
{
    return all_of(str.begin(),str.end(),
                   [] (char c) { return (isdigit(c)); });
}

void LongL::CallCheck(const string &str) const
{
    if (str.length() > 29) throw overflow_exception();
    if (!CheckForDecimal(str)) throw not_decimal_exception();
}

string LongL::toString() const noexcept
{
    const LongL ten = 10;
    std::string str;
    LongL num = *this;
    LongL result = 0;
    do {
        fraction(num,ten,result);  // -- в num помещается цифра - остаток от деления, в result - результат целочисленного деления -- //
        auto digit = num.low;  // -- берем последнюю цифру - остаток от деления- -- //
        str += std::to_string(digit);
        num = result;   // -- кладем в num результат целочисленного деления, так как остаток больше не нужен -- //
    } while (num != 0);
    std::reverse(str.begin(),str.end());  // -- переворачиваем str, так как прибавлял цифры в обратном порядке -- //
    return str;
}

/// -- унарные операции-методы -- ///
LongL LongL::operator~() const noexcept
{
    LongL t = *this;
    t.low = ~t.low;
    t.high = ~t.high;
    return t;
}

LongL LongL::operator-() const noexcept // -- унарный минус -- //
{
    // -- переводим в доп. код -- //
    return ++(~LongL(*this));   // -- ~chislo + 1 -- //
}

/// -- побитовые операции-методы присваивания -- ///
LongL &LongL::operator|=(const LongL &b)
{
    low |= b.low;
    high |= b.high;
    return *this;
}

LongL &LongL::operator&=(const LongL &b)
{
    low &= b.low;
    high &= b.high;
    return *this;
}

LongL &LongL::operator^=(const LongL &b)
{
    low ^= b.low;
    high ^= b.high;
    return *this;
}

LongL &LongL::operator<<=(const uint8_t &i)
{
    if(i >= bit_size)   // -- если сдвиг больше длины числа -- //
    {
        low = 0;
        high = 0;
    }
    else
    {
        const uint8_t bit_size_of_low = 64;
        uint8_t n = i;
        if (n >= bit_size_of_low)
        {
            n -= bit_size_of_low;
            high = static_cast<uint32_t>(low);  // -- помещаем в high что влезает -- //
            low = 0;
        }
        if (n != 0)
        {
            high <<= n; // -- сдвигаем старшую часть -- //
            const uint64_t mask = ( uint64_t(-1) << (bit_size_of_low - n) );  // -- получаем маску сдвига, чтобы потом получить те биты, которые должны быть перемещены в старшую часть из младшей при сдвиге -- //
            high |= ( (low & mask) >> (bit_size_of_low - n) );   // -- перемещаем в старшую часть биты из младшей части -- //
            low <<= n; // -- сдвигаем младшую часть -- //
        }
    }
    return *this;
}

LongL &LongL::operator>>=(const uint8_t &i)
{
    if(i >= bit_size)   // -- если сдвиг больше длины числа -- //
    {
        low = 0;
        high = 0;
    }
    else
    {
        const uint8_t bit_size_of_high = 32;
        const uint8_t bit_size_of_low = 64;
        uint8_t n = i;
        if (n >= bit_size_of_high)  // -- если сдвиг больше чем длина старшей части -- //
        {
            n -= bit_size_of_high;
            low >>= bit_size_of_high;   // -- сдвигаем младшую часть на длину старшей части -- //
            low |= uint64_t(high) << bit_size_of_high;  // -- перемещаем в младшую часть все биты из старшей -- //
            high = 0;
            low >>= n; // -- сдвигаем младшую часть -- //
        }
        else if (n != 0)
        {
            low >>= n; // -- сдвигаем младшую часть -- //
            const uint64_t mask = ( uint64_t(-1) >> (bit_size_of_high - n) );  // -- получаем маску сдвига, чтобы потом получить те биты, которые должны быть перемещены в младшую часть из старшей при сдвиге -- //
            low |= ( (high & mask) << (bit_size_of_low - n) );   // -- перемещаем в младшую часть биты из старшей части -- //
            high >>= n; // -- сдвигаем старшую часть -- //
        }
    }
    return *this;
}

/// -- арифметические операции-методы присваивания -- ///

LongL &LongL::operator+=(const LongL &b)
{
    uint64_t old_low = low; // -- сохраняем старую младшую часть -- //
    low += b.low;
    high += b.high;
    if (low < old_low) ++high;  // -- если было переполнение в младшей части, значит осуществляем перенос в старшую -- //
    return *this;
}

LongL &LongL::operator-=(const LongL &b)
{
    return *this += -b; // -- складываем с доп. кодом -- //
}

LongL &LongL::operator*=(const LongL &b)    // -- двоичное умножение в столбик -- //
{
    if (b == 0) // -- если умножаем на ноль, результат = 0 -- //
    {
        high = 0;
        low = 0;
    }
    else if (b != 1)    // -- если не умножаем на 1 -- //
    {
        LongL a = *this;
        LongL t = b;

        high = 0;
        low = 0;

        for (uint8_t i = 0; i < bit_size; ++i)
        {
            if (t.low & 1) // -- если последний бит числа == 1 -- //
            {
                *this += (a << i);  // -- складываю с результатом, сдвигая число на i влево -- //
            }

            t >>= 1;
        }
    }
    return *this;
}

void fraction(LongL &a, const LongL &in_b, LongL &IntPart) // -- функция, которая используется операциями /= и %= -- //
{
    LongL b = in_b;
    if (b == 0) throw LongL::zero_divide_exception();

    if (a > b)  // -- деление двоичных чисел в столбик -- //
    {

        LongL x = 1;    // -- используется для формирования результата в двоичном виде, длина x (в битах) равна длине результата -- //
        IntPart = 0;    // -- целая часть от деления -- //
        // -- пока делимое a >= делителя b -- //
        // -- и пока первый бит делителя равен нулю -- //
        // -- т.е пока длина (в битах) делителя b короче длины делимого a -- //
        while((a >= b) && (((b >> (LongL::bit_size - 1)) & 1) == 0))
        {
            x <<= 1;
            b <<= 1;
        }
        // -- в результате x имеет длину (в битах) как у результата, а делитель b как у делимого a -- //
        while (x != 0)  // -- пока мы не прошлись по всей длине результата, слева на право -- //
        {
            if (a >= b) // -- если можно вычесть, то вычитаем -- //
            {
                a -= b;
                IntPart |= x;   // -- и записываем бит в результат -- //
            }

            x >>= 1;   // -- идем дальше -- //
            b >>= 1;
        }
    }
    else if (a == b)
    {
        IntPart = 1;    // -- целая часть от деления -- //
        a = 0;  // -- остаток от деления -- //
    }
    else
    {
        IntPart = 0;
    }
}

LongL &LongL::operator/=(const LongL &b)
{
    LongL result;
    fraction(*this,b,result);
    return *this = result;  // -- возвращаем result - результат целочисленного деления -- //
}

LongL &LongL::operator%=(const LongL &b)
{
    LongL result;
    fraction(*this,b,result);   // -- в результате работы функции fraction в *this помещается остаток от деления -- //
    return *this;
}

// -- операции инкремента/декремента -- //
LongL &LongL::operator++() noexcept
{
    if(++low == 0)  // -- увеличиваю low на 1 и если произошло переполнение (low == 0) то -- //
    {
        ++high;     // -- то увеличиваю high на 1 -- //
    }
    return *this;
}

LongL LongL::operator++(int) noexcept
{
    LongL t = *this;
    ++(*this);
    return t;
}

LongL &LongL::operator--() noexcept
{
    if(low-- == 0)  // -- если перед вычитанием low было равно нулю, то -- //
    {
        --high;
    }
    return *this;
}

LongL LongL::operator--(int) noexcept
{
    LongL t = *this;
    --(*this);
    return t;
}

/// -- побитовые дружественные операции -- ///
LongL operator|(const LongL &a, const LongL &b)
{
    LongL t = a;
    t |= b;
    return t;
}

LongL operator&(const LongL &a, const LongL &b)
{
    LongL t = a;
    t &= b;
    return t;
}

LongL operator^(const LongL &a, const LongL &b)
{
    LongL t = a;
    t ^= b;
    return t;
}

LongL operator<<(const LongL &a, const uint8_t &i)   // побитовый сдвиг влево
{
    LongL t = a;
    t <<= i;
    return t;
}
LongL operator>>(const LongL &a, const uint8_t &i) // побитовый сдвиг вправо
{
    LongL t = a;
    t >>= i;
    return t;
}

/// -- арифметические операции -- ///

LongL operator+(const LongL &a, const LongL &b)
{
    LongL t = a;
    t += b;
    return t;
}

LongL operator-(const LongL &a, const LongL &b)
{
    LongL t = a;
    t -= b;
    return t;
}

LongL operator*(const LongL &a, const LongL &b)
{
    LongL t = a;
    t *= b;
    return t;
}

LongL operator/(const LongL &a, const LongL &b)
{
    LongL t = a;
    t /= b;
    return t;
}

LongL operator%(const LongL &a, const LongL &b)
{
    LongL t = a;
    t %= b;
    return t;
}

/// -- операции сравнения -- ///

bool operator==(const LongL &a, const LongL &b)
{
    return ((a.high == b.high) && (a.low == b.low));
}

bool operator!=(const LongL &a, const LongL &b)
{
    return !(a == b);
}

bool operator<(const LongL &a, const LongL &b)
{
    if (a.high == b.high)
    {
        return a.low < b.low;
    }
    return a.high < b.high;
}

bool operator>=(const LongL &a, const LongL &b)
{
    return !(a < b);
}

bool operator>(const LongL &a, const LongL &b)
{
    return (b < a);
}

bool operator<=(const LongL &a, const LongL &b)
{
    return !(b < a);
}
/// -- ввод/вывод -- ///

ostream& operator<<(ostream &t, const LongL &r)
{
    std::string s = r.toString();
    return (t << s);
}

istream& operator>>(istream &t, LongL &r)
{
    std::string str;
    t >> str;
    r = str;
    return t;
}
