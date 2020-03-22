#include <vector>
#include <iostream>
#include "doctest.h"
#include "queue.h"


// -- непосредственно тесты -
TEST_CASE("Тесты класса Queue в doctest")
{
    Queue t;         // -- нулевой объект – нужен всем --
    std::vector<int> a;
//    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
//    {
//        Array two;
//        CHECK(t == two);
//        CHECK(two.get_size() == 0);
//        CHECK(two.get_data() == nullptr);
//        Array dz(2);      CHECK(dz.toString() == "0 0");
//        CHECK(dz.get_size() == 2);
//        Array d(3,7);      CHECK(d.toString() == "7 7 7");
//        Array dd{4,2,14};    CHECK(dd.toString() == "4 2 14");
//        CHECK(dd.get_size() == 3);
//        Array dstr("124523"); CHECK(dstr.toString() == "1 2 4 5 2 3");
//        two = dd;           CHECK(two.toString() == "4 2 14");
//        two = {3, 7};      CHECK(two.toString() == "3 7");
//        std::cout << two;
//        two = {};      CHECK(two.get_size() == 0);
//        Array zz(d);    CHECK(zz.toString() == "7 7 7");
//        zz.push_back(5); CHECK(zz.toString() == "7 7 7 5");
//    }

//    SUBCASE("Тест индексации и удаления")              // -- тестирование сложения --
//    {
//        Array dstr("124523");
//        CHECK(dstr[0] == 1);
//        CHECK(dstr[1] == 2);
//        CHECK(dstr[4] == 2);
//        dstr.erase(dstr.begin(), dstr.end()-1);
//        CHECK(dstr.get_size() == 1);
//        CHECK(dstr[0] == 3);
//    }
//    SUBCASE("Тест сравнения")          // -- проверка сравнения --
//    {
//        Array a = {8, 5};
//        Array b = {4, 9};
//        Array c = {8, 5};
//        Array d = {1, 2, 3};

//        CHECK(a == c);
//        CHECK(a <= c);
//        CHECK(b < c);
//        CHECK(a > b);
//        CHECK(d > a);
//        CHECK(d >= b);
//        CHECK(a != b);
//        CHECK(b != c);
//    }
//    SUBCASE("Тест исключений")          // -- проверка сравнения --
//    {
//        CHECK_THROWS([&](){Array a(3,5);
//                           a[3];}()); // -- индекс за диапазоном -- // //

//        CHECK_THROWS([&](){Array g(0,3);
//                           }()); // -- нулевой размер -- //
//    }
//}

//TEST_CASE("Тесты класса Four в doctest")
//{
//    Four t;         // -- нулевой объект – нужен всем --
//    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
//    {
//        Four two;
//        CHECK(t == two);
//        CHECK(two.get_size() == 0);
//        CHECK(two.get_data() == nullptr);
//        Four dz(2);      CHECK(dz.toString() == "0 0");
//        CHECK(dz.get_size() == 2);
//        Four d(3,3);      CHECK(d.toString() == "3 3 3");
//        Four dd{1,2,0,3,1,0};    CHECK(dd.toString() == "1 2 0 3 1 0");
//        Four zz(dd);    CHECK(zz.toString() == "1 2 0 3 1 0");
//        CHECK(dd.get_size() == 6);
//        Four dstr("120123"); CHECK(dstr.toString() == "1 2 0 1 2 3");
//        two = dd;           CHECK(two.toString() == "1 2 0 3 1 0");
//        two = {3,2};      CHECK(two.toString() == "3 2");
//        two = {};      CHECK(two.get_size() == 0);
//    }

//    SUBCASE("Тест сложения")              // -- тестирование сложения --
//    {
//        Four a = {1,2};
//        Four b = {2};
//        Four c = a+b;
//        CHECK(c.toString() == "2 0");
//        c = b + a;
//        CHECK(c.toString() == "2 0");

//        a = {"1213"};
//        b = {"112"};
//        c = a+b;
//        CHECK(c.toString() == "1 3 3 1");
//        c = b + a;
//        CHECK(c.toString() == "1 3 3 1");

//        a = {3,3};
//        b = {3,3};
//        c = a+b;
//        CHECK(c.toString() == "1 3 2");
//        c = b + a;
//        CHECK(c.toString() == "1 3 2");

//        a = {0};
//        b = {1,2,1,3,1};
//        c = a+b;
//        CHECK(c.toString() == "1 2 1 3 1");
//        c = b + a;
//        CHECK(c.toString() == "1 2 1 3 1");

//        a = {"112131"};
//        b = {"1133213"};
//        c = a+b;
//        CHECK(c.toString() == "1 3 1 2 0 1 0");
//        c = b + a;
//        CHECK(c.toString() == "1 3 1 2 0 1 0");
//    }
//    SUBCASE("Тест вычитания")              // -- тестирование вычитания --
//    {
//        Four a = {1,2};
//        Four b = {2};
//        Four c = a-b;
//        CHECK(c.toString() == "1 0");

//        a = {"1213"};
//        b = {"112"};
//        c = a - b;
//        CHECK(c.toString() == "1 1 0 1");

//        a = {3,3};
//        b = {3,3};
//        c = a-b;
//        CHECK(c.toString() == "0");
//        c = b - a;
//        CHECK(c.toString() == "0");

//        a = {0};
//        b = {1,2,1,3,1};
//        c = b - a;
//        CHECK(c.toString() == "1 2 1 3 1");
//    }
//    SUBCASE("Тест умножения")              // -- тестирование умножения -- //
//    {
//        Four a = {1,2,3,2};
//        Four b = {3,2};
//        a *= b;
//        CHECK(a.toString() == "1 2 0 0 1 0");
//        a = {"12321312"};
//        b = {"31312"};
//        b *= a;
//        CHECK(b.toString() == "1 1 3 3 2 1 0 3 0 1 2 1 0");
//        a = {"31013101201212030121021321203121312013"};
//        b = {"310131012012120301210213212031213120"};
//        a *= b;
//        Four res("22233313302032222003123121323231030131232010100211110232032303023011203220");
//        CHECK(res == a);
//    }
//    SUBCASE("Тест деления")              // -- тестирование умножения -- //
//    {
//        Four a = {"123213312"};
//        Four b = {"12"};
//        a /= b;
//        Four res = {("10212221")};
//        CHECK(res == a);

//        a = {"12321331212"};
//        b = {"1213"};
//        a /= b;
//        res = {("10102213")};
//        CHECK(res == a);

//        a = {"1213"};
//        b = {"1213"};
//        a /= b;
//        res = {1};
//        CHECK(res == a);

//        a = {"3"};
//        b = {"1213"};
//        a /= b;
//        res = {0};
//        CHECK(res == a);

//        a = {"0"};
//        b = {"1213"};
//        a /= b;
//        res = {0};
//        CHECK(res == a);

//        a = {"1213"};
//        b = {"1"};
//        a /= b;
//        res = {1,2,1,3};
//        CHECK(res == a);
//    }
//    SUBCASE("Тест сравнения")          // -- проверка сравнения --
//    {
//        Four a = {1, 3};
//        Four b = {3, 2};
//        Four c = {1, 3};
//        Four d = {1, 2, 3};

//        CHECK(a == c);
//        CHECK(a <= c);
//        CHECK(b > c);
//        CHECK(a < b);
//        CHECK(d > a);
//        CHECK(d >= b);
//        CHECK(a != b);
//        CHECK(b != c);
//    }
//    SUBCASE("Тест исключений")          // -- проверка сравнения --
//    {
//        CHECK_THROWS([&](){Four a(3,5);
//                           ;}()); // -- нечетверичное число -- // //

//        CHECK_THROWS(([&](){Four a = {1,2};
//                            Four b = {1,3};
//                            Four c = a-b;
//                           }())); // -- a < b при вычитание -- //
//        CHECK_THROWS(([&](){Four a = {1,2};
//                            Four b = {0};
//                            Four c = a/b;
//                           }())); // -- деление на ноль -- //
//    }
}
