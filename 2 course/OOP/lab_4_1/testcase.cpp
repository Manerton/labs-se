#include "doctest.h"
#include "money.h"
#include "account.h"

// -- непосредственно тесты -
TEST_CASE("Тесты класс Money в doctest")
{
    Money t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        Money two;      	CHECK(t == two);
                            CHECK(two.toString() == "0,0");
        Money d(3,7);      CHECK(d.toString() == "3,7");
        Money dd(10,105);    CHECK(dd.toString() == "11,5");
        two = dd;           CHECK(two.toString() == "11,5");
        two = {3, 7};      CHECK(two.toString() == "3,7");
        two = {5, 12};      CHECK(two.toString() == "5,12");
    }

    SUBCASE("Тест сложения")              // -- тестирование сложения --
    {
        Money one;
        one += 1;
        CHECK(one.toString() == "1,0");
        Money two = (one + 1);
        CHECK (two.toString() == "2,0");
        CHECK((1 + one) == two);
        CHECK((two+one).toString() == "3,0");
        CHECK((two+=one).toString() == "3,0");
        CHECK((two).toString() == "3,0");
        Money aa(3,7);
        Money bb(5,7);
        Money cc(1412,94);
        CHECK ((aa+bb).toString() == "8,14");
        CHECK ((aa+4).toString() == "7,7");
        CHECK ((cc+bb).toString() == "1418,1");
        CHECK ((cc+aa).toString() == "1416,1");
    }
    SUBCASE("Тест вычитания")          // -- тестирования вычитания --
    {
        Money one(3, 8);
        one -= 2;
        CHECK(one.toString() == "1,8");
        Money zero = (one - 1);
        CHECK (zero.toString() == "0,8");
        Money four(4,8);
        CHECK((four-one).toString() == "3,0");
        CHECK((four-=one).toString() == "3,0");
        CHECK(four.toString() == "3,0");
        Money aa(3,7);
        Money bb(2,14);
        CHECK ((aa-bb).toString() == "0,93");
        CHECK ((aa-3).toString() == "0,7");
        CHECK ((14-bb).toString() == "11,86");
    }
    SUBCASE("Тест умножения")          // -- тестирования умножения --
    {
        Money three(1, 6);
        three *= 3;
        CHECK(three.toString() == "3,18");

        Money bbb = (three * 63);
        CHECK (bbb.toString() == "200,34");
        CHECK((three *=63) == bbb);
        Money four(4,6);
        CHECK((four*5).toString() == "20,30");
        CHECK((four*=5).toString() == "20,30");
        CHECK(four.toString() == "20,30");
    }
    SUBCASE("Тест деления")          // -- тестирования деления --
    {
        Money two(3, 7);
        two /= 5;
        CHECK(two.toString() == "0,61");

        Money six = (two / 5);
        CHECK (six.toString() == "0,12");
        CHECK((5 / six).toString() == "41,67");
        Money faa(400,35);
        Money dollar(63,74);
        Money ss(1,5);
        CHECK((faa/dollar).toString() == "6,28");
        CHECK((faa/=two).toString() == "656,31");
        CHECK(faa.toString() == "656,31");
    }
    SUBCASE("Тест сравнения")          // -- проверка сравнения --
    {
        Money a = {154, 5};
        Money b = {343, 5};
        Money c = {154, 56};
        Money d = {455, 0};
        Money q = {343, 5};

        CHECK(a < b);
        CHECK(d > b);
        CHECK(a < c);
        CHECK(a <= b);
        CHECK(b <= q);
        CHECK(b >= c);
        CHECK(b >= c);
        CHECK(b == q);
        CHECK(a != c);
    }
    SUBCASE("Тест исключений")          // -- проверка сравнения --
    {
        CHECK_THROWS([&](){Money a(1,5);
                           [[maybe_unused]] Money c = a/0.0;}()); // -- правильно - генерит исключения --

        CHECK_THROWS([&](){Money g(3,8);
                           g/=(-5.2);}()); // -- правильно - генерит исключения --
        CHECK_THROWS([&](){Money g(3,8);
                           [[maybe_unused]] Money c = g*(-5);}()); // -- правильно - генерит исключения --
    }
}
TEST_CASE("Тесты класса Account в doctest")
{
    Account t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        Account d("Gagarin",104,0.03,{155,64});
        CHECK(d.toString() == "Gagarin 104 0.03 155,64");

        Account two;
        two = d;
        CHECK(two.toString() == "Gagarin 104 0.03 155,64");
        two = {"Bobrov", 1, 0.2, Money(35,0)};
        CHECK(two.toString() == "Bobrov 1 0.2 35,0");
    }

    SUBCASE("Тест смены владельца счета")
    {
        Account d("Gagarin",104,0.03,{155,64});
        d.change_surname("Petrov");
        CHECK(d.toString() == "Petrov 104 0.03 155,64");
    }
    SUBCASE("Тест снятия суммы со счета")
    {
        Account d("Gagarin",104,0.03,{155,64});
        d.withdraw(Money(15,33));
        CHECK(d.toString() == "Gagarin 104 0.03 140,31");
    }
    SUBCASE("Тест депозита суммы на счет")
    {
        Account d("Gagarin",104,0.03,{155,64});
        d.deposit(Money(15,33));
        CHECK(d.toString() == "Gagarin 104 0.03 170,97");
    }
    SUBCASE("Тест начисления процентов")
    {
        Account d("Gagarin",104,0.03,{155,64});
        d.accrue_procent();
        CHECK(d.toString() == "Gagarin 104 0.03 160,31");
    }
    SUBCASE("Тест перевода суммы в другую валюту")
    {
        Account d("Gagarin",104,0.03,{155,64});
        d.to_another_exchange(1/63.2);
        CHECK(d.toString() == "Gagarin 104 0.03 2,46");
    }
    SUBCASE("Тест сравнения")          // -- проверка сравнения --
    {
        Account a("Gagarin",104,0.03,{155,64});
        Account b = {"Bobrov", 1, 0.2, Money(35,0)};
        Account c("Petrov",4,0.05,{155,0});
        Account d("Ivanov",105,0.03,{155,64});

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
        CHECK_THROWS([&](){Account a("Gagarin",104,0.03,{155,64});
                           a.withdraw(Money(200,55));}()); // -- правильно - генерит исключения --

        CHECK_THROWS([&](){Account a("Gagarin",104,-0.03,{155,64});}()); // -- правильно - генерит исключения --
    }
}

