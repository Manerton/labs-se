#include "doctest.h"
#include "array.h"
#include "seven.h"
#include "binary.h"
#include "thirteen.h"
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

        Array indextest("12345");
        CHECK(indextest[1] == 2);
        Array::value_type value = indextest[1];
        CHECK(value == 2);
        indextest[1] = 5;
        CHECK(indextest[1] == 5);

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

TEST_CASE("Тесты класса Seven в doctest")
{
    Seven t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        Seven two;
        CHECK(t == two);
        CHECK(two.get_size() == 0);
        CHECK(two.get_data() == nullptr);
        Seven dz(2);      CHECK(dz.toString() == "0 0");
        CHECK(dz.get_size() == 2);
        Seven d(3,5);      CHECK(d.toString() == "5 5 5");
        Seven dd{1,2,5,3,4,0};    CHECK(dd.toString() == "1 2 5 3 4 0");
        Seven zz(dd);    CHECK(zz.toString() == "1 2 5 3 4 0");
        CHECK(dd.get_size() == 6);
        Seven dstr("125143"); CHECK(dstr.toString() == "1 2 5 1 4 3");
        two = dd;           CHECK(two.toString() == "1 2 5 3 4 0");
        two = {3,5};      CHECK(two.toString() == "3 5");
        two = {};      CHECK(two.get_size() == 0);
    }

    SUBCASE("Тест сложения")              // -- тестирование сложения --
    {
        Seven a = {1,2};
        Seven b = {2};
        Seven c = a+b;
        CHECK(c.toString() == "1 4");
        c = b + a;
        CHECK(c.toString() == "1 4");

        a = {"1213"};
        b = {"112"};
        c = a+b;
        CHECK(c.toString() == "1 3 2 5");
        c = b + a;
        CHECK(c.toString() == "1 3 2 5");

        a = {4,5};
        b = {3,2};
        c = a+b;
        CHECK(c.toString() == "1 1 0");
        c = b + a;
        CHECK(c.toString() == "1 1 0");

        a = {0};
        b = {1,2,1,5,1};
        c = a+b;
        CHECK(c.toString() == "1 2 1 5 1");
        c = b + a;
        CHECK(c.toString() == "1 2 1 5 1");

        a = {"112131"};
        b = {"1133213"};
        c = a+b;
        CHECK(c.toString() == "1 2 4 5 3 4 4");
        c = b + a;
        CHECK(c.toString() == "1 2 4 5 3 4 4");
    }
    SUBCASE("Тест вычитания")              // -- тестирование вычитания --
    {
        Seven a = {1,5};
        Seven b = {2};
        Seven c = a-b;
        CHECK(c.toString() == "1 3");

        a = {"1216"};
        b = {"112"};
        c = a - b;
        CHECK(c.toString() == "1 1 0 4");

        a = {4,5};
        b = {4,5};
        c = a-b;
        CHECK(c.toString() == "0");
        c = b - a;
        CHECK(c.toString() == "0");

        a = {0};
        b = {1,2,1,5,1};
        c = b - a;
        CHECK(c.toString() == "1 2 1 5 1");
    }
    SUBCASE("Тест умножения")              // -- тестирование умножения -- //
    {
        Seven a = {1,2,3,2};
        Seven b = {3,2};
        a *= b;
        CHECK(a.toString() == "4 3 0 5 4");
        a = {"12321312"};
        b = {"31312"};
        b *= a;
        CHECK(b.toString() == "4 2 2 5 0 4 6 1 4 6 4 4");
        a = {"31014101201212050121021421203121612013"};
        b = {"310131012012120301210213212031213120"};
        a *= b;
        Seven res("12622365315611221316160212003040246305316531410103600210414340366654543560");
        CHECK(res == a);
    }
    SUBCASE("Тест деления")              // -- тестирование умножения -- //
    {
        Seven a = {"123213312"};
        Seven b = {"12"};
        a /= b;
        Seven res = {("10240245")};
        CHECK(res == a);

        a = {"12321331212"};
        b = {"1213"};
        a /= b;
        res = {("10130226")};
        CHECK(res == a);

        a = {"1213"};
        b = {"1213"};
        a /= b;
        res = {1};
        CHECK(res == a);

        a = {"3"};
        b = {"1215"};
        a /= b;
        res = {0};
        CHECK(res == a);

        a = {"0"};
        b = {"1215"};
        a /= b;
        res = {0};
        CHECK(res == a);

        a = {"1513"};
        b = {"1"};
        a /= b;
        res = {1,5,1,3};
        CHECK(res == a);
    }
    SUBCASE("Тест сравнения")          // -- проверка сравнения --
    {
        Seven a = {1, 5};
        Seven b = {3, 2};
        Seven c = {1, 5};
        Seven d = {1, 2, 3};

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
        CHECK_THROWS([&](){Seven a(3,7);
                           ;}()); // -- нечетверичное число -- // //

        CHECK_THROWS(([&](){Seven a = {1,2};
            Seven b = {1,3};
            Seven c = a-b;
        }())); // -- a < b при вычитание -- //
        CHECK_THROWS(([&](){Seven a = {1,2};
            Seven b = {0};
            Seven c = a/b;
        }())); // -- деление на ноль -- //
    }
}

