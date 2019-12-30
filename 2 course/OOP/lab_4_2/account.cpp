#include <sstream>
#include <limits> // для numeric_limits<double>::epsilon()
#include <cmath>
#include "account.h"
using namespace std;

void Account::change_surname(const std::string &_surname) noexcept
{
    surname = _surname;
}

void Account::withdraw(const Money &howmuch_money)
{
    check_is_enough_money_to_withdraw(howmuch_money);
    summa -= howmuch_money;
}

void Account::deposit(const Money &howmuch_money)
{
    summa += howmuch_money;
}

void Account::change_procent(double new_procent)
{
    check_is_correct_procent(new_procent);
    procent = new_procent;
}

void Account::accrue_procent()
{
    summa += summa * procent;
}

void Account::to_another_exchange(double exchange_rate)
{
    summa *= exchange_rate;
}

void Account::check_is_correct_procent(double procent) const
{
    if (procent < 0) throw negative_procent_exception();
}

void Account::check_is_enough_money_to_withdraw(const Money &withdraw_money) const
{
    if (summa < withdraw_money) throw not_enough_money_exception();
}

string Account::toString() const noexcept
{
    std::stringstream ss;
    ss << surname << " " << account_number << " " << procent << " " << summa;
    return ss.str();
}

bool operator==(const Account &a, const Account &b)
{
    return (a.summa == b.summa);
}
bool operator!=(const Account &a, const Account &b)
{
    return !(a == b);
}
bool operator<(const Account &a, const Account &b)
{
    return (a.summa < b.summa);
}
bool operator>=(const Account &a, const Account &b)
{
    return !(a<b);
}

bool operator>(const Account &a, const Account &b)
{
    return (b < a);
}

bool operator<=(const Account &a, const Account &b)
{
    return !(b < a);
}

ostream& operator<<(ostream &t, const Account &r)
{
    string s;
    s = r.toString();
    return (t << s);
}

istream& operator>>(istream &t, Account &r)
{
    t >> r.surname;
    t >> r.account_number;
    t >> r.procent;
    t >> r.summa;
    return t;
}

void Account::Money::CheckConversionFactor(const double &factor) // -- коэффициент должен быть > 0 -- //
{
    if (factor < 0) throw negative_conversion_factor_exception();
    if (factor <= std::numeric_limits<double>::epsilon()) throw divide_by_zero_exception();
}

string Account::Money::toString() const noexcept // -- перевод денег в строку для вывода -- //
{
    stringstream ss;
    ss << rubles << "," << int(kopecks);
    return ss.str();
}

Account::Money &Account::Money::operator+=(const Money &b)
{
    rubles += b.rubles;
    kopecks += b.kopecks;
    while (kopecks >= 100) {kopecks -= 100; ++rubles;}
    return *this;
}

Account::Money &Account::Money::operator-=(const Money &b)
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

Account::Money &Account::Money::operator/=(const Money &b)
{
    double a_v = fabs(rubles) + (kopecks / 100.0);   // -- абсолютное значение -- //
    double b_v = fabs(b.rubles) + (b.kopecks / 100.0);   // -- абсолютное значение -- //
    CheckConversionFactor(b_v);
    a_v /= b_v;
    rubles = static_cast<uint64_t>(a_v);  // -- целая часть результата - это рубли -- //
    kopecks = static_cast<uint8_t>(round((a_v - rubles) * 100));  // -- дробная часть результата - это копейки -- //
    return *this;
}

Account::Money &Account::Money::operator/=(const double &b)
{
    CheckConversionFactor(b);
    double v = fabs(rubles) + (kopecks / 100.0);   // -- абсолютное значение -- //
    v /= b;
    rubles = static_cast<uint64_t>(v);  // -- целая часть результата - это рубли -- //
    kopecks = static_cast<uint8_t>(round((v - rubles) * 100));  // -- дробная часть результата - это копейки -- //
    return *this;
}

Account::Money &Account::Money::operator*=(const double &b)
{
    CheckConversionFactor(b);
    double v = fabs(rubles) + (kopecks / 100.0);   // -- абсолютное значение -- //
    v *= b;
    rubles = static_cast<uint64_t>(v);  // -- целая часть результата - это рубли -- //
    kopecks = static_cast<uint8_t>(round((v - rubles) * 100));  // -- дробная часть результата - это копейки -- //
    return *this;
}

Account::Money operator+(const Account::Money &a, const Account::Money &b)
{
    Account::Money t = a;
    t += b;
    return t;
}

Account::Money operator-(const Account::Money &a, const Account::Money &b)
{
    Account::Money t = a;
    t -= b;
    return t;
}

Account::Money operator/(const Account::Money &a, const Account::Money &b)
{
    Account::Money t = a;
    t /= b;
    return t;
}

Account::Money operator*(const Account::Money &a, const double &b)
{
    Account::Money t = a;
    t *= b;
    return t;
}

bool operator==(const Account::Money &a, const Account::Money &b)
{
    return ((a.rubles == b.rubles) && (a.kopecks == b.kopecks));
}
bool operator!=(const Account::Money &a, const Account::Money &b)
{
    return !(a == b);
}
bool operator<(const Account::Money &a, const Account::Money &b)
{
    if (a.rubles < b.rubles)
    {
        return true;
    }
    else if (a.rubles == b.rubles)
    {
        return (a.kopecks < b.kopecks);
    }
    return false;
}
bool operator>=(const Account::Money &a, const Account::Money &b)
{
    return !(a<b);
}

bool operator>(const Account::Money &a, const Account::Money &b)
{
    return (b < a);
}

bool operator<=(const Account::Money &a, const Account::Money &b)
{
    return !(b < a);
}

ostream& operator<<(ostream &t, const Account::Money &r)
{
    string s;
    s = r.toString();
    return (t << s);
}

istream& operator>>(istream &t, Account::Money &r)
{
    double input;
    t >> input;
    r.rubles = static_cast<uint64_t>(input);
    r.kopecks = static_cast<uint8_t>(round((input - r.rubles) * 100));
    return t;
}
