#include "doctest.h"
#include "modulo.h"

// -- непосредственно тесты -
TEST_CASE("Тесты ModuloTest в doctest")
{
    Modulo t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        Modulo two;      	CHECK(t == two);
                            CHECK(two.toString() == "0 mod 10");
        Modulo d(3,7);      CHECK(d.toString() == "3 mod 7");
        Modulo dd(10,6);    CHECK(dd.toString() == "4 mod 6");
        two = dd;           CHECK(two.toString() == "4 mod 6");
        two = {3, 7};      CHECK(d.toString() == "3 mod 7");
        two = {5, 12};      CHECK(d.toString() == "7 mod 11");    	// -- ошибка -- должно быть 5 mod 12 --
    }
    SUBCASE("Тест сложения")              // -- тестирование сложения --
    {
        Modulo one;
        one += 1;
        CHECK(one.toString() == "1 mod 10");
        Modulo two = (one + 1);
        CHECK (two.toString() == "2 mod 10");
        CHECK((1 + one) == two);
        CHECK((two+one).toString() == "3 mod 10");
        CHECK((two+=one).toString() == "3 mod 10");
        CHECK(two.toString() == "3 mod 10");
        Modulo aa(3,7);
        Modulo bb(5,7);
        CHECK ((aa+bb).toString() == "1 mod 7");
        CHECK ((aa+4).toString() == "0 mod 7");
        CHECK ((aa++).toString() == "3 mod 7");
        CHECK (aa.toString() == "4 mod 7");
        CHECK ((++aa).toString() == "5 mod 7");
        CHECK ((1+bb).toString() == "5 mod 7"); // -- ошибка - должно быть 6 mod 7 --
    }
    SUBCASE("Тест вычитания")          // -- тестирования вычитания --
    {
        Modulo one(3, 8);
        one -= 2;
        CHECK(one.toString() == "1 mod 8");

        Modulo zero = (one - 1);
        CHECK (zero.toString() == "0 mod 8");
        CHECK((1 - one) == zero);
        Modulo four(4,8);
        CHECK((four-one).toString() == "3 mod 8");
        CHECK((four-=one).toString() == "3 mod 8");
        CHECK(four.toString() == "3 mod 8");
        Modulo aa(3,7);
        Modulo bb(5,7);
        CHECK ((aa-bb).toString() == "5 mod 7");
        CHECK ((aa-4).toString() == "6 mod 7");
        CHECK ((aa--).toString() == "3 mod 7");
        CHECK (aa.toString() == "2 mod 7");
        CHECK ((--aa).toString() == "1 mod 7");
        CHECK ((1-bb).toString() == "5 mod 7"); // -- ошибка - должно быть 3 mod 7 --
    }
    SUBCASE("Тест умножения")          // -- тестирования умножения --
    {
        Modulo three(1, 6);
        three *= 3;
        CHECK(three.toString() == "3 mod 6");

        Modulo zero = (three * 6);
        CHECK (zero.toString() == "0 mod 6");
        CHECK((2 * three) == zero);
        Modulo four(4,6);
        CHECK((four*three).toString() == "0 mod 6");
        CHECK((four*=three).toString() == "0 mod 6");
        CHECK(four.toString() == "0 mod 6");
        Modulo aa(3,7);
        Modulo bb(5,7);
        CHECK ((aa*bb).toString() == "1 mod 7");
        CHECK ((aa*4).toString() == "5 mod 7");
        CHECK ((2*bb).toString() == "5 mod 7"); // -- ошибка - должно быть 3 mod 7 --
    }
    SUBCASE("Тест деления")          // -- тестирования деления --
    {
        Modulo two(3, 7);
        two /= 5;
        CHECK(two.toString() == "2 mod 7");

        Modulo six = (two / 5);
        CHECK (six.toString() == "6 mod 7");
        CHECK((5 / six) == two);
        Modulo four(4,7);
        CHECK((four/two).toString() == "2 mod 7");
        CHECK((four/=two).toString() == "2 mod 7");
        CHECK(four.toString() == "2 mod 7");
        Modulo aa(3,7);
        Modulo bb(4,7);
        CHECK ((aa/bb).toString() == "3 mod 7"); // -- ошибка - должно быть 6 mod 7 --
    }
    SUBCASE("Тест сравнения")          // -- проверка сравнения --
    {
        Modulo a = {1, 5};
        Modulo b = {3, 5};
        Modulo c = {1, 5};
        Modulo d = {4, 5};

        CHECK(a < b);
        CHECK(d > b);
        CHECK(b < a);       // -- ошибка --
        CHECK(a <= b);
        CHECK(a <= c);
        CHECK(b >= c);
        CHECK(a >= c);
        CHECK(a == c);
        CHECK(b == c);      // -- ошибка --
    }
}
// -- тестирование исключений --
TEST_CASE("Тест исключений")
{
    CHECK_THROWS([&](){Modulo F(5,0);}()); // -- правильно - генерит исключения --

    CHECK_THROWS([&](){Modulo a(1,5);
                       Modulo b(3,7);
                       Modulo c = a+b;}()); // -- правильно - генерит исключения --

    CHECK_THROWS([&](){Modulo g(3,8);
                       Modulo k(4,8);
                       Modulo c = g/k;}()); // -- правильно - генерит исключения --
}
