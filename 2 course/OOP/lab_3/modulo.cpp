#include "modulo.h"
#include <sstream>
#include <stdint.h>


void Modulo::CheckForZeroModule(const uint16_t &N) const // если модуль нулевой, то происходит деление на ноль, избегаем этого
{
    if (N==0) throw zero_module_exception();
}

bool Modulo::CheckForNoModulo() const
{
    return (N == NO_MODULO);
}

void Modulo::CheckForDifferentModules(const Modulo &b) const // операции +, - и так далее разрешаем только между числами с одинаковыми модулями
{
    if (N!=b.N) throw different_modules_exception();
}

void Modulo::CheckForExistInverseElem(const uint16_t &i) const // если i >= N, значит обратного элемента не существует (он не был найден)
{
    if (i>=N) throw inverse_elem_not_exist_exception();
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
    while (((b * i) % N != 1) && (i < N)) ++i;
    CheckForExistInverseElem(i); // нашелся ли обратный элемент, если нет, то выбрасывается исключение
    return i;
}

Modulo& Modulo::operator+=(const Modulo &b)
{
    if (!b.CheckForNoModulo())       // Проверяем, b - безмодульное или нет, если с модулем, то проверяем
        CheckForDifferentModules(b); // чтобы модули двух чисел совпадали

    chislo += b.chislo;
    if (chislo >= N) chislo %= N; // сокращаю по модулю N
    return *this;
}

Modulo& Modulo::operator-=(const Modulo &b)
{
    if (!b.CheckForNoModulo())
        CheckForDifferentModules(b);

    uint64_t dop_code = ~b.chislo + 1; // перевожу правое число в доп код
    chislo += dop_code;
    chislo = (N - (~chislo + 1)) % N;
    return *this;
}

Modulo& Modulo::operator*=(const Modulo &b)
{
    if (!b.CheckForNoModulo())
        CheckForDifferentModules(b);

    chislo *= b.chislo;
    if (chislo >= N) chislo %= N;
    return *this;
}

Modulo& Modulo::operator/=(const Modulo &b)
{
    if (!b.CheckForNoModulo())
        CheckForDifferentModules(b);

    uint16_t inverse = GetInverseElement(b.chislo);
    *this *= inverse;
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

Modulo& Modulo::operator--() noexcept
{
    return (*this-=1);
}
Modulo Modulo::operator--(int) noexcept
{
    Modulo t = *this;
    *this -= 1;
    return t;
}

Modulo operator+(const Modulo &a, const Modulo &b)
{
    Modulo t;
    if (a.CheckForNoModulo()) // если левое число безмодульное, то привожу его к модулю правого числа
    {
        t = {a.chislo, b.N};
    } else { t = a; } // просто t = левое
    t += b; // левое + правое
    return t;
}

Modulo operator-(const Modulo &a, const Modulo &b)
{
    Modulo t;
    if (a.CheckForNoModulo())
    {
        t = {a.chislo, b.N};
    } else { t = a; }
    t -= b;
    return t;
}

Modulo operator*(const Modulo &a, const Modulo &b)
{
    Modulo t;
    if (a.CheckForNoModulo())
    {
        t = {a.chislo, b.N};
    } else { t = a; }
    t *= b;
    return t;
}

Modulo operator/(const Modulo &a, const Modulo &b)
{
    Modulo t;
    if (a.CheckForNoModulo())
    {
        t = {a.chislo, b.N};
    } else { t = a; }
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
    return (b < a);
}

bool operator<=(const Modulo &a, const Modulo &b)
{
    a.CheckForDifferentModules(b);
    return !(b < a);
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
