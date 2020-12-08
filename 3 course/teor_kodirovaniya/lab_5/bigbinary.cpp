#include "bigbinary.h"
#include <sstream>

using namespace std;

std::string BigBinary::toString() const
{
    stringstream ss;
    for (auto c : digits)
    {
        ss << int(c);
    }
    string s = ss.str();
    return s;
}

void BigBinary::removeLeadingZeros()
{
    if (size() > 1)
    {
        // ищем первый элемент неравный нулю
        auto pos = find_if(digits.begin(), digits.end(),
                                        [](auto elem)
                                        {
                                           return (elem != 0);
                                        });
        digits.erase(digits.begin(), pos); // удаляем лишние нули
    }
}

BigBinary::value_type& BigBinary::operator[](const size_t i)
{
    return digits.at(i);
}

BigBinary &BigBinary::operator>>=(const int i)
{
    vector<value_type> shifted {digits.end()-i, digits.end()};
    digits.erase(digits.end()-i, digits.end());
    digits.insert(digits.begin(),shifted.begin(),shifted.end());
    return *this;
}

BigBinary &BigBinary::operator<<=(const int i)
{
    vector<value_type> shifted {digits.begin(), digits.begin()+i};
    digits.erase(digits.begin(),digits.begin()+i);
    digits.insert(digits.end(),shifted.begin(),shifted.end());
    return *this;
}

BigBinary &BigBinary::operator+=(const BigBinary &b)
{
    if (size() < b.size())
    {
        size_t diff = b.size() - size();
        digits.insert(digits.begin(),diff,0);
    }

    auto it = digits.rbegin();
    for (auto b_it = b.digits.rbegin(); b_it != b.digits.rend(); ++b_it)
    {
        *it ^= *b_it;
        ++it;
    }
    return *this;
}

BigBinary operator+(const BigBinary &a, const BigBinary &b)
{
    BigBinary t(a);
    t += b;
    return t;
}

BigBinary &BigBinary::operator%=(const BigBinary &b)
{
    const size_t b_N = b.size();
    const size_t N = this->size();
    // количество пройденных битов при делении
    size_t passed_bits = b_N;
    BigBinary t;
    t.digits = {digits.begin(),digits.begin()+passed_bits};
    while (passed_bits != N)
    {
        t += b;
        t.removeLeadingZeros();
        size_t diff = b.size() - t.size();
        if (t.size() == 1 && t[0] == '0') diff = b_N;
        if (passed_bits+diff > N) diff = N - passed_bits;
        t.digits.insert(t.digits.end(),digits.begin()+passed_bits,digits.begin()+passed_bits+diff);
        passed_bits += diff;
    }
    if (t.size() == b_N)
    {
        t += b;
        t.removeLeadingZeros();
    }
    *this = t;
    return *this;
}

BigBinary operator%(const BigBinary &a, const BigBinary &b)
{
    BigBinary t(a);
    t %= b;
    return t;
}

bool operator==(const BigBinary &a, const BigBinary &b)
{
    return a.digits == b.digits;
}

bool operator!=(const BigBinary &a, const BigBinary &b)
{
    return !(a == b);
}


