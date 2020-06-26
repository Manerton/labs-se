#include "doctest.h"
#include "unemployedelem.h"
#include "unemployed.h"
#include "array.h"
#include <iostream>
#include <vector>

// -- непосредственно тесты -
TEST_CASE("Тесты класс Array в doctest")
{
    SUBCASE("Тест создания и присваивания") // -- создание и присваивание --
    {
        Array<UnemployedElem> w;
        CHECK(w.size() == 0);
        CHECK(w.empty());
        UnemployedElem a("1314",44,1,{4,4},{7,3},45500);
        UnemployedElem b("5231",33,1,{5,0},{11,4},154355);
        UnemployedElem c{"33",25,0,{1,2},{3,7},18543};
        UnemployedElem d{"18",15,0,{0,3},{1,2},32322};
        Array<UnemployedElem> e{a,b};
        e.back();
        CHECK(e.back().toString() == "5231 33 1 5;0 11;4 154355");
        e.push_back(c);
        CHECK(e[2].toString() == "33 25 0 1;2 3;7 18543");
        Array<UnemployedElem> r(e);
        CHECK((r == e));
        w = Array(1,b);
        CHECK((w[0].toString() == "5231 33 1 5;0 11;4 154355"));
        Array<UnemployedElem> y(5,a);
        CHECK(y.front().toString() == "1314 44 1 4;4 7;3 45500");
        Array<UnemployedElem> u{a, b, c};
        CHECK(u.size() == 3);
        auto it = ++u.begin();  // it указывает на 2 элемент (b)
        it = u.insert(it,d);
        CHECK(((*it) == b));
        auto it2 = u.insert(2,a);
        CHECK(((*it2) == b));
    }

    SUBCASE("Тест создания и присваивания для типа int") // -- создание и присваивание --
    {
        Array<int> w;
        CHECK(w.size() == 0);
        Array<int> e{1, 5, 9};
        e.back();
        CHECK(e.back() == 9);
        e.push_back(7);
        CHECK(e[1] == 5);
        Array<int> r(e);
        CHECK((r == e));
        w = Array(1,3);
        CHECK((w[0] == 3));
        Array<int> y(5,6);
        CHECK(y.front() == 6);
        Array<int> u{5, 1, 7};
        CHECK(u.size() == 3);
        auto it = ++u.begin();  // it указывает на 2 элемент (1)
        it = u.insert(it,9);
        CHECK(((*it) == 1));
        auto it2 = u.insert(2,3);
        CHECK(((*it2) == 1));
    }

    SUBCASE("Тест удаления")              // -- тестирование сложения --
    {
        UnemployedElem a("1314",44,1,{4,4},{7,3},45500);
        UnemployedElem b("5231",33,1,{5,0},{11,4},154355);
        UnemployedElem c{"33",25,0,{1,2},{3,7},18543};
        UnemployedElem d{"18",15,0,{0,3},{1,2},32322};
        UnemployedElem e{"17",75,0,{11,6},{25,7},18543};
        Array<UnemployedElem> u{a, b, c, d, e};
        auto it = ++u.begin();  // it указывает на b
        it = u.erase(it);
        CHECK(u.size() == 4);
        CHECK((*it == c));
        CHECK(u[0].toString() == "1314 44 1 4;4 7;3 45500");
        CHECK(u[1].toString() == "33 25 0 1;2 3;7 18543");
        CHECK(u[2].toString() == "18 15 0 0;3 1;2 32322");
        CHECK(u[3].toString() == "17 75 0 11;6 25;7 18543");

        u.insert(1,b);
        CHECK(u[1].toString() == "5231 33 1 5;0 11;4 154355");

        it = u.erase(1);
        CHECK(u.size() == 4);
        CHECK((*it == c));
        CHECK(u[0].toString() == "1314 44 1 4;4 7;3 45500");
        CHECK(u[1].toString() == "33 25 0 1;2 3;7 18543");
        CHECK(u[2].toString() == "18 15 0 0;3 1;2 32322");
        CHECK(u[3].toString() == "17 75 0 11;6 25;7 18543");

        u.pop_back();
        CHECK(u.size() == 3);
        CHECK(u[0].toString() == "1314 44 1 4;4 7;3 45500");
        CHECK(u[1].toString() == "33 25 0 1;2 3;7 18543");
        CHECK(u[2].toString() == "18 15 0 0;3 1;2 32322");

        u.pop_front();
        CHECK(u.size() == 2);
        CHECK(u[0].toString() == "33 25 0 1;2 3;7 18543");
        CHECK(u[1].toString() == "18 15 0 0;3 1;2 32322");

        u.push_back(e);
        CHECK(u.size() == 3);
        CHECK(u[0].toString() == "33 25 0 1;2 3;7 18543");
        CHECK(u[1].toString() == "18 15 0 0;3 1;2 32322");
        CHECK(u[2].toString() == "17 75 0 11;6 25;7 18543");

        u.push_front(a);
        CHECK(u.size() == 4);
        CHECK(u[0].toString() == "1314 44 1 4;4 7;3 45500");
        CHECK(u[1].toString() == "33 25 0 1;2 3;7 18543");
        CHECK(u[2].toString() == "18 15 0 0;3 1;2 32322");
        CHECK(u[3].toString() == "17 75 0 11;6 25;7 18543");
    }
}

