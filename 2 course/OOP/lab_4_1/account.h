#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>


class Account   // -- банковский счёт -- //
{
    std::string surname; // -- фамилия владельца -- //
    int account_number; // -- номер счёта -- //
    int interest_charges; // -- процент начисления -- //
    int summa; // -- сумма в рублях -- //
public:
    Account();
};

#endif // ACCOUNT_H
