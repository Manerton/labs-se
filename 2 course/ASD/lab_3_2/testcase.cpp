#include <vector>
#include <iostream>
#include <chrono>
#include "doctest.h"
#include "queue.h"

using std::cout;
using std::endl;
using Lodger = Queue::Lodger;
using namespace std::chrono;
using sec = duration<double, std::ratio<1>>;

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

        auto array2 = a.find_before_date(2017,4);
        CHECK(array2.size() == 5);
        CHECK(array2[0] == lodger_5);
        CHECK(array2[1] == lodger_2);
        CHECK(array2[2] == lodger_1);
        CHECK(array2[3] == lodger_6);
        CHECK(array2[4] == lodger_3);
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

        auto array2 = a.find_after_date(2017,2);
        CHECK(array2.size() == 2);
        CHECK(array2[0] == lodger_3);
        CHECK(array2[1] == lodger_4);
    }

    SUBCASE("Тест поиска по требуемой площади (не меньше заданной)")
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
        auto array = a.find_by_area(70);
        CHECK(array.size() == 3);
        CHECK(array[0] == lodger_1);
        CHECK(array[1] == lodger_4);
        CHECK(array[2] == lodger_5);

        // так как не меньше заданной, а lodger_1 равен (т.е не меньше)
        auto array2 = a.find_by_area(71.3);
        CHECK(array2.size() == 3);
        CHECK(array2[0] == lodger_1);
        CHECK(array2[1] == lodger_4);
        CHECK(array2[2] == lodger_5);
    }

    SUBCASE("Тест удаления")
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
        auto it_of_deleted_elem = a.find_by_ID(17);
        CHECK(a.get_ID(*it_of_deleted_elem) == 17);
        a.erase(it_of_deleted_elem);
        CHECK(a.get_Count_of_people() == 5);

        // попробуем найти удаленный элемент
        auto try_to_find = [&](uint32_t ID)
        {
            [[maybe_unused]] auto it = a.find_by_ID(ID);
        };
        // выбрасывается исключение
        CHECK_THROWS_AS(try_to_find(17),Queue::element_not_found);

        // теперь удалим все элементы с площадью больше 80
        // это элементы с ID = 7 и 141444
        it_of_deleted_elem = a.find_area_iterator(80);
        CHECK(a.get_Area_required(*it_of_deleted_elem) >= 80);
        a.erase(it_of_deleted_elem,a.end());
        CHECK(a.get_Count_of_people() == 3);

        // попробуем найти эти удаленные элементы
        CHECK_THROWS_AS(try_to_find(7),Queue::element_not_found);
        CHECK_THROWS_AS(try_to_find(141444),Queue::element_not_found);
        // выбрасываются исключения
    }

    SUBCASE("Тест замены")
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
        auto it = a.find_by_ID(17);
        CHECK(a.get_ID(*it) == 17);
        CHECK(a.get_Year(*it) == 2011);

        Lodger lodger_7(17,0,2077,0,0,0,0);
        a.replace(it,lodger_7);
        CHECK(a.get_Count_of_people() == 6);
        it = a.find_by_ID(17);
        CHECK(a.get_Year(*it) == 2077);
    }

//    SUBCASE("Тест производительности")
//    {
//        auto somefunc = [](std::vector<int> &v)
//        {
//            for (int i = 0; i < 100000000; ++i)
//            {
//                v.at(i);
//            }
//        };
//        std::vector<int> vec(100000000,8);
//        const auto start = std::chrono::steady_clock::now();
//        somefunc(vec);
//        const auto end = std::chrono::steady_clock::now();
//        const auto res = duration_cast<sec>(end - start);
//        MESSAGE(res.count());
//    }

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
            Queue a("element not found - find by id", "12.11.19");
            Lodger lodger(12345678,3,2014,4,60.3f,3,87.7f);
            a.add(lodger);
            [[maybe_unused]] auto it = a.find_by_ID(12345670);
        }());

        CHECK_THROWS([&](){
            Queue a("element not found - find before date", "12.11.19");
            Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
            Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
            a.add(lodger_1);
            a.add(lodger_2);
            auto array = a.find_before_date(2011,5); // попытка поиска элемента в контейнере до первого элемента
        }());

        CHECK_THROWS([&](){
            Queue a("element not found - find before date", "12.11.19");
            Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
            Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
            a.add(lodger_1);
            a.add(lodger_2);
            auto array = a.find_before_date(2007,3); // попытка поиска элемента до существующих дат
        }());

        CHECK_THROWS([&](){
            Queue a("element not found - find after date", "12.11.19");
            Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
            Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
            a.add(lodger_1);
            a.add(lodger_2);
            auto array = a.find_after_date(2014,2); // попытка поиска элемента после последнего элемента
        }());

        CHECK_THROWS([&](){
            Queue a("element not found - find before date", "12.11.19");
            Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
            Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
            a.add(lodger_1);
            a.add(lodger_2);
            auto array = a.find_after_date(2077,7); // попытка поиска элемента после существующих дат
        }());

        CHECK_THROWS([&](){
            Queue a("element not found - find by area", "12.11.19");
            Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
            Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
            a.add(lodger_1);
            a.add(lodger_2);
            // попытка поиска элементов с площадью больше указанной, но указанная площадь превышает все возможные значения элементов
            auto array = a.find_by_area(100);
        }());
    }
}

