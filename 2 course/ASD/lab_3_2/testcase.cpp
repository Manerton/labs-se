#include <vector>
#include <iostream>
#include "doctest.h"
#include "queue.h"

using std::cout;
using std::endl;
using Lodger = Queue::Lodger;

// -- непосредственно тесты -
TEST_CASE("Тесты класса Queue в doctest")
{
    Queue t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        Queue a("Ivan");
        CHECK(a.get_Name() == "Ivan");
        Queue b("Vanya", "17.11.18");
        CHECK(b.get_Name() == "Vanya");
        CHECK(b.get_Date() == "17.11.18");
        Lodger b_lodger(12345678,3,2014,4,60.3f,3,87.7f);
        b.add(b_lodger);
        CHECK(b.get_Count_of_people() == 1);
    }
    SUBCASE("Тест поиска по ID")
    {
        Queue a("Vanya", "17.11.18");
        Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
        Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
        Lodger lodger_3(12452   ,5,2017,3,54.3f,4,60.5f);
        Lodger lodger_4(7       ,3,2019,4,60.3f,3,87.7f);
        Lodger lodger_5(141444  ,1,2010,1,44.8f,3,88.2f);
        a.add(lodger_1);
        a.add(lodger_2);
        a.add(lodger_3);
        a.add(lodger_4);
        a.add(lodger_5);
        CHECK(a.get_Count_of_people() == 5);
        auto it = a.find_by_ID(7);
        CHECK(a.lodger_toString(*it) == "7 3 2019 4 60.3 3 87.7");
        it = a.find_by_ID(12452);
        CHECK(*it == lodger_3);
    }
    /// по площади, сделать замену/удаление
    SUBCASE("Тест поиска по дате (до заданной)")
    {
        Queue a("Vanya", "17.11.18");
        Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
        Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
        Lodger lodger_3(12452   ,5,2017,3,54.3f,4,60.5f);
        Lodger lodger_4(7       ,3,2019,4,60.3f,3,87.7f);
        Lodger lodger_5(141444  ,1,2010,1,44.8f,3,88.2f);
        Lodger lodger_6(148     ,1,2014,5,33.9f,2,56.7f);
        a.add(lodger_1);
        a.add(lodger_2);
        a.add(lodger_3);
        a.add(lodger_4);
        a.add(lodger_5);
        a.add(lodger_6);
        CHECK(a.get_Count_of_people() == 6);
        auto array = a.find_before_date(2017,3);
        CHECK(array.size() == 4);
        CHECK(array[0] == lodger_5);
        CHECK(array[1] == lodger_2);
        CHECK(array[2] == lodger_1);
        CHECK(array[3] == lodger_6);
    }
    SUBCASE("Тест поиска по дате (после заданной)")
    {
        Queue a("Vanya", "17.11.18");
        Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
        Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
        Lodger lodger_3(12452   ,5,2017,3,54.3f,4,60.5f);
        Lodger lodger_4(7       ,3,2019,4,60.3f,3,87.7f);
        Lodger lodger_5(141444  ,1,2010,1,44.8f,3,88.2f);
        Lodger lodger_6(148     ,1,2014,5,33.9f,2,56.7f);
        a.add(lodger_1);
        a.add(lodger_2);
        a.add(lodger_3);
        a.add(lodger_4);
        a.add(lodger_5);
        a.add(lodger_6);
        CHECK(a.get_Count_of_people() == 6);
        auto array = a.find_after_date(2017,3);
        CHECK(array.size() == 1);
        CHECK(array[0] == lodger_4);
    }

    SUBCASE("Тест исключений")
    {
        CHECK_THROWS([&](){
            Queue c("wrong day", "35.11.19");
        }());

        CHECK_THROWS([&](){
            Queue a("wrong month", "17.13.19");
        }());

        CHECK_THROWS([&](){
            Queue a("wrong dots", "05,11,14");
        }());

        CHECK_THROWS([&](){
            Queue a("wrong digits", "11.1z.19");
        }());

        CHECK_THROWS([&](){
            Queue a("wrong dots", "12011.19");
        }());

        CHECK_THROWS([&](){
            Queue a("id out of bounds", "12.11.19");
            Lodger lodger(123456789,3,2014,4,60.3f,3,87.7f);
            a.add(lodger);
        }());

        CHECK_THROWS([&](){
            Queue a("element not found", "12.11.19");
            Lodger lodger(12345678,3,2014,4,60.3f,3,87.7f);
            a.add(lodger);
            [[maybe_unused]] auto it = a.find_by_ID(12345670);
        }());
    }
}

