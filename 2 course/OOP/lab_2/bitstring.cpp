#include <string>
#include <stdint.h>
#include "bitstring.h"

bool BitString::CheckForBinary(const std::string &str) const noexcept // проверка содержимого битовой строки, чтобы оно было двоичным
{
    uint8_t len = uint8_t(str.length()); // uint8_t, так как у нас длина строки не больше 128-бит
    for (auto i = 0; i < len; ++i)
    {
        if (str[i] != '0' && str[i] != '1') return false;
    }
    return true;
}

void BitString::CallCheck(const std::string &str) const // проверка содержимого битовой строки, чтобы оно было двоичным
{
    if (str.length() > 128) throw overflow_exception();
    if (!CheckForBinary(str)) throw not_binary_exception();
}

uint64_t BitString::BinaryString_toInt(const std::string &str) // перевожу битовую строку в 64-битное десятичное число
{
    uint64_t res = 0;
    uint64_t m = 1;
    uint8_t len = uint8_t(str.length());
    for(auto i = 0; i < len; ++i)
    {
        if (str[i] == '1')
        {
            res += m << (len - i - 1);
        }
    }
    return res;
}
std::string BitString::toString() const noexcept // перевожу десятичные числа из полей в строку с двоичным кодом
{
    std::stringstream ss;
    uint64_t m = 1;
    for (int i = 0; i < 64; ++i)
    {
        ss << ((F1 & (m << (i-1))) ? 1 : 0);
    }
    for (int i = 0; i < 64; ++i)
    {
        ss << ((F2 & (m << (i-1))) ? 1 : 0);
    }
    std::reverse(s.begin(),s.end());
    //std::string str = GetOptimizedBinary(ss.str());
    return s;
}

BitString& BitString::operator&=(const BitString &b)
{
    F1 &= b.F1;
    F2 &= b.F2;
    return *this;
}

BitString operator&(const BitString &a, const BitString &b) // побитовое and
{
    BitString t = a;
    t &= b;
    return t;
}

BitString& BitString::operator|=(const BitString &b)
{
    F1 |= b.F1;
    F2 |= b.F2;
    return *this;
}

BitString operator|(const BitString &a, const BitString &b) // побитовое or
{
    BitString t = a;
    t |= b;
    return t;
}

BitString& BitString::operator^=(const BitString &b)
{
    F1 ^= b.F1;
    F2 ^= b.F2;
    return *this;
}

BitString operator^(const BitString &a, const BitString &b) // побитовое xor
{
    BitString t = a;
    t ^= b;
    return t;
}

BitString BitString::operator~() // побитовое not
{
    F1 = ~F1;
    F2 = ~F2;
    return *this;
}

BitString& BitString::operator<<=(const uint8_t &i)
{
    std::string str = toString();
    uint8_t len = static_cast<uint8_t>(str.length());
    std::string strnew;
    if (len+i > 128)
    {
        strnew.assign(len,'0');
        strnew.replace(0,len-i,str.substr(i,len));
    } else {
        strnew.assign(len+i,'0');
        strnew.replace(0,len,str);
    }
    *this = strnew;
    return *this;
}

BitString BitString::operator<<(const uint8_t &i) // побитовый сдвиг влево
{
    BitString t = *this;
    t <<= i;
    return t;
}

BitString& BitString::operator>>=(const uint8_t &i)
{
    std::string str = toString();
    uint8_t len = static_cast<uint8_t>(str.length());
    std::string strnew;
    if (len+i > 128)
    {
        strnew.assign(len,'0');
        strnew.replace(i,len-i,str.substr(0,len-i));
    } else {
        strnew.assign(len+i,'0');
        strnew.replace(i,len,str);
    }

    *this = strnew;
    return *this;
}

BitString BitString::operator>>(const uint8_t &i) // побитовый сдвиг вправо
{
    BitString t = *this;
    t >>= i;
    return t;
}

void BitString::SetOutputFlag() noexcept
{
    OutputFlag = true;
}

void BitString::ClearOutputFlag() noexcept
{
    OutputFlag = false;
}

int BitString::count_of_SingleBit() const noexcept // перевожу десятичные числа из полей в строку с двоичным кодом
{
    std::string str = toString();
    uint8_t res = 0;
    res = std::count(str.begin(), str.end(), '1');
    return res;
}

bool operator==(const BitString &a, const BitString &b)
{
    return (a.count_of_SingleBit() == b.count_of_SingleBit());
}
bool operator!=(const BitString &a, const BitString &b)
{
    return !(a == b);
}
bool operator<(const BitString &a, const BitString &b)
{
    return (a.count_of_SingleBit() < b.count_of_SingleBit());
}
bool operator>=(const BitString &a, const BitString &b)
{
    return !(a<b);
}

bool operator>(const BitString &a, const BitString &b)
{
    return (a.count_of_SingleBit() > b.count_of_SingleBit());
}

bool operator<=(const BitString &a, const BitString &b)
{
    return !(a>b);
}

std::string BitString::GetOptimizedBinaryString() const noexcept
{
    std::string str = toString();
    bool mask = false;
    uint64_t i = 0;
    uint64_t len = str.length();
    std::string temp = str;
    while (!mask && i < len)
    {
        if (str[i] == '1') {mask = true; temp.erase(0,i);}
        i++;
    }
    if (i == len) temp = "0";
    return temp;
}

bool is_included(const BitString &a, const BitString &b) // включен ли a в b
{
    std::string s1, s2;
    s1 = a.GetOptimizedBinaryString();
    s2 = b.GetOptimizedBinaryString();
    return (s2.find(s1) != std::string::npos);
}

std::ostream& operator<<(std::ostream& t, const BitString &r)
{
    std::string s;
    if (r.OutputFlag) s = r.GetOptimizedBinaryString();
    else s = r.toString();
    return (t << s);
}
std::istream& operator>>(std::istream& t, BitString &r)
{
    std::string str;
    t >> str;
    r = str;
    return t;
}
