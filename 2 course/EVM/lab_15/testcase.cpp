#include "doctest.h"
#include "array.h"
#include "five.h"
#include "bitstring.h"
#include <iostream>



// -- непосредственно тесты -
TEST_CASE("Тесты класс Array в doctest")
{
    Array t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        Array two;
        CHECK(t == two);
        CHECK(two.get_size() == 0);
        CHECK(two.get_data() == nullptr);
        Array dz(2);      CHECK(dz.toString() == "0 0");
        CHECK(dz.get_size() == 2);
        Array d(3,7);      CHECK(d.toString() == "7 7 7");
        Array dd{4,2,14};    CHECK(dd.toString() == "4 2 14");
        CHECK(dd.get_size() == 3);
        Array dstr("124523"); CHECK(dstr.toString() == "1 2 4 5 2 3");
        two = dd;           CHECK(two.toString() == "4 2 14");
        two = {3, 7};      CHECK(two.toString() == "3 7");
        std::cout << two;
        two = {};      CHECK(two.get_size() == 0);
        Array zz(d);    CHECK(zz.toString() == "7 7 7");
        zz.push_back(5); CHECK(zz.toString() == "7 7 7 5");
    }

    SUBCASE("Тест индексации и удаления")              // -- тестирование сложения --
    {
        Array dstr("124523");
        CHECK(dstr[0] == 1);
        CHECK(dstr[1] == 2);
        CHECK(dstr[4] == 2);
        dstr.erase(dstr.begin(), dstr.end()-1);
        CHECK(dstr.get_size() == 1);
        CHECK(dstr[0] == 3);
    }
    SUBCASE("Тест сравнения")          // -- проверка сравнения --
    {
        Array a = {8, 5};
        Array b = {4, 9};
        Array c = {8, 5};
        Array d = {1, 2, 3};

        CHECK(a == c);
        CHECK(a <= c);
        CHECK(b < c);
        CHECK(a > b);
        CHECK(d > a);
        CHECK(d >= b);
        CHECK(a != b);
        CHECK(b != c);
    }
    SUBCASE("Тест исключений")          // -- проверка сравнения --
    {
        CHECK_THROWS([&](){Array a(3,5);
                           a[3];}()); // -- индекс за диапазоном -- // //

        CHECK_THROWS([&](){Array g(0,3);
                           }()); // -- нулевой размер -- //
    }
}

