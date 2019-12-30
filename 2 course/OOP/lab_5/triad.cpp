#include "triad.h"
#include <iostream>
#include <sstream>

using namespace std;

std::string Triad::toString() const noexcept
{
    stringstream ss;
    ss << num1 << " " << num2 << " " << num3;
    return ss.str();
}

Triad &Triad::operator+=(int64_t b)
{
    num1 += b;
    num2 += b;
    num3 += b;
    return *this;
}

Triad &Triad::operator*=(int64_t b)
{
    num1 *= b;
    num2 *= b;
    num3 *= b;
    return *this;
}

Triad operator+(const Triad &a, int64_t b)
{
    Triad t = a;
    t += b;
    return t;
}

Triad operator*(const Triad &a, int64_t b)
{
    Triad t = a;
    t *= b;
    return t;
}

bool operator==(const Triad &a, const Triad &b)
{
    const bool equal_1 = (a.num1 == b.num1);
    const bool equal_2 = (a.num2 == b.num2);
    const bool equal_3 = (a.num3 == b.num3);
    return (equal_1 && equal_2 && equal_3);
}

bool operator!=(const Triad &a, const Triad &b)
{
    return !(a == b);
}

ostream& operator<<(ostream &t, const Triad &r)
{
    string s;
    s = r.toString();
    return (t << s);
}

istream& operator>>(istream &t, Triad &r)
{
    int64_t num1, num2, num3;
    t >> num1 >> num2 >> num3;
    r = {num1, num2, num3};
    return t;
}
