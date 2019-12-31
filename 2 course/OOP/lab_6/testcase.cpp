#include "doctest.h"
#include <iostream>
#include "money.h"
#include "model.h"


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

TEST_CASE("Тесты класс Model в doctest")
{
    Model t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        Model two;      	CHECK(t == two);
        CHECK(two.toString() == "default x64 3500 8192 122880 1024 0,0 1 0.25");
        Model d("acer", "intel-x86",2800,4096,122880,2048,Money(25567,99),5,0.03);
        CHECK(d.toString() == "acer intel-x86 2800 4096 122880 2048 25567,99 5 0.03");
        std::cout << d; // -- проверим вывод -- //
        d = two;
        CHECK(d.toString() == "default x64 3500 8192 122880 1024 0,0 1 0.25");
    }

    SUBCASE("Тест цен")              // -- тестирование сложения --
    {
        Model d("acer", "intel-x86",2800,4096,122880,2048,Money(25567,99),5,0.03);
        d.set_price(Money(44113,10));
        CHECK(d.toString() == "acer intel-x86 2800 4096 122880 2048 44113,10 5 0.03");
        CHECK(d.get_price().toString() == "44113,10");
        CHECK(d.get_price_for_N_models(3).toString() == "132339,30");
        CHECK(d.calculate_sale().toString() == "1323,39");
        CHECK(d.calculate_price_with_sale().toString() == "42789,71");
    }
    SUBCASE("Тест изменения количества экземпляров")          // -- тестирования вычитания --
    {
        Model d("acer", "intel-x86",2800,4096,122880,2048,Money(25567,99),5,0.03);
        d += 10;
        CHECK(d.toString() == "acer intel-x86 2800 4096 122880 2048 25567,99 15 0.03");
        d -= 3;
        CHECK(d.toString() == "acer intel-x86 2800 4096 122880 2048 25567,99 12 0.03");
    }
    SUBCASE("Тест сравнения")          // -- проверка сравнения --
    {
        Model a("acer", "intel-x86",2800,4096,122880,2048,Money(25567,99),5,0.03);
        Model b("hp", "amd-x86",3300,2048,122880,4096,Money(27142,44),5,0.03);
        Model c("asus", "intel-x64",3700,8192,245760,4096,Money(25567,99),5,0.03);

        CHECK(a < b);
        CHECK(b > c);
        CHECK(a <= c);
        CHECK(b >= a);
        CHECK(b >= c);
        CHECK(a == c);
        CHECK(a != b);
    }
    SUBCASE("Тест исключений")          // -- проверка сравнения --
    {
        CHECK_THROWS([&](){Model d("acer", "intel-x86",2800,4096,122880,2048,Money(25567,99),5,-0.03);
                           }()); // -- правильно - генерит исключения --

    }
}