TEST_CASE("Тесты класса Five в doctest")
{
    Five t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        Five two;
        CHECK(t == two);
        CHECK(two.get_size() == 0);
        CHECK(two.get_data() == nullptr);
        Five dz(2);      CHECK(dz.toString() == "0 0");
        CHECK(dz.get_size() == 2);
        Five d(3,3);      CHECK(d.toString() == "3 3 3");
        Five ddd(3,4);      CHECK(ddd.toString() == "4 4 4");
        Five dd{1,2,0,3,1,4};    CHECK(dd.toString() == "1 2 0 3 1 4");
        Five zz(dd);    CHECK(zz.toString() == "1 2 0 3 1 4");
        CHECK(dd.get_size() == 6);
        Five dstr("120423"); CHECK(dstr.toString() == "1 2 0 4 2 3");
        two = dd;           CHECK(two.toString() == "1 2 0 3 1 4");
        two = {4,2};      CHECK(two.toString() == "4 2");
        two = {};      CHECK(two.get_size() == 0);
    }

    SUBCASE("Тест сложения")              // -- тестирование сложения --
    {
        Five a = {1,2};
        Five b =   {2};
        Five c = a+b;
        CHECK(c.toString() == "1 4");
        c = b + a;
        CHECK(c.toString() == "1 4");

        a = {"1213"};
        b =  {"112"};
        c = a+b;
        CHECK(c.toString() == "1 3 3 0");
        c = b + a;
        CHECK(c.toString() == "1 3 3 0");

        a = {3,3};
        b = {4,4};
        c = a+b;
        CHECK(c.toString() == "1 3 2");
        c = b + a;
        CHECK(c.toString() == "1 3 2");

        a = {0};
        b = {1,2,1,4,1};
        c = a+b;
        CHECK(c.toString() == "1 2 1 4 1");
        c = b + a;
        CHECK(c.toString() == "1 2 1 4 1");

        a =  {"112131"};
        b = {"1433213"};
        c = a+b;
        CHECK(c.toString() == "2 1 0 0 3 4 4");
        c = b + a;
        CHECK(c.toString() == "2 1 0 0 3 4 4");
    }
    SUBCASE("Тест вычитания")              // -- тестирование вычитания --
    {
        Five a = {1,2};
        Five b =   {2};
        Five c = a-b;
        CHECK(c.toString() == "1 0");

        a = {"1214"};
        b = {"112"};
        c = a - b;
        CHECK(c.toString() == "1 1 0 2");

        a = {3,3};
        b = {3,3};
        c = a-b;
        CHECK(c.toString() == "0");
        c = b - a;
        CHECK(c.toString() == "0");

        a = {0};
        b = {1,2,1,3,1};
        c = b - a;
        CHECK(c.toString() == "1 2 1 3 1");
    }
    SUBCASE("Тест умножения")              // -- тестирование умножения -- //
    {
        Five a = {1,2,4,2};
        Five b = {3,2};
        a *= b;
        CHECK(a.toString() == "1 0 1 3 4 4");
        a = {"12321342"};
        b = {"31412"};
        b *= a;
        CHECK(b.toString() == "1 0 0 4 3 2 1 3 2 2 0 0 4");
        a = {"31013101201212030121021321203121312013"};
        b = {"310131012012120301210213212031213120"};
        a *= b;
        Five res("20130231222443434000323441014422203441233043221122430414223334020140321110");
        CHECK(res == a);
    }
    SUBCASE("Тест деления")              // -- тестирование умножения -- //
    {
        Five a = {"123213412"};
        Five b = {"12"};
        a /= b;
        Five res = {("10221442")};
        CHECK(res == a);

        a = {"12321331212"};
        b = {"1213"};
        a /= b;
        res = {("10111430")};
        CHECK(res == a);

        a = {"1213"};
        b = {"1213"};
        a /= b;
        res = {1};
        CHECK(res == a);

        a = {"3"};
        b = {"1213"};
        a /= b;
        res = {0};
        CHECK(res == a);

        a = {"0"};
        b = {"1213"};
        a /= b;
        res = {0};
        CHECK(res == a);

        a = {"1213"};
        b = {"1"};
        a /= b;
        res = {1,2,1,3};
        CHECK(res == a);
    }
    SUBCASE("Тест сравнения")          // -- проверка сравнения --
    {
        Five a = {1, 3};
        Five b = {3, 2};
        Five c = {1, 3};
        Five d = {1, 2, 4};

        CHECK(a == c);
        CHECK(a <= c);
        CHECK(b > c);
        CHECK(a < b);
        CHECK(d > a);
        CHECK(d >= b);
        CHECK(a != b);
        CHECK(b != c);
    }
    SUBCASE("Тест исключений")          // -- проверка сравнения --
    {
        CHECK_THROWS([&](){Five a(3,5);
                           ;}()); // -- непятеричное число -- // //
        CHECK_THROWS(([&](){Five a = {1,2};
                            Five b = {1,3};
                            Five c = a-b;
                           }())); // -- a < b при вычитание -- //
        CHECK_THROWS(([&](){Five a = {1,2};
                            Five b = {0};
                            Five c = a/b;
                           }())); // -- деление на ноль -- //
    }
}

