#include <vector>
#include <iostream>
#include <chrono>
#include <random>
#include "doctest.h"
#include "queue.h"

using std::cout;
using std::endl;
using Lodger = Queue::Lodger;
using namespace std::chrono;
using sec = duration<double, std::ratio<1>>;

Queue::value_type generateElement()
{
    time_t seed = time(nullptr);
    std::mt19937 mt(seed);
    // диапазоны для рандома
    std::uniform_int_distribution<uint64_t> ID(1,99999999);
    std::uniform_int_distribution<uint8_t> family_count(1,20); // до 20 детей в семье, к примеру
    std::uniform_int_distribution<uint16_t> year(2000,2020);
    std::uniform_int_distribution<uint8_t> month(1,12);
    std::uniform_real_distribution<float> area_occupied(0,100); // пусть до 100 кв метров
    std::uniform_int_distribution<uint8_t> rooms_count(1,12);
    std::uniform_real_distribution<float> area_required(0,200);

    float old_area = area_occupied(mt);
    float new_area = area_required(mt);
    while (new_area <= old_area) new_area = area_required(mt);
    return Lodger(ID(mt),family_count(mt),year(mt),month(mt),old_area,rooms_count(mt),new_area);
}

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

    SUBCASE("1 задание - заполнение контейнера случайными данными (алгоритмы generate(), generate_n())")
    {
        size_t count = 20;
        Queue::Container list(count);
        std::generate(list.begin(),list.end(),generateElement);
        CHECK(t.get_ID(list.front()) != 0);
        CHECK(t.get_ID(list.back()) != 0);

        Queue::Container list2(count);
        std::generate_n(list2.begin(),10,generateElement);
        CHECK(t.get_ID(list2.front()) != 0);
        CHECK(t.get_ID(list2.back()) == 0);
    }

    SUBCASE("2 задание - сортировка списка по 8-байтовому номеру")
    {
        Queue a("Vanya", "17.11.18");
        Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
        Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
        Lodger lodger_3(12452   ,5,2017,3,54.3f,4,60.5f);
        Lodger lodger_4(7       ,3,2019,4,60.3f,3,87.7f);
        Lodger lodger_5(141444  ,1,2010,1,44.8f,3,88.2f);
        Lodger lodger_6(448     ,1,2010,2,41.8f,3,69.2f);
        Lodger lodger_7(51     ,1,2010,2,41.8f,3,69.2f);
        a.add(lodger_1);
        a.add(lodger_2);
        a.add(lodger_3);
        a.add(lodger_4);
        a.add(lodger_5);
        a.add(lodger_6);
        a.add(lodger_7);
        quick_sort(a.begin(),a.begin(),--a.end());
        auto it = a.begin();
        CHECK(*it++ == lodger_4);
        CHECK(*it++ == lodger_2);
        CHECK(*it++ == lodger_7);
        CHECK(*it++ == lodger_6);
        CHECK(*it++ == lodger_3);
        CHECK(*it++ == lodger_5);
        CHECK(*it == lodger_1);
    }

    SUBCASE("3 задание - удалить дубликаты")
    {
        Queue a("Vanya", "17.11.18");
        Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
        Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
        Lodger lodger_3(12345678,2,2014,2,22.1f,5,71.3f);
        Lodger lodger_4(7       ,3,2019,4,60.3f,3,87.7f);
        Lodger lodger_5(141444  ,1,2010,1,44.8f,3,88.2f);
        Lodger lodger_6(12345678,2,2014,2,22.1f,5,71.3f);
        Lodger lodger_7(51     ,1,2010,2,41.8f,3,69.2f);
        a.add(lodger_1);
        a.add(lodger_2);
        a.add(lodger_3);
        a.add(lodger_4);
        a.add(lodger_5);
        a.add(lodger_6);
        a.add(lodger_7);
        CHECK(a.get_Count_of_people() == 7);
        quick_sort(a.begin(),a.begin(),--a.end());
        CHECK(a.get_Count_of_people() == 7);
        a.unique();
        CHECK(a.get_Count_of_people() == 5);
    }

    SUBCASE("4 задание - случайно переставить элементы")
    {
        Queue a("Vanya", "17.11.18");
        Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
        Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
        Lodger lodger_3(12452   ,5,2017,3,54.3f,4,60.5f);
        Lodger lodger_4(7       ,3,2019,4,60.3f,3,87.7f);
        Lodger lodger_5(141444  ,1,2010,1,44.8f,3,88.2f);
        Lodger lodger_6(448     ,1,2010,2,41.8f,3,69.2f);
        Lodger lodger_7(51     ,1,2010,2,41.8f,3,69.2f);
        a.add(lodger_1);
        a.add(lodger_2);
        a.add(lodger_3);
        a.add(lodger_4);
        a.add(lodger_5);
        a.add(lodger_6);
        a.add(lodger_7);
        a.shuffle();
        auto it = a.begin();
        bool check = (*it++ == lodger_1) && (*it++ == lodger_2)
                     && (*it++ == lodger_3) && (*it++ == lodger_4)
                     && (*it++ == lodger_5) && (*it++ == lodger_6) && (*it == lodger_7);
        CHECK(check == false);
    }
    SUBCASE("5 задание - реализовать алгоритм бинарного поиска 8-байтовому номеру")
    {
        Queue a("Vanya", "17.11.18");
        Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
        Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
        Lodger lodger_3(12452   ,5,2017,3,54.3f,4,60.5f);
        Lodger lodger_4(7       ,3,2019,4,60.3f,3,87.7f);
        Lodger lodger_5(141444  ,1,2010,1,44.8f,3,88.2f);
        Lodger lodger_6(448     ,1,2010,2,41.8f,3,69.2f);
        Lodger lodger_7(51     ,1,2010,2,41.8f,3,69.2f);
        a.add(lodger_1);
        a.add(lodger_2);
        a.add(lodger_3);
        a.add(lodger_4);
        a.add(lodger_5);
        a.add(lodger_6);
        a.add(lodger_7);
        quick_sort(a.begin(),a.begin(),--a.end());
        CHECK(a.get_Count_of_people() == 7);
        CHECK(*(a.begin()) == lodger_4);
        auto it = a.binarySearch_by_ID(7);
        CHECK(a.lodger_toString(*it) == "7 3 2019 4 60.3 3 87.7");
        it = a.binarySearch_by_ID(12452);
        CHECK(*it == lodger_3);
        it = a.binarySearch_by_ID(448);
        CHECK(*it == lodger_6);
        it = a.binarySearch_by_ID(14);
        CHECK(it == a.end());

    }
    SUBCASE("6 задание - реализовать алгоритм интерполяционного поиска 8-байтовому номеру")
    {
        Queue a("Vanya", "17.11.18");
        Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
        Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
        Lodger lodger_3(12452   ,5,2017,3,54.3f,4,60.5f);
        Lodger lodger_4(7       ,3,2019,4,60.3f,3,87.7f);
        Lodger lodger_5(141444  ,1,2010,1,44.8f,3,88.2f);
        Lodger lodger_6(448     ,1,2010,2,41.8f,3,69.2f);
        Lodger lodger_7(51     ,1,2010,2,41.8f,3,69.2f);
        a.add(lodger_1);
        a.add(lodger_2);
        a.add(lodger_3);
        a.add(lodger_4);
        a.add(lodger_5);
        a.add(lodger_6);
        a.add(lodger_7);
        quick_sort(a.begin(),a.begin(),--a.end());
        CHECK(a.get_Count_of_people() == 7);
        CHECK(*(a.begin()) == lodger_4);
        auto it = a.interpolarSearch_by_ID(7);
        CHECK(a.lodger_toString(*it) == "7 3 2019 4 60.3 3 87.7");
        it = a.interpolarSearch_by_ID(12452);
        CHECK(*it == lodger_3);
        it = a.interpolarSearch_by_ID(448);
        CHECK(*it == lodger_6);
        it = a.interpolarSearch_by_ID(14);
        CHECK(it == a.end());
    }
    SUBCASE("7 задание - разделить список на два списка по критерию")
    {
        Queue a("Vanya", "17.11.18");
        Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
        Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
        Lodger lodger_3(12452   ,5,2017,3,54.3f,4,60.5f);
        Lodger lodger_4(7       ,3,2019,4,60.3f,3,87.7f);
        Lodger lodger_5(141444  ,1,2010,1,44.8f,3,88.2f);
        Lodger lodger_6(448     ,1,2010,2,41.8f,3,69.2f);
        Lodger lodger_7(51     ,1,2010,2,41.8f,3,69.2f);
        a.add(lodger_1);
        a.add(lodger_2);
        a.add(lodger_3);
        a.add(lodger_4);
        a.add(lodger_5);
        a.add(lodger_6);
        a.add(lodger_7);
        quick_sort(a.begin(),a.begin(),--a.end());
        CHECK(a.get_Count_of_people() == 7);
        Queue b = a.split_by_ID(448);
        CHECK(a.get_Count_of_people() == 4);
        CHECK(b.get_Count_of_people() == 3);
        auto it = a.begin();
        CHECK(*it++ == lodger_4);
        CHECK(*it++ == lodger_2);
        CHECK(*it++ == lodger_7);
        CHECK(*it == lodger_6);
        it = b.begin();
        CHECK(*it++ == lodger_3);
        CHECK(*it++ == lodger_5);
        CHECK(*it++ == lodger_1);
    }

    SUBCASE("8 задание - сортировка слиянием")
    {
        Queue a("Vanya", "17.11.18");
        Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
        Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
        Lodger lodger_3(12452   ,5,2017,3,54.3f,4,60.5f);
        Lodger lodger_4(7       ,3,2019,4,60.3f,3,87.7f);
        Lodger lodger_5(141444  ,1,2010,1,44.8f,3,88.2f);
        Lodger lodger_6(448     ,1,2010,2,41.8f,3,69.2f);
        Lodger lodger_7(51     ,1,2010,2,41.8f,3,69.2f);
        a.add(lodger_1);
        a.add(lodger_2);
        a.add(lodger_3);
        a.add(lodger_4);
        a.add(lodger_5);
        a.add(lodger_6);
        a.add(lodger_7);
        a.mergeSort(0,a.get_Count_of_people()-1);
        auto it = a.begin();
        CHECK(*it++ == lodger_4);
        CHECK(*it++ == lodger_2);
        CHECK(*it++ == lodger_7);
        CHECK(*it++ == lodger_6);
        CHECK(*it++ == lodger_3);
        CHECK(*it++ == lodger_5);
        CHECK(*it == lodger_1);
    }

    SUBCASE("9 задание - слить два отсортированных списка в один")
    {
        Queue a("Vanya", "17.11.18");
        Lodger lodger_1(12345678,2,2014,2,22.1f,5,71.3f);
        Lodger lodger_2(17      ,2,2011,5,48.0f,2,55.9f);
        Lodger lodger_3(12452   ,5,2017,3,54.3f,4,60.5f);
        Lodger lodger_4(7       ,3,2019,4,60.3f,3,87.7f);
        Lodger lodger_5(141444  ,1,2010,1,44.8f,3,88.2f);
        Lodger lodger_6(448     ,1,2010,2,41.8f,3,69.2f);
        Lodger lodger_7(51     ,1,2010,2,41.8f,3,69.2f);
        a.add(lodger_1);
        a.add(lodger_2);
        a.add(lodger_3);
        a.add(lodger_4);
        a.add(lodger_5);
        a.add(lodger_6);
        a.add(lodger_7);
        // сортируем
        quick_sort(a.begin(),a.begin(),--a.end());
        CHECK(a.get_Count_of_people() == 7);
        // разделяем на два
        Queue b = a.split_by_ID(448);
        CHECK(a.get_Count_of_people() == 4);
        CHECK(b.get_Count_of_people() == 3);
        auto it = a.begin();
        CHECK(*it++ == lodger_4);
        CHECK(*it++ == lodger_2);
        CHECK(*it++ == lodger_7);
        CHECK(*it == lodger_6);
        it = b.begin();
        CHECK(*it++ == lodger_3);
        CHECK(*it++ == lodger_5);
        CHECK(*it++ == lodger_1);
        // сливаем обратно
        a.merge(b);
        CHECK(a.get_Count_of_people() == 7);
        CHECK(b.get_Count_of_people() == 3);
        // проверяем слияние
        it = a.begin();
        CHECK(*it++ == lodger_4);
        CHECK(*it++ == lodger_2);
        CHECK(*it++ == lodger_7);
        CHECK(*it++ == lodger_6);
        CHECK(*it++ == lodger_3);
        CHECK(*it++ == lodger_5);
        CHECK(*it == lodger_1);
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
    }
}

