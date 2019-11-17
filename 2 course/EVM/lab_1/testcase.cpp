#include "doctest.h"
#include "big_int.h"

// -- непосредственно тесты -
TEST_CASE("Тесты длинной арифметики в doctest")
{
    SUBCASE("Тест функции tostring")
    {
        big_int a = {sign::minus, 3, 5, 7};
        CHECK(toString(a) == "- 357");
        a = {sign::plus, 1, 3, 5, 1, 7, 9};
        CHECK(toString(a) == "135 179");
    }

    SUBCASE("Тест функции stobi")
    {
        big_int a = stobi("-357");
        CHECK(toString(a) == "- 357");
        a = stobi("135179");
        CHECK(toString(a) == "135 179");
    }
    SUBCASE("Тест функции optimize_string")
    {
        big_int a = stobi("-0007");
        CHECK(toString(a) == "-7");
        a = stobi("00000005");
        CHECK(toString(a) == "5");
    }
    SUBCASE("Тест сложения")              // -- тестирование сложения --
    {
        big_int a = stobi("15");
        big_int b = stobi("5");
        big_int c = a+b;
        CHECK(toString(c) == "20");
        c = b + a;
        CHECK(toString(c) == "20");

        a = stobi("15");
        b = stobi("-5");
        c = a+b;
        CHECK(toString(c) == "10");
        c = b + a;
        CHECK(toString(c) == "10");

        a = stobi("15");
        b = stobi("-10");
        c = a+b;
        CHECK(toString(c) == "5");
        c = b + a;
        CHECK(toString(c) == "5");

        a = stobi("15");
        b = stobi("-7");
        c = a+b;
        CHECK(toString(c) == "8");
        c = b + a;
        CHECK(toString(c) == "8");

        a = stobi("15");
        b = stobi("-15");
        c = a+b;
        CHECK(toString(c) == "0");
        c = b + a;
        CHECK(toString(c) == "0");

        a = stobi("15");
        b = stobi("-25");
        c = a+b;
        CHECK(toString(c) == "-10");
        c = b + a;
        CHECK(toString(c) == "-10");

        a = stobi("15");
        b = stobi("-100");
        c = a+b;
        CHECK(toString(c) == "-85");
        c = b + a;
        CHECK(toString(c) == "-85");

        a = stobi("15");
        b = stobi("-1001");
        c = a+b;
        CHECK(toString(c) == "- 986");
        c = b + a;
        CHECK(toString(c) == "- 986");

    }
    SUBCASE("Тест вычитания")          // -- тестирования вычитания --
    {
        big_int a = stobi("15");
        big_int b = stobi("5");
        big_int c = a-b;
        CHECK(toString(c) == "10");
        c = b - a;
        CHECK(toString(c) == "-10");

        a = stobi("15");
        b = stobi("-5");
        c = a-b;
        CHECK(toString(c) == "20");
        c = b - a;
        CHECK(toString(c) == "-20");

        a = stobi("15");
        b = stobi("-10");
        c = a-b;
        CHECK(toString(c) == "25");
        c = b - a;
        CHECK(toString(c) == "-25");

        a = stobi("15");
        b = stobi("-7");
        c = a-b;
        CHECK(toString(c) == "-8"); // ошибка! -- должно быть 22
        c = b - a;
        CHECK(toString(c) == "-22");

        a = stobi("15");
        b = stobi("-15");
        c = a-b;
        CHECK(toString(c) == "30");
        c = b - a;
        CHECK(toString(c) == "-30");

        a = stobi("15");
        b = stobi("-25");
        c = a-b;
        CHECK(toString(c) == "40");
        c = b - a;
        CHECK(toString(c) == "-40");

        a = stobi("15");
        b = stobi("-100");
        c = a-b;
        CHECK(toString(c) == "115");
        c = b - a;
        CHECK(toString(c) == "- 115");

        a = stobi("15");
        b = stobi("-1001");
        c = a-b;
        CHECK(toString(c) == "1 016");
        c = b - a;
        CHECK(toString(c) == "-1 016");
    }
    SUBCASE("Тест деления")          // -- тестирования деления --
    {
        big_int a = stobi("153");
        big_int b = stobi("7");
        big_int res;
        fraction(a,b,res);
        CHECK(toString(res) == "21");
        CHECK(toString(a) == "6");
        CHECK(toString(b) == "7");

        a = stobi("-153");
        b = stobi("7");
        fraction(a,b,res);
        CHECK(toString(res) == "-21");
        CHECK(toString(a) == "6");
        CHECK(toString(b) == "7");

        a = stobi("153");
        b = stobi("-7");
        fraction(a,b,res);
        CHECK(toString(res) == "-21");
        CHECK(toString(a) == "6");
        CHECK(toString(b) == "7");

        a = stobi("-153");
        b = stobi("-7");
        fraction(a,b,res);
        CHECK(toString(res) == "21");
        CHECK(toString(a) == "6");
        CHECK(toString(b) == "7");

        a = stobi("200");
        b = stobi("100");
        fraction(a,b,res);
        CHECK(toString(res) == "2");
        CHECK(toString(a) == "0");

        a = stobi("50");
        b = stobi("5");
        fraction(a,b,res);
        CHECK(toString(res) == "5"); // ошибка! -- должно быть 10
        CHECK(toString(a) == "0");

        a = stobi("5235240630456");
        b = stobi("153052035");
        fraction(a,b,res);
        CHECK(toString(res) == "34 205");
        CHECK(toString(a) == "95 773 281");
        CHECK(toString(b) == "153 052 035");

        a = stobi("-5235240630456");
        b = stobi("153052035");
        fraction(a,b,res);
        CHECK(toString(res) == "-34 205");
        CHECK(toString(a) == "95 773 281");
        CHECK(toString(b) == "153 052 035");

        a = stobi("5235240630456");
        b = stobi("-153052035");
        fraction(a,b,res);
        CHECK(toString(res) == "-34 205");
        CHECK(toString(a) == "95 773 281");
        CHECK(toString(b) == "153 052 035");

        a = stobi("-5235240630456");
        b = stobi("-153052035");
        fraction(a,b,res);
        CHECK(toString(res) == "34 205");
        CHECK(toString(a) == "95 773 281");
        CHECK(toString(b) == "153 052 035");

        a = stobi("7");
        b = stobi("153");
        fraction(a,b,res);
        CHECK(toString(res) == "0");
        CHECK(toString(a) == "7");
        CHECK(toString(b) == "153");

        a = stobi("-7");
        b = stobi("153");
        fraction(a,b,res);
        CHECK(toString(res) == "0");
        CHECK(toString(a) == "-7");
        CHECK(toString(b) == "153");

        a = stobi("7");
        b = stobi("-153");
        fraction(a,b,res);
        CHECK(toString(res) == "0");
        CHECK(toString(a) == "-7");
        CHECK(toString(b) == "153");

        a = stobi("-7");
        b = stobi("-153");
        fraction(a,b,res);
        CHECK(toString(res) == "0");
        CHECK(toString(a) == "-7");      // ошибка! -- должно быть 7
        CHECK(toString(b) == "153");

        a = stobi("153052035");
        b = stobi("5235240630456");
        fraction(a,b,res);
        CHECK(toString(res) == "0");
        CHECK(toString(a) == "153 052 035");
        CHECK(toString(b) == "5 235 240 630 456");

        a = stobi("-153052035");
        b = stobi("5235240630456");
        fraction(a,b,res);
        CHECK(toString(res) == "0");
        CHECK(toString(a) == "- 153 052 035");
        CHECK(toString(b) == "5 235 240 630 456");

        a = stobi("153052035");
        b = stobi("-5235240630456");
        fraction(a,b,res);
        CHECK(toString(res) == "0");
        CHECK(toString(a) == "- 153 052 035");
        CHECK(toString(b) == "5 235 240 630 456");

        a = stobi("-153052035");
        b = stobi("-5235240630456");
        fraction(a,b,res);
        CHECK(toString(res) == "0");
        CHECK(toString(a) == "153 052 035");
        CHECK(toString(b) == "5 235 240 630 456");

    }
    SUBCASE("Тест сравнений")          // -- проверка сравнения --
    {
        big_int a = stobi("15");
        big_int b = stobi("15");
        CHECK((a == b));

        b = stobi("10");
        CHECK((a > b));
        CHECK((a >= b));
        CHECK((a != b));
        CHECK((b < a));

        a = stobi("-15");

        CHECK((a < b));
        CHECK((a <= b));
        CHECK((a != b));
        CHECK((a > b));     // ошибка!

        a = stobi("15");
        b = stobi("100");
        CHECK((a < b));
        CHECK((a <= b));
        CHECK((a != b));
        CHECK((b > a));

        a = stobi("-15");
        b = stobi("-100");
        CHECK((a > b));
        CHECK((a >= b));
        CHECK((a != b));
        CHECK((b < a));
    }
}

TEST_CASE("Тест исключений")
{
    CHECK_THROWS([&](){big_int a = stobi("-3535gsfg");}()); // -- правильно - генерит исключения --

    CHECK_THROWS([&](){big_int a = stobi("15");
                       big_int b = stobi("0");
                       big_int res;
                       fraction(a,b,res);}()); // -- правильно - генерит исключения --
}

