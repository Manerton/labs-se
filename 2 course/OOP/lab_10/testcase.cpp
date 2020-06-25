#include "doctest.h"
#include "unemployedelem.h"
#include "unemployed.h"
#include "list.h"
#include <iostream>
#include <vector>

// -- непосредственно тесты -
TEST_CASE("Тесты класс List в doctest")
{
    List q;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создания и присваивания") // -- создание и присваивание --
    {
        List w;
        CHECK(w.size() == 0);
        CHECK(w.empty());
        List e;
        UnemployedElem a("1314",44,1,{4,4},{7,3},45500);
        UnemployedElem b("5231",33,1,{5,0},{11,4},154355);
        UnemployedElem c{"33",25,0,{1,2},{3,7},18543};
        e.push_back(a);
        e.push_back(b);
        e.push_back(c);
        List r(e);
        CHECK((r == e));
        List t = e;
        CHECK((t == e));
        List y(5,a);
        CHECK(y.front().toString() == "1314 44 1 4;4 7;3 45500");
        CHECK((y.back() == y.front()));
        List u{a, b, c};
        CHECK(u.size() == 3);
        auto it = ++u.begin();  // it указывает на 2 элемент (b)
        u.insert(it,a);
        CHECK(*it == b);
    }

    SUBCASE("Тест удаления")              // -- тестирование сложения --
    {
        UnemployedElem a("1314",44,1,{4,4},{7,3},45500);
        UnemployedElem b("5231",33,1,{5,0},{11,4},154355);
        UnemployedElem c{"33",25,0,{1,2},{3,7},18543};
        List u{a, b, c};
        auto it = ++u.begin();  // it указывает на b
        it = u.erase(it);
        CHECK(u.size() == 2);
        CHECK((*it == c));
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
        CHECK(--it == list.find_by_ID("33"));
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
    SUBCASE("Тест обратного итератора")
    {
        UnemployedElem a("1314",44,1,{4,4},{7,3},45500);
        UnemployedElem b("5231",33,1,{5,0},{11,4},154355);
        UnemployedElem c{"33",21,0,{5,2},{7,7},18543};
        UnemployedElem d{"546",18,1,{4,2},{4,4},18543};
        UnemployedElem e{"17",75,0,{11,6},{25,7},18543};
        Unemployed list("Ivanov", "25062020",{a,b,c,d,e});
        auto it = list.rbegin();
        ++it;
        CHECK((*it).toString() == "546 18 1 4;2 4;4 18543");
        ++it;
        CHECK((*it).toString() == "33 21 0 5;2 7;7 18543");
        auto it2 = list.rend();
        --it2;
        CHECK((*it2).toString() == "5231 33 1 5;0 11;4 154355");
    }
    SUBCASE("Тест обратного константного итератора")
    {
        UnemployedElem a("1314",44,1,{4,4},{7,3},45500);
        UnemployedElem b("5231",33,1,{5,0},{11,4},154355);
        UnemployedElem c{"33",21,0,{5,2},{7,7},18543};
        UnemployedElem d{"546",18,1,{4,2},{4,4},18543};
        UnemployedElem e{"17",75,0,{11,6},{25,7},18543};
        const Unemployed list("Ivanov", "25062020",{a,b,c,d,e});
        auto it = list.rbegin();
        ++it;
        CHECK((*it).toString() == "546 18 1 4;2 4;4 18543");
        ++it;
        CHECK((*it).toString() == "33 21 0 5;2 7;7 18543");
        auto it2 = list.rend();
        --it2;
        CHECK((*it2).toString() == "5231 33 1 5;0 11;4 154355");
    }
}