TEST_CASE("Тесты класса Thirteen в doctest")
{
    Thirteen t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        Thirteen two;
        CHECK(t == two);
        CHECK(two.get_size() == 0);
        CHECK(two.get_data() == nullptr);
        Thirteen dz(2);      CHECK(dz.toString() == "0 0");
        CHECK(dz.get_size() == 2);
        Thirteen d(3,5);      CHECK(d.toString() == "5 5 5");
        Thirteen dd{1,'A',5,'C',4,0};    CHECK(dd.toString() == "1 A 5 C 4 0");
        Thirteen zz(dd);    CHECK(zz.toString() == "1 A 5 C 4 0");
        CHECK(dd.get_size() == 6);
        Thirteen dstr("125C43"); CHECK(dstr.toString() == "1 2 5 C 4 3");
        two = dd;           CHECK(two.toString() == "1 A 5 C 4 0");
        two = {3,'B'};      CHECK(two.toString() == "3 B");
        two = {};      CHECK(two.get_size() == 0);
    }

    SUBCASE("Тест сложения")              // -- тестирование сложения --
    {
        Thirteen a = {1,2};
        Thirteen b = {2};
        Thirteen c = a+b;
        CHECK(c.toString() == "1 4");
        c = b + a;
        CHECK(c.toString() == "1 4");

        a = {"121A"};
        b = {"112"};
        c = a+b;
        CHECK(c.toString() == "1 3 2 C");
        c = b + a;
        CHECK(c.toString() == "1 3 2 C");

        a = {4,5};
        b = {3,2};
        c = a+b;
        CHECK(c.toString() == "7 7");
        c = b + a;
        CHECK(c.toString() == "7 7");

        a = {0};
        b = {1,2,1,'c',1};
        c = a+b;
        CHECK(c.toString() == "1 2 1 C 1");
        c = b + a;
        CHECK(c.toString() == "1 2 1 C 1");

        a = {"11A131"};
        b = {"1B13213"};
        c = a+b;
        CHECK(c.toString() == "1 C 3 0 3 4 4");
        c = b + a;
        CHECK(c.toString() == "1 C 3 0 3 4 4");
    }
    SUBCASE("Тест вычитания")              // -- тестирование вычитания --
    {
        Thirteen a = {'A',5};
        Thirteen b = {8};
        Thirteen c = a-b;
        CHECK(c.toString() == "9 A");

        a = {"1216"};
        b = {"112"};
        c = a - b;
        CHECK(c.toString() == "1 1 0 4");

        a = {4,5};
        b = {4,5};
        c = a-b;
        CHECK(c.toString() == "0");
        c = b - a;
        CHECK(c.toString() == "0");

        a = {0};
        b = {1,2,1,5,1};
        c = b - a;
        CHECK(c.toString() == "1 2 1 5 1");
    }
    SUBCASE("Тест умножения")              // -- тестирование умножения -- //
    {
        Thirteen a = {1,2,3,2};
        Thirteen b = {3,'b'};
        a *= b;
        CHECK(a.toString() == "4 6 8 1 9");
        a = {"12321312"};
        b = {"31312"};
        b *= a;
        CHECK(b.toString() == "3 8 2 4 6 B 5 8 B 0 4 4");
        a = {"31014A012012120501B102142120312161C013"};
        b = {"B1013101201212030121021321203121312A"};
        a *= b;
        Thirteen res("28125C40C413583A7A233124517226A630713C569C6534A0CB00474B799887C830656B0654");
        CHECK(res == a);
    }
    SUBCASE("Тест деления")              // -- тестирование умножения -- //
    {
        Thirteen a = {"1232133"};
        Thirteen b = {"12"};
        a /= b;
        Thirteen res = {("102980")};
        CHECK(res == a);

        a = {"12321331212"};
        b = {"121BCA"};
        a /= b;
        res = {("1010AA")};
        CHECK(res == a);

        a = {"1213"};
        b = {"1213"};
        a /= b;
        res = {1};
        CHECK(res == a);

        a = {"3"};
        b = {"1215"};
        a /= b;
        res = {0};
        CHECK(res == a);

        a = {"0"};
        b = {"1215"};
        a /= b;
        res = {0};
        CHECK(res == a);

        a = {"1513"};
        b = {"1"};
        a /= b;
        res = {1,5,1,3};
        CHECK(res == a);
    }
    SUBCASE("Тест сравнения")          // -- проверка сравнения --
    {
        Thirteen a = {'C', 5};
        Thirteen b = {'C', 'C'};
        Thirteen c = {'C', 5};
        Thirteen d = {1, 2, 3};

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
        CHECK_THROWS([&](){Thirteen a(3,'D');
                           ;}()); // -- нетринадцатиричное число -- // //

        CHECK_THROWS(([&](){Thirteen a = {1,2};
                            Thirteen b = {1,3};
                            Thirteen c = a-b;
                           }())); // -- a < b при вычитание -- //
        CHECK_THROWS(([&](){Thirteen a = {1,2};
                            Thirteen b = {0};
                            Thirteen c = a/b;
                           }())); // -- деление на ноль -- //
    }
}

