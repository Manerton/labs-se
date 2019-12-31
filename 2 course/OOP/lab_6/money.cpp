#include <sstream>
#include <limits> // для numeric_limits<double>::epsilon()
#include <cmath>
#include "money.h"
using namespace std;

void Money::CheckConversionFactor(double factor) // -- коэффициент должен быть > 0 -- //
{
    if (factor < 0) throw negative_conversion_factor_exception();
    if (factor <= std::numeric_limits<double>::epsilon()) throw divide_by_zero_exception();
}

string Money::toString() const noexcept // -- перевод денег в строку для вывода -- //
{
    stringstream ss;
    ss << rubles << "," << int(kopecks);
    return ss.str();
}

Money &Money::operator+=(const Money &b)
{
    rubles += b.rubles;
    kopecks += b.kopecks;
    while (kopecks >= 100) {kopecks -= 100; ++rubles;}
    return *this;
}

Money &Money::operator-=(const Money &b)
{
    rubles -= b.rubles;
    while (kopecks < b.kopecks)
    {
        --rubles;
        kopecks += 100;
    }
    kopecks -= b.kopecks;
    return *this;
}

Money &Money::operator/=(const Money &b)
{
    double a_v = fabs(rubles) + (kopecks / 100.0);   // -- абсолютное значение -- //
    double b_v = fabs(b.rubles) + (b.kopecks / 100.0);   // -- абсолютное значение -- //
    CheckConversionFactor(b_v);
    a_v /= b_v;
    rubles = static_cast<uint64_t>(a_v);  // -- целая часть результата - это рубли -- //
    kopecks = static_cast<uint8_t>(round((a_v - rubles) * 100));  // -- дробная часть результата - это копейки -- //
    return *this;
}

Money &Money::operator/=(double b)
{
    CheckConversionFactor(b);
    double v = fabs(rubles) + (kopecks / 100.0);   // -- абсолютное значение -- //
    v /= b;
    rubles = static_cast<uint64_t>(v);  // -- целая часть результата - это рубли -- //
    kopecks = static_cast<uint8_t>(round((v - rubles) * 100));  // -- дробная часть результата - это копейки -- //
    return *this;
}

Money &Money::operator*=(double b)
{
    CheckConversionFactor(b);
    double v = fabs(rubles) + (kopecks / 100.0);   // -- абсолютное значение -- //
    v *= b;
    rubles = static_cast<uint64_t>(v);  // -- целая часть результата - это рубли -- //
    kopecks = static_cast<uint8_t>(round((v - rubles) * 100));  // -- дробная часть результата - это копейки -- //
    return *this;
}

Money operator+(const Money &a, const Money &b)
{
    Money t = a;
    t += b;
    return t;
}

Money operator-(const Money &a, const Money &b)
{
    Money t = a;
    t -= b;
    return t;
}

Money operator/(const Money &a, const Money &b)
{
    Money t = a;
    t /= b;
    return t;
}

Money operator*(const Money &a, double b)
{
    Money t = a;
    t *= b;
    return t;
}

bool operator==(const Money &a, const Money &b)
{
    return ((a.rubles == b.rubles) && (a.kopecks == b.kopecks));
}
bool operator!=(const Money &a, const Money &b)
{
    return !(a == b);
}
bool operator<(const Money &a, const Money &b)
{
    if (a.rubles < b.rubles)
    {
        return true;
    }
    if (a.rubles == b.rubles)
    {
        return (a.kopecks < b.kopecks);
    }
    return false;
}
bool operator>=(const Money &a, const Money &b)
{
    return !(a<b);
}

bool operator>(const Money &a, const Money &b)
{
    return (b < a);
}

bool operator<=(const Money &a, const Money &b)
{
    return !(b < a);
}

ostream& operator<<(ostream &t, const Money &r)
{
    string s;
    s = r.toString();
    return (t << s);
}

istream& operator>>(istream &t, Money &r)
{
    double input;
    t >> input;
    r.rubles = static_cast<uint64_t>(input);
    r.kopecks = static_cast<uint8_t>(round((input - r.rubles) * 100));
    return t;
}
