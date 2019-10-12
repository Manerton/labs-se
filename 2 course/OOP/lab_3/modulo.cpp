#include "modulo.h"
#include <sstream>
#include <stdint.h>

void Modulo::CheckForZeroModule(const uint16_t &N) const // если модуль нулевой, то происходит деление на ноль, избегаем этого
{
    if (N!=0) throw zero_module_exception();
}

void Modulo::CheckForDifferentModules(const Modulo &b) const // операции +, - и так далее разрешаем только между числами с одинаковыми модулями
{
    if (N!=b.N) throw different_modules_exception();
}

std::string Modulo::toString() const noexcept
{
    std::stringstream ss;
    uint64_t temp;
    temp = chislo % N; // выводимое число сокращаю по модулю N
    ss << "<" << temp << ", " << N << ">" << std::endl;
    return ss.str();
}

Modulo& Modulo::operator+=(const Modulo &b)
{
    CheckForDifferentModules(b);
    chislo += b.chislo;
    chislo %= N;
    return *this;
}

Modulo& Modulo::operator-=(const Modulo &b)
{
    CheckForDifferentModules(b);
    while (chislo < b.chislo) // чтобы не получить отрицательного переполнения в uint64_t, в случае когда левое число меньше правого
    {
        chislo += N;          // прибавляю модуль к левому
    }
    chislo -= b.chislo;       // потом вычитаю и получаю положительное число, которое является разностью этих чисел по модулю N
    return *this;
}

Modulo& Modulo::operator*=(const Modulo &b)
{
    CheckForDifferentModules(b);
    chislo *= b.chislo;
}
Modulo& Modulo::operator/=(const Modulo &b);
Modulo& Modulo::operator++();
Modulo Modulo::operator++(int);

Modulo operator&(const Modulo &a, const Modulo &b) // побитовое and
{
    Modulo t = a;
    t &= b;
    return t;
}

Modulo& Modulo::operator|=(const Modulo &b)
{
    F1 |= b.F1;
    F2 |= b.F2;
    return *this;
}

Modulo operator|(const Modulo &a, const Modulo &b) // побитовое or
{
    Modulo t = a;
    t |= b;
    return t;
}

Modulo& Modulo::operator^=(const Modulo &b)
{
    F1 ^= b.F1;
    F2 ^= b.F2;
    return *this;
}

Modulo operator^(const Modulo &a, const Modulo &b) // побитовое xor
{
    Modulo t = a;
    t ^= b;
    return t;
}

Modulo Modulo::operator~() // побитовое not
{
    F1 = ~F1;
    F2 = ~F2;
    return *this;
}

Modulo& Modulo::operator<<=(const uint8_t &i)
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

Modulo Modulo::operator<<(const uint8_t &i) // побитовый сдвиг влево
{
    Modulo t = *this;
    t <<= i;
    return t;
}

Modulo& Modulo::operator>>=(const uint8_t &i)
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

Modulo Modulo::operator>>(const uint8_t &i) // побитовый сдвиг вправо
{
    Modulo t = *this;
    t >>= i;
    return t;
}

void Modulo::ToggleOutputFlag() noexcept
{
    OutputFlag = !OutputFlag;
}

int Modulo::count_of_SingleBit() const noexcept // перевожу десятичные числа из полей в строку с двоичным кодом
{
    std::string str = toString();
    uint8_t res = 0;
    res = std::count(str.begin(), str.end(), '1');
    return res;
}

bool operator==(const Modulo &a, const Modulo &b)
{
    return (a.count_of_SingleBit() == b.count_of_SingleBit());
}
bool operator!=(const Modulo &a, const Modulo &b)
{
    return !(a == b);
}
bool operator<(const Modulo &a, const Modulo &b)
{
    return (a.count_of_SingleBit() < b.count_of_SingleBit());
}
bool operator>=(const Modulo &a, const Modulo &b)
{
    return !(a<b);
}

bool operator>(const Modulo &a, const Modulo &b)
{
    return (a.count_of_SingleBit() > b.count_of_SingleBit());
}

bool operator<=(const Modulo &a, const Modulo &b)
{
    return !(a>b);
}

std::string Modulo::GetOptimizedBinaryString() const noexcept
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

bool is_included(const Modulo &a, const Modulo &b) // включен ли a в b
{
    std::string s1, s2;
    s1 = a.GetOptimizedBinaryString();
    s2 = b.GetOptimizedBinaryString();
    return (s2.find(s1) != std::string::npos);
}

std::ostream& operator<<(std::ostream& t, const Modulo &r)
{
    std::string s;
    if (r.OutputFlag) s = r.GetOptimizedBinaryString();
    else s = r.toString();
    return (t << s);
}
std::istream& operator>>(std::istream& t, Modulo &r)
{
    std::string str;
    t >> str;
    r = str;
    return t;
}
