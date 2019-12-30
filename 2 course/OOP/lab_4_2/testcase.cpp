#include "doctest.h"
#include "account.h"

TEST_CASE("Тесты класса Account в doctest")
{
    Account t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        Account d("Gagarin",104,0.03,155.64);
        CHECK(d.toString() == "Gagarin 104 0.03 155,64");

        Account two;
        two = d;
        CHECK(two.toString() == "Gagarin 104 0.03 155,64");
        two = {"Bobrov", 1, 0.2, 35.0};
        CHECK(two.toString() == "Bobrov 1 0.2 35,0");
    }

    SUBCASE("Тест смены владельца счета")
    {
        Account d("Gagarin",104,0.03,155.64);
        d.change_surname("Petrov");
        CHECK(d.toString() == "Petrov 104 0.03 155,64");
    }
    SUBCASE("Тест снятия суммы со счета")
    {
        Account d("Gagarin",104,0.03,155.64);
        d.withdraw(15.33);
        CHECK(d.toString() == "Gagarin 104 0.03 140,31");
    }
    SUBCASE("Тест депозита суммы на счет")
    {
        Account d("Gagarin",104,0.03,155.64);
        d.deposit(15.33);
        CHECK(d.toString() == "Gagarin 104 0.03 170,97");
    }
    SUBCASE("Тест начисления процентов")
    {
        Account d("Gagarin",104,0.03,155.64);
        d.accrue_procent();
        CHECK(d.toString() == "Gagarin 104 0.03 160,31");
    }
    SUBCASE("Тест перевода суммы в другую валюту")
    {
        Account d("Gagarin",104,0.03,155.64);
        d.to_another_exchange(1/63.2);
        CHECK(d.toString() == "Gagarin 104 0.03 2,46");
    }
    SUBCASE("Тест сравнения")          // -- проверка сравнения --
    {
        Account a("Gagarin",104,0.03,155.64);
        Account b = {"Bobrov", 1, 0.2, 35.0};
        Account c("Petrov",4,0.05,155.0);
        Account d("Ivanov",105,0.03,155.64);

        CHECK(b < a);
        CHECK(c < a);
        CHECK(c > b);
        CHECK(a <= d);
        CHECK(b <= d);
        CHECK(a >= c);
        CHECK(d >= a);
        CHECK(a == d);
        CHECK(a != c);
    }
    SUBCASE("Тест исключений")          // -- проверка сравнения --
    {
        CHECK_THROWS([&](){Account a("Gagarin",104,0.03,155.64);
            a.withdraw(200.55);}()); // -- правильно - генерит исключения --

        CHECK_THROWS([&](){Account a("Gagarin",104,-0.03,155.64);}()); // -- правильно - генерит исключения --
    }
}