TEST_CASE("Тесты класса Binary в doctest")
{
    Binary t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        Binary two;
        CHECK(t == two);
        CHECK(two.get_size() == 0);
        CHECK(two.get_data() == nullptr);
        Binary dz(2);      CHECK(dz.toString() == "0 0");
        CHECK(dz.get_size() == 2);
        Binary d(3,1);      CHECK(d.toString() == "1 1 1");
        Binary dd{1,0,1,0,0,1};    CHECK(dd.toString() == "1 0 1 0 0 1");
        Binary zz(dd);    CHECK(zz.toString() == "1 0 1 0 0 1");
        CHECK(dd.get_size() == 6);
        Binary dstr("111000"); CHECK(dstr.toString() == "1 1 1 0 0 0");
        two = dd;           CHECK(two.toString() == "1 0 1 0 0 1");
        two = {1,0};      CHECK(two.toString() == "1 0");
        two = {};      CHECK(two.get_size() == 0);
    }

    SUBCASE("Тест сложения")              // -- тестирование сложения --
    {
        Binary a = {1,0};
        Binary b = {1};
        Binary c = a+b;
        CHECK(c.toString() == "1 1");
        c = b + a;
        CHECK(c.toString() == "1 1");

        a = {"1010"};
        b = {"110"};
        c = a+b;
        CHECK(c.toString() == "1 0 0 0 0");
        c = b + a;
        CHECK(c.toString() == "1 0 0 0 0");

        a = {1,1};
        b = {1,1};
        c = a+b;
        CHECK(c.toString() == "1 1 0");
        c = b + a;
        CHECK(c.toString() == "1 1 0");

        a = {0};
        b = {1,1,1,0,1};
        c = a+b;
        CHECK(c.toString() == "1 1 1 0 1");
        c = b + a;
        CHECK(c.toString() == "1 1 1 0 1");

        a = {"110101"};
        b = {"1100010"};
        c = a+b;
        CHECK(c.toString() == "1 0 0 1 0 1 1 1");
        c = b + a;
        CHECK(c.toString() == "1 0 0 1 0 1 1 1");
    }
    SUBCASE("Тест вычитания")              // -- тестирование вычитания --
    {
        Binary a = {1,0};
        Binary b = {1};
        Binary c = a-b;
        CHECK(c.toString() == "1");

        a = {"1010"};
        b = {"110"};
        c = a - b;
        CHECK(c.toString() == "1 0 0");

        a = {1,1};
        b = {1,1};
        c = a-b;
        CHECK(c.toString() == "0");
        c = b - a;
        CHECK(c.toString() == "0");

        a = {0};
        b = {1,0,1,0,1};
        c = b - a;
        CHECK(c.toString() == "1 0 1 0 1");
    }
    SUBCASE("Тест умножения")              // -- тестирование умножения -- //
    {
        Binary a = {1,0,0,1};
        Binary b = {1,1};
        a *= b;
        CHECK(a.toString() == "1 1 0 1 1");
        a = {"10011011"};
        b = {"11010"};
        b *= a;
        CHECK(b.toString() == "1 1 1 1 1 0 1 1 1 1 1 0");
        a = {"111101111101111111101010001011101110"};
        b = {"100101011001010101110101010101111"};
        a *= b;
        Binary res("100100001101011000001010001111101010010010111000010101110110010110010");
        CHECK(res == a);
    }
    SUBCASE("Тест деления")              // -- тестирование умножения -- //
    {
        Binary a = {"100110011"};
        Binary b = {"11"};
        a /= b;
        Binary res = {("1100110")};
        CHECK(res == a);

        a = {"110010110"};
        b = {"11101"};
        a /= b;
        res = {("1110")};
        CHECK(res == a);

        a = {"1010"};
        b = {"1010"};
        a /= b;
        res = {1};
        CHECK(res == a);

        a = {"1"};
        b = {"1110"};
        a /= b;
        res = {0};
        CHECK(res == a);

        a = {"0"};
        b = {"1101"};
        a /= b;
        res = {0};
        CHECK(res == a);

        a = {"1101"};
        b = {"1"};
        a /= b;
        res = {1,1,0,1};
        CHECK(res == a);
    }
    SUBCASE("Тест сравнения")          // -- проверка сравнения --
    {
        Binary a = {1, 0};
        Binary b = {1, 1};
        Binary c = {1, 0};
        Binary d = {1, 1, 0};

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
        CHECK_THROWS([&](){Binary a(3,2);
                           ;}()); // -- нечетверичное число -- // //

        CHECK_THROWS(([&](){Binary a = {1,0};
            Binary b = {1,1};
            Binary c = a-b;
        }())); // -- a < b при вычитание -- //
        CHECK_THROWS(([&](){Binary a = {1,1};
            Binary b = {0};
            Binary c = a/b;
        }())); // -- деление на ноль -- //
    }
}
TEST_CASE("Тест мультиметодов")
{
    Thirteen thi;
    Seven sev;
    Binary bin;
    SUBCASE("dynamic cast")
    {
        CHECK(thi.MultMet_dynCast(thi) == "Thirteen-Thirteen");
        CHECK(thi.MultMet_dynCast(sev) == "Thirteen-Seven");
        CHECK(thi.MultMet_dynCast(bin) == "Thirteen-Binary");

        CHECK(sev.MultMet_dynCast(thi) == "Seven-Thirteen");
        CHECK(sev.MultMet_dynCast(sev) == "Seven-Seven");
        CHECK(sev.MultMet_dynCast(bin) == "Seven-Binary");

        CHECK(bin.MultMet_dynCast(thi) == "Binary-Thirteen");
        CHECK(bin.MultMet_dynCast(sev) == "Binary-Seven");
        CHECK(bin.MultMet_dynCast(bin) == "Binary-Binary");
    }
    SUBCASE("typeid")
    {
        CHECK(thi.MultMet_typeId(thi) == "Thirteen-Thirteen");
        CHECK(thi.MultMet_typeId(sev) == "Thirteen-Seven");
        CHECK(thi.MultMet_typeId(bin) == "Thirteen-Binary");

        CHECK(sev.MultMet_typeId(thi) == "Seven-Thirteen");
        CHECK(sev.MultMet_typeId(sev) == "Seven-Seven");
        CHECK(sev.MultMet_typeId(bin) == "Seven-Binary");

        CHECK(bin.MultMet_typeId(thi) == "Binary-Thirteen");
        CHECK(bin.MultMet_typeId(sev) == "Binary-Seven");
        CHECK(bin.MultMet_typeId(bin) == "Binary-Binary");
    }
    SUBCASE("virt func")
    {
        // без виртуальности
        CHECK(thi.MultMet_virtFunc(thi) == "Thirteen-Thirteen");
        CHECK(thi.MultMet_virtFunc(sev) == "Thirteen-Seven");
        CHECK(thi.MultMet_virtFunc(bin) == "Thirteen-Binary");

        CHECK(sev.MultMet_virtFunc(thi) == "Seven-Thirteen");
        CHECK(sev.MultMet_virtFunc(sev) == "Seven-Seven");
        CHECK(sev.MultMet_virtFunc(bin) == "Seven-Binary");

        CHECK(bin.MultMet_virtFunc(thi) == "Binary-Thirteen");
        CHECK(bin.MultMet_virtFunc(sev) == "Binary-Seven");
        CHECK(bin.MultMet_virtFunc(bin) == "Binary-Binary");

        Number &num = thi;

        CHECK(thi.MultMet_virtFunc(num) == "Thirteen-Thirteen");
        CHECK(sev.MultMet_virtFunc(num) == "Thirteen-Seven");
        CHECK(bin.MultMet_virtFunc(num) == "Thirteen-Binary");

        CHECK(num.MultMet_virtFunc(thi) == "Thirteen-Thirteen");
        CHECK(num.MultMet_virtFunc(sev) == "Thirteen-Seven");
        CHECK(num.MultMet_virtFunc(bin) == "Thirteen-Binary");
    }
}