TEST_CASE("Тест класса Unemployed")
{
    SUBCASE("Тест поиска и удаления по ID")
    {
         UnemployedElem a("1314",44,1,{4,4},{7,3},45500);
         UnemployedElem b("5231",33,1,{5,0},{11,4},154355);
         UnemployedElem c{"33",25,0,{1,2},{3,7},18543};
         Unemployed list("Ivanov", "25062020",{a,b,c});
         CHECK(list.size() == 3);
         auto it1 = ++list.begin();
         CHECK((*it1).toString() == "5231 33 1 5;0 11;4 154355");
         auto it2 = list.find_by_ID("5231");
         CHECK((*it2).toString() == "5231 33 1 5;0 11;4 154355");
         CHECK(it1 == it2);
         auto elem = list.find_elem_by_ID("5231");
         CHECK(elem.toString() == b.toString());
         list.delete_by_ID("5231");
         CHECK(list.size() == 2);
         CHECK((*list.begin()).toString() == "1314 44 1 4;4 7;3 45500");
         CHECK((*(++list.begin())).toString() == "33 25 0 1;2 3;7 18543");
         UnemployedElem d("17",19,0,{0,5},{2,4},35444);
         list.change_by_ID("1314",d);
         CHECK(list.size() == 2);
         CHECK((*list.begin()).toString() == "17 19 0 0;5 2;4 35444");
    }
    SUBCASE("Тест поиска по стажу (больше или меньше заданной величины)")
    {
        UnemployedElem d{"546",18,1,{4,2},{4,4},18543};
        UnemployedElem a("1314",44,1,{4,4},{7,3},45500);
        UnemployedElem c{"33",21,0,{5,2},{7,7},18543};
        UnemployedElem b("5231",33,1,{5,0},{11,4},154355);
        UnemployedElem e{"17",75,0,{11,6},{25,7},18543};
        Unemployed list("Ivanov", "25062020",{d,a,c,b,e});
        auto it = list.find_greater_than_seniority(UnemployedElem::Seniority{7,7});
        CHECK(it == list.find_by_ID("5231"));
        UnemployedElem g{"555",22,1,{3,5},{8,3},18543};
        list.push_front(g);
        auto it2 = list.find_less_than_seniority(UnemployedElem::Seniority{5,0});
        CHECK(it2 == list.find_by_ID("546"));
        CHECK(it2 == ++list.begin());
        it = list.begin();
        ++it; ++it; ++it;   // сместились на 3 от начала, т.е на элемент с ID 33
        CHECK(it == list.find_by_ID("33"));
    }
    SUBCASE("Тест итератора")
    {
        UnemployedElem a("1314",44,1,{4,4},{7,3},45500);
        UnemployedElem b("5231",33,1,{5,0},{11,4},154355);
        UnemployedElem c{"33",21,0,{5,2},{7,7},18543};
        Unemployed list("Ivanov", "25062020",{a,b,c});
        auto it = list.begin();
        ++it;
        CHECK((*it).toString() == "5231 33 1 5;0 11;4 154355");
        *it = a;
        CHECK((*it).toString() == "1314 44 1 4;4 7;3 45500");
        auto it2 = list.find_by_ID("33");
        CHECK(it2.ptr != nullptr);
        CHECK((*it2).toString() == "33 21 0 5;2 7;7 18543");
        *it2 = b;
        CHECK((*it2).toString() == "5231 33 1 5;0 11;4 154355");
    }
    SUBCASE("Тест константного итератора")
    {
        UnemployedElem a("1314",44,1,{4,4},{7,3},45500);
        UnemployedElem b("5231",33,1,{5,0},{11,4},154355);
        UnemployedElem c{"33",21,0,{5,2},{7,7},18543};
        const Unemployed list("Ivanov", "25062020",{a,b,c});
        auto it = list.begin();
        ++it;
        CHECK((*it).toString() == "5231 33 1 5;0 11;4 154355");
        auto it2 = list.find_by_ID("33");
        CHECK((*it2).toString() == "33 21 0 5;2 7;7 18543");
    }
}
