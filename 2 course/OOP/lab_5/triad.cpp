#include <sstream>
#include "triad.h"


int64_t Triad::getNum1() const
{
    return num1;
}

void Triad::setNum1(const int64_t &b)
{
    num1 = b;
}

int64_t Triad::getNum2() const
{
    return num2;
}

void Triad::setNum2(const int64_t &b)
{
    num2 = b;
}

int64_t Triad::getNum3() const
{
    return num3;
}

void Triad::setNum3(const int64_t &b)
{
    num3 = b;
}
std::string Triad::toString() const
{
    stringstream ss;
    ss << num1 << " " << num2 << " " << num3;
    return ss.str();
}

void Triad::add_to_num1(const int64_t &b)
{
    num1 += b;
}

void Triad::add_to_num2(const int64_t &b)
{
    num2 += b;
}

void Triad::add_to_num3(const int64_t &b)
{
    num3 += b;
}

void Triad::mul_num1(const int64_t &b)
{
    num1 *= b;
}

void Triad::mul_num2(const int64_t &b)
{
    num2 *= b;
}

void Triad::mul_num3(const int64_t &b)
{
    num3 *= b;
}

Triad &Triad::operator+=(const int64_t &b)
{
    num1 += b;
    num2 += b;
    num3 += b;
    return *this;
}

Triad &Triad::operator*=(const int64_t &b)
{
    num1 *= b;
    num2 *= b;
    num3 *= b;
    return *this;
}

bool operator==(const Triad &a, const Triad &b)
{
    const bool equal_1 = (a.num1 == b.num1);
    const bool equal_2 = (a.num2 == b.num2);
    const bool equal_3 = (a.num3 == b.num3);
    return ((equal_1 == equal_2) && (equal_2 == equal_3));
}

bool operator!=(const Triad &a, const Triad &b)
{
    return !(a == b);
}
