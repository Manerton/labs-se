#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include <iostream>
#include "money.h"


class Account                   // -- банковский счёт -- //
{
    std::string surname;        // -- фамилия владельца -- //
    uint32_t account_number;    // -- номер счёта -- //
    double procent;             // -- процент начисления -- //
    Money summa;                // -- сумма в рублях -- //

    void check_is_correct_procent(double procent) const;
    void check_is_enough_money_to_withdraw(const Money& withdraw_money) const;    // -- проверка: хватает ли денег на счете при снятии -- //
public:
    Account(const std::string& _surname = "Ivanov", uint32_t _num = 0, double _procent = 0, const Money& _summa = 0)
        : surname(_surname), account_number(_num), procent(_procent), summa(_summa)
    {
        check_is_correct_procent(_procent); // -- не введен ли отрицательный процент -- //
    }

    std::string toString() const noexcept;

    void change_surname(const std::string& _surname = "Ivanov") noexcept;    // -- сменить владельца -- //
    void withdraw(const Money& howmuch_money = {0,0});  // -- снять сумму денег со счета -- //
    void deposit(const Money& howmuch_money = {0,0});  // -- положить сумму денег на счет -- //
    void change_procent(double new_procent);  // -- поменять процент начисления -- //
    void accrue_procent();  // -- начислить проценты -- //
    void to_another_exchange(double exchange_rate); // -- перевести в другую валюту -- //

    // -- операции сравнения и вывода -- //
    friend bool operator==(const Account &a, const Account &b);
    friend bool operator!=(const Account &a, const Account &b);
    friend bool operator<(const Account &a, const Account &b);
    friend bool operator>=(const Account &a, const Account &b);
    friend bool operator>(const Account &a, const Account &b);
    friend bool operator<=(const Account &a, const Account &b);
    friend std::ostream& operator<<(std::ostream& t, const Account &r);
    friend std::istream& operator>>(std::istream& t, Account &r);
    // -- исключения -- //
    class not_enough_money_exception {}; // -- на счете не хватает денег при снятии -- //
    class negative_procent_exception {};

};

#endif // ACCOUNT_H
