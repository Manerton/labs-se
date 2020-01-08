#include "doctest.h"
#include "longl.h"
#include <bitset>
#include <iostream>

// -- непосредственно тесты -
TEST_CASE("Тесты класса LongL в doctest")
{
    LongL t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание")
    {
        LongL two;
        CHECK((t == two));
        //CHECK(two.toString() == "0");

        LongL a("79228162514264337593543950335");   // -- это максимум для 2 ^ 96 -- //
        CHECK(a.toString() == "79228162514264337593543950335");
        a = {"141423431412"};
        CHECK(a.toString() == "141423431412");
        a = {42954604143};
        CHECK(a.toString() == "42954604143");
        a = {64, 0};    // -- это 2 ^ 70, так как 64 - это шестой бит в старшей части и 64 нулевых бита в младшей -- //
        CHECK(a.toString() == "1180591620717411303424");  
//        LongL b("4142352352345");
//        LongL c("14142343141412");
//        CHECK((b*c).toString() == "58582568379498175184811140");
    }   
    SUBCASE("Тест сдвигов")
    {
        LongL a(2352351,4512352345634131134);
        // -- получим битовую строку из числа a, сдвинутую на 13 бит влево -- //
        std::string bitstr_a_shifted = ( ((std::bitset<96>(a.get_high()) << 64) | (std::bitset<96>(a.get_low()))) << 13 ).to_string();
        // -- сдвинем число на 13 бит влево -- //
        a <<= 13;
        // -- сверим результат -- //
        std::string bitstr_a = ( (std::bitset<96>(a.get_high()) << 64) | (std::bitset<96>(a.get_low())) ).to_string();
        CHECK(bitstr_a == bitstr_a_shifted);

        a = {2352351,4512352345634131134};
        // -- получим битовую строку из числа a, сдвинутую на 70 бит влево -- //
        bitstr_a_shifted = ( ((std::bitset<96>(a.get_high()) << 64) | (std::bitset<96>(a.get_low()))) << 70 ).to_string();
        // -- сдвинем число на 13 бит влево -- //
        a <<= 70;
        // -- сверим результат -- //
        bitstr_a = ( (std::bitset<96>(a.get_high()) << 64) | (std::bitset<96>(a.get_low())) ).to_string();
        CHECK(bitstr_a == bitstr_a_shifted);

        a = {2352351,4512352345634131134};
        // -- получим битовую строку из числа a, сдвинутую на 13 бит вправо -- //
        bitstr_a_shifted = ( ((std::bitset<96>(a.get_high()) << 64) | (std::bitset<96>(a.get_low()))) >> 13 ).to_string();
        // -- сдвинем число на 13 бит вправо -- //
        a >>= 13;
        // -- сверим результат -- //
        bitstr_a = ( (std::bitset<96>(a.get_high()) << 64) | (std::bitset<96>(a.get_low())) ).to_string();
        CHECK(bitstr_a == bitstr_a_shifted);

        a = {2352351,4512352345634131134};
        // -- получим битовую строку из числа a, сдвинутую на 70 бит вправо -- //
        bitstr_a_shifted = ( ((std::bitset<96>(a.get_high()) << 64) | (std::bitset<96>(a.get_low()))) >> 70 ).to_string();
        // -- сдвинем число на 70 бит вправо -- //
        a >>= 70;
        // -- сверим результат -- //
        bitstr_a = ( (std::bitset<96>(a.get_high()) << 64) | (std::bitset<96>(a.get_low())) ).to_string();
        CHECK(bitstr_a == bitstr_a_shifted);
    }
    SUBCASE("Тест сложения")              // -- тестирование сложения --
    {
        LongL a ("1180591620717411303424");
        LongL b ("262091339799265309360128");
        LongL c = a + b;
        CHECK(c.toString() == "263271931419982720663552");
        c = b + a;
        CHECK(c.toString() == "263271931419982720663552");
        c+=0;
        CHECK(c.toString() == "263271931419982720663552");
        a = {"155"};
        b = {"1313"};
        b+=a;
        CHECK(b.toString() == "1468");
    }
    SUBCASE("Тест операцй инкремента и декремента")              // -- тестирование сложения --
    {
        LongL a ("1180591620717411303424");
        ++a;
        CHECK(a.toString() == "1180591620717411303425");
        --a;
        CHECK(a.toString() == "1180591620717411303424");
        CHECK((a++).toString() == "1180591620717411303424");
        CHECK(a.toString() == "1180591620717411303425");
    }
    SUBCASE("Тест вычитания")
    {
        LongL a ("262091339799265309360128");
        LongL b ("1180591620717411303424");
        LongL c = a - b;
        CHECK(c.toString() == "260910748178547898056704");
        c-=0;
        CHECK(c.toString() == "260910748178547898056704");
        a = {"155"};
        b = {"1313"};
        b-=a;
        CHECK(b.toString() == "1158");
        a = {0};
        a-=1;
        CHECK(a.toString() == "79228162514264337593543950335");
    }
    SUBCASE("Тест умножения")
    {
        LongL a ("4142352352345");
        LongL b ("14142343141412");
        LongL c = a * b;
        CHECK(c.toString() == "58582568379498175184811140");
        c = b * a;
        CHECK(c.toString() == "58582568379498175184811140");
        a = {"155"};
        b = {"1313"};
        b*=a;
        CHECK(b.toString() == "203515");
        a *= {0};
        CHECK(a.toString() == "0");
    }
    SUBCASE("Тест деления")
    {
        LongL a ("79228162514264337593543950335");
        LongL b ("10");
        LongL c = a / b;
        CHECK(c.toString() == "7922816251426433759354395033");
        c = a / 1;
        CHECK(c.toString() == "79228162514264337593543950335");
        c = a / 14142343141412;
        CHECK(c.toString() == "5602194892462073");
        a = {"155"};
        b = {"1313"};
        c = a / b;
        CHECK(c.toString() == "0");
        c = a % b;
        CHECK(a.toString() == "155");
    }
    SUBCASE("Тест сравнений")
    {
        LongL a ("79228162514264337593543950335");
        LongL b ("79228162514264337593543950321");
        LongL c ("79228162514264337593543950335");
        LongL d ("79228162514264337");
        LongL e ("41343242341341423");
        CHECK((a >= b));
        CHECK((a >= c));
        CHECK((a == c));
        CHECK((b != c));
        CHECK((b > d));
        CHECK((e < d));
        CHECK((e <= b));
        CHECK((d > e));
    }
}

TEST_CASE("Тест исключений")
{
    CHECK_THROWS_AS([&](){LongL a ("-3535gsfg");}(),LongL::not_decimal_exception); // -- некорректный ввод -- //
    CHECK_THROWS_AS([&](){LongL a ("79228162514264337593543950335141423");}(),LongL::overflow_exception); // -- слишком длинное число -- //
    CHECK_THROWS_AS([&](){LongL a = 15;
                       LongL b = 0;
                       LongL res;
                       res = a / b;}(),LongL::zero_divide_exception); // -- правильно - генерит исключения --
}

