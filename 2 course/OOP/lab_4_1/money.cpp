#include <sstream>
#include <limits> // для numeric_limits<double>::epsilon()
#include <cmath>
#include "money.h"
using namespace std;

void Money::CheckConversionFactor(const double &factor) // -- коэффициент должен быть > 0 -- //
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

}

Money &Money::operator-=(const Money &b)
{

}

Money &Money::operator/=(const Money &b)
{

}

Money &Money::operator/=(const double &b)
{
    CheckConversionFactor(b);
    double v = fabs(rubles) + (kopecks / 100.0);   // -- абсолютное значение -- //
    v /= b;
    rubles = static_cast<uint64_t>(v);  // -- целая часть результата - это рубли -- //
    kopecks = static_cast<uint8_t>(round((v - rubles) * 100));  // -- дробная часть результата - это копейки -- //
    return *this;
}

Money &Money::operator*=(const double &b)
{

}

ostream& operator<<(ostream &t, const Money &r)
{
    string s;
    s = r.toString();
    return (t << s);
}
