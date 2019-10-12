#include "modulo.h"
#include <sstream>
#include <stdint.h>

void Modulo::CheckForZeroModule(const uint16_t &N) const // если модуль нулевой, то происходит деление на ноль, избегаем этого
{
    if (N==0) throw zero_module_exception();
}

void Modulo::CheckForDifferentModules(const Modulo &b) const // операции +, - и так далее разрешаем только между числами с одинаковыми модулями
{
    if (N!=b.N) throw different_modules_exception();
}

std::string Modulo::toString() const noexcept
{
    std::stringstream ss;
    ss << chislo << " mod " << N;
    return ss.str();
}

uint16_t Modulo::GetInverseElement(const uint64_t &b) const
{
    uint16_t i = 1;
    while (((b * i) % N != 1) && (i < N)) i++;
    if (i < N) return i;
    else throw inverse_elem_not_exist_exception();
}

Modulo &Modulo::operator+=(const uint64_t &b) noexcept
{
    chislo += b;   // прибавляю
    if (chislo >= N) chislo %= N; // сокращаю по модулю N
    return *this;
}

Modulo& Modulo::operator+=(const Modulo &b)
{
    CheckForDifferentModules(b); // проверяю, чтобы был одинаковый модуль
    *this += b.chislo; // делегирование через уже определенную операцию сложения с присваиванием с uint64_t
    return *this;
}

Modulo& Modulo::operator-=(const uint64_t &b) noexcept
{
    while (chislo < b) // чтобы не получить отрицательного переполнения в uint64_t, в случае когда левое число меньше правого
    {
        chislo += N;          // прибавляю модуль к левому
    }
    chislo -= b;       // потом вычитаю и получаю положительное число, которое является разностью этих чисел по модулю N
    return *this;
}

Modulo& Modulo::operator-=(const Modulo &b)
{
    CheckForDifferentModules(b);
    *this -= b.chislo;
    return *this;
}

Modulo& Modulo::operator*=(const uint64_t &b) noexcept
{
    chislo *= b;
    if (chislo >= N) chislo %= N;
    return *this;
}

Modulo& Modulo::operator*=(const Modulo &b)
{
    CheckForDifferentModules(b);
    *this *= b.chislo;
    return *this;
}

Modulo& Modulo::operator/=(const uint64_t &b)
{
    uint16_t inverse = GetInverseElement(b);
    *this *= inverse;
    return *this;
}

Modulo& Modulo::operator/=(const Modulo &b)
{
    CheckForDifferentModules(b);
    *this /= b.chislo;
    return *this;
}
Modulo& Modulo::operator++() noexcept
{
    return (*this+=1);
}
Modulo Modulo::operator++(int) noexcept
{
    Modulo t = *this;
    *this += 1;
    return t;
}

Modulo operator+(const Modulo &a, const Modulo &b)
{
    Modulo t = a;
    t += b;
    return t;
}

Modulo operator+(const Modulo &a, const uint64_t &b) noexcept
{
    Modulo t = a;
    t += b;
    return t;
}

Modulo operator+(const uint64_t &a, const Modulo &b) noexcept
{
    return b+a;
}

Modulo operator-(const Modulo &a, const Modulo &b)
{
    Modulo t = a;
    t -= b;
    return t;
}

Modulo operator-(const Modulo &a, const uint64_t &b) noexcept
{
    Modulo t = a;
    t -= b;
    return t;
}

Modulo operator-(const uint64_t &a, const Modulo &b) noexcept
{
    Modulo t = b;
    t.chislo = a;
    t -= b;
    return t;
}

Modulo operator*(const Modulo &a, const Modulo &b)
{
    Modulo t = a;
    t *= b;
    return t;
}

Modulo operator*(const Modulo &a, const uint64_t &b) noexcept
{
    Modulo t = a;
    t *= b;
    return t;
}

Modulo operator*(const uint64_t &a, const Modulo &b) noexcept
{
    return b*a;
}

Modulo operator/(const Modulo &a, const Modulo &b)
{
    Modulo t = a;
    t /= b;
    return t;
}

Modulo operator/(const Modulo &a, const uint64_t &b)
{
    Modulo t = a;
    t /= b;
    return t;
}

Modulo operator/(const uint64_t &a, const Modulo &b)
{
    Modulo t = b;
    t.chislo = a;
    t /= b;
    return t;
}

bool operator==(const Modulo &a, const Modulo &b)
{
    a.CheckForDifferentModules(b);
    return (a.chislo == b.chislo);
}
bool operator!=(const Modulo &a, const Modulo &b)
{
    return !(a == b);
}
bool operator<(const Modulo &a, const Modulo &b)
{
    a.CheckForDifferentModules(b);
    return (a.chislo < b.chislo);
}
bool operator>=(const Modulo &a, const Modulo &b)
{
    a.CheckForDifferentModules(b);
    return !(a<b);
}

bool operator>(const Modulo &a, const Modulo &b)
{
    a.CheckForDifferentModules(b);
    return (a.chislo > b.chislo);
}

bool operator<=(const Modulo &a, const Modulo &b)
{
    a.CheckForDifferentModules(b);
    return !(a>b);
}

std::ostream& operator<<(std::ostream& t, const Modulo &r)
{
    std::string s = r.toString();
    return (t << s);
}
std::istream& operator>>(std::istream& t, Modulo &r)
{
    uint64_t chislo;
    uint16_t N;
    t >> chislo >> N;
    r = {chislo, N};
    return t;
}
