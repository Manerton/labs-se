#include <sstream>
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
