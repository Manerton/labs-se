#include <vector>
#include <iostream>
#include "doctest.h"
#include "queue.h"

using std::cout;
using std::endl;

// -- непосредственно тесты -
TEST_CASE("Тесты класса Queue в doctest")
{
    Queue t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        Queue a("Ivan");
        CHECK(a.getName() == "Ivan");
        Queue b("Vanya", "17.11.18");
        CHECK(b.getName() == "Vanya");
        CHECK(b.getDate() == "17.11.18");
        Queue::Lodger b_lodger(12345678,3,2014,4,60.3f,3,87.7f);
        b.add(b_lodger);
        CHECK(b.getCount_of_people() == 1);
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
            Queue::Lodger lodger(123456789,3,2014,4,60.3f,3,87.7f);
            a.add(lodger);
        }());
    }
}