TEST_CASE("Тесты класса BitString в doctest")
{
    BitString t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        BitString two;
        CHECK(t == two);
        CHECK(two.get_size() == 0);
        CHECK(two.get_data() == nullptr);
        BitString BS1("00100110");      CHECK(BS1.toString() == "0 0 1 0 0 1 1 0");
        BitString BS2("110101"); CHECK(BS2.toString() == "1 1 0 1 0 1");
        CHECK(BS2.get_size() == 6);
        BitString d(3,1);      CHECK(d.toString() == "1 1 1");
        BitString dd{1,0,0,1,1,0};    CHECK(dd.toString() == "1 0 0 1 1 0");
        BitString zz(dd);    CHECK(zz.toString() == "1 0 0 1 1 0");
        CHECK(dd.get_size() == 6);
        BitString dstr("100000"); CHECK(dstr.toString() == "1 0 0 0 0 0");
        two = dstr;           CHECK(two.toString() == "1 0 0 0 0 0");
        two = {0};      CHECK(two.toString() == "0");
        two = {};      CHECK(two.get_size() == 0);
    }
    SUBCASE("Тест операции and (&)")
    {
        BitString BS1("100110");
        BitString BS2("110101");
        BS1 &= BS2;
        CHECK(BS1.toString() == "1 0 0 1 0 0");
        BS1 = {("110")};
        BS2 = {("1010110111")};
        BS1 &= BS2;
        CHECK(BS1.toString() == "1 1 0");
        BS1 = {("0000100110")};
        BS2 = {("110101")};
        BS1 &= BS2;
        CHECK(BS1.toString() == "1 0 0 1 0 0");
    }
    SUBCASE("Тест операции or (|)")
    {
        BitString BS1("100110");
        BitString BS2("110101");
        BS1 |= BS2;
        CHECK(BS1.toString() == "1 1 0 1 1 1");
        BS1 = {("110")};
        BS2 = {("1010110111")};
        BS1 |= BS2;
        CHECK(BS1.toString() == "1 0 1 0 1 1 0 1 1 1");
        BS1 = {("0000100110")};
        BS2 = {("110101")};
        BS1 |= BS2;
        CHECK(BS1.toString() == "0 0 0 0 1 1 0 1 1 1");
        BS1 = {("001000100000")};
        BS2 = {("1110")};
        BS1 |= BS2;
        CHECK(BS1.toString() == "0 0 1 0 0 0 1 0 1 1 1 0");
    }
    SUBCASE("Тест операции xor (^)")
    {
        BitString BS1("100110");
        BitString BS2("110101");
        BS1 ^= BS2;
        CHECK(BS1.toString() == "0 1 0 0 1 1");
        BS1 = {("110")};
        BS2 = {("1010110111")};
        BS1 ^= BS2;
        CHECK(BS1.toString() == "1 0 1 0 1 1 0 0 0 1");
        BS1 = {("0000100110")};
        BS2 = {("110101")};
        BS1 ^= BS2;
        CHECK(BS1.toString() == "0 0 0 0 0 1 0 0 1 1");
        BS1 = {("001000100000")};
        BS2 = {("1110")};
        BS1 ^= BS2;
        CHECK(BS1.toString() == "0 0 1 0 0 0 1 0 1 1 1 0");
    }
    SUBCASE("Тест операций сдвига")
    {
        BitString BS1("100110");
        BitString BS2("110101");
        BS1 >>= 3;
        CHECK(BS1.toString() == "0 0 0 1 0 0 1 1 0");
        BS2 <<= 4;
        CHECK(BS2.toString() == "1 1 0 1 0 1 0 0 0 0");
        BS1 = {("110")};
        BS2 = {("1010110111")};
        BS1 >>= 2;
        CHECK(BS1.toString() == "0 0 1 1 0");
        BS2 <<= 1;
        CHECK(BS2.toString() == "1 0 1 0 1 1 0 1 1 1 0");
    }
    SUBCASE("Тест операции not (~)")
    {
        BitString BS1("111111");
        BitString BS2("0000");
        BitString BS3("100110");
        BitString BS4 = {("001000100000")};
        CHECK((~BS1).toString() == "0 0 0 0 0 0");
        CHECK((~BS2).toString() == "1 1 1 1");
        CHECK((~BS3).toString() == "0 1 1 0 0 1");
        CHECK((~BS4).toString() == "1 1 0 1 1 1 0 1 1 1 1 1");
    }
    SUBCASE("Тест сравнения")          // -- проверка сравнения --
    {
        BitString a("100110");
        BitString b("110110");
        BitString c("100110");
        CHECK(a.count_of_SingleBit() == 3);
        CHECK(b.count_of_SingleBit() == 4);
        CHECK(a == c);
        CHECK(a <= c);
        CHECK(b > c);
        CHECK(a < b);
        CHECK(a != b);
        CHECK(b != c);
    }
    SUBCASE("Тест исключений")          // -- проверка сравнения --
    {
        CHECK_THROWS([&](){BitString a(3,2);
                           ;}()); // -- недвоичное число -- // //
    }
}
