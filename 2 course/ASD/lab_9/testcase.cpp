#include "queue.h"
#include "btree.h"
#include "doctest.h"
#include <iostream>
#include <chrono>
#include <random>
#include <sstream>
#include <fstream>

static time_t seed = time(nullptr);
static std::mt19937 mt(seed);

using std::cout;
using std::endl;
using namespace std::chrono;
using sec = duration<double, std::ratio<1>>;
using lodger = Queue::lodger;

Queue::value_type generateElement()
{
    // диапазоны для рандома
    std::uniform_int_distribution<uint64_t> ID(1,9999999);
    std::uniform_int_distribution<uint8_t> family_count(1,20); // до 20 детей в семье, к примеру
    std::uniform_int_distribution<uint16_t> year(2000,2020);
    std::uniform_int_distribution<uint8_t> month(1,12);
    std::uniform_real_distribution<float> area_occupied(0,100); // пусть до 100 кв метров
    std::uniform_int_distribution<uint8_t> rooms_count(1,12);
    std::uniform_real_distribution<float> area_required(0,200);

    float old_area = area_occupied(mt);
    float new_area = area_required(mt);
    while (new_area <= old_area) new_area = area_required(mt);
    Queue::lodger lodg = {family_count(mt),year(mt),month(mt),old_area,rooms_count(mt),new_area};
    return Queue::value_type(ID(mt),lodg);
}

std::string elemToString(const Queue::value_type &element)
{
    std::stringstream ss;
    auto [key, lodg] = element;
    ss << key << " "
       << int(std::get<0>(lodg)) << " "
       << std::get<1>(lodg) << " "
       << int(std::get<2>(lodg)) << " "
       << std::get<3>(lodg) << " "
       << int(std::get<4>(lodg)) << " "
       << std::get<5>(lodg);
    return ss.str();
}

std::string readFromFile(const std::string &flName)
{
    std::string A;
    std::ifstream B(flName);
    std::getline(B,A,'\0');
    B.close();
    return A;
}

// -- непосредственно тесты -

TEST_CASE("Тесты класса Btree")
{
    SUBCASE("Тесты добавления, удаления")
    {
        BTree t(2); // параметр t = 2
        t.insert({3,0});
        t.insert({7,0});
        t.insert({1,0});
        t.insert({4,0});
        t.insert({5,0});
        t.insert({16,0});
        t.insert({2,0});
        t.insert({14,0});
        t.insert({9,0});
        t.insert({10,0});
        t.insert({15,0});
        t.insert({12,0});
        t.insert({13,0});
        t.insert({8,0});
        t.insert({11,0});
        t.insert({6,0});
        t.insert({17,0});
        t.insert({17,0});
        t.remove(14);
        CHECK (t.traverseIterative() == " 5 / 3 / 9 12 15 / 1 2 / 4 / 6 7 8 / 10 11 / 13 / 16 17 /");
    }
}

TEST_CASE("Тесты класса Array с BTree в качестве индексации")
{
    SUBCASE("Тесты добавления, удаления, замены")
    {
        // в операторе [] используется функция MESSAGE, которая выведет
        // информацию о ключе и используемом индексе
        // это для отладки
        Array a(10);
        lodger a_lodger(3,2017,4,62.3f,3,83.7f);
        lodger b_lodger(3,2014,4,60.3f,3,87.7f);
        lodger c_lodger(3,2011,4,57.3f,3,89.7f);
        a.push_back({13,a_lodger});
        CHECK(a.size() == 1);
        a.push_back({1535324,c_lodger});
        a.push_back({675868,b_lodger});
        CHECK(a.size() == 3);
        CHECK(elemToString(a.find(13)) == elemToString({13,a_lodger}));
        CHECK(elemToString(a[13]) == elemToString({13,a_lodger}));
        CHECK(elemToString(a[1535324]) == elemToString({1535324,c_lodger}));
        CHECK(elemToString(a[675868]) == elemToString({675868,b_lodger}));
        a.erase(1535324);

        // должно выброситься исключение, что элемента нет
        CHECK_THROWS([&]()
        {
            a[1535324];
        }());

        CHECK(elemToString(a[675868]) == elemToString({675868,b_lodger}));
    }
    SUBCASE("Тест 50000 элементов")
    {
        Array a(50000);
        for (size_t i = 0; i < 50000; ++i)
        {
            a.push_back(generateElement());
        }
        std::cout << a.tree.traverseIterative();
    }
}

TEST_CASE("Тесты Queue")
{
    Queue t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        Queue a("Ivan");
        CHECK(a.get_Name() == "Ivan");
        Queue b("Vanya", "17.11.18");
    }
    SUBCASE("Тесты добавлений")
    {
        Queue b("Vanya", "17.11.18");
        lodger a_lodger(3,2017,4,62.3f,3,83.7f);
        lodger b_lodger(3,2014,4,60.3f,3,87.7f);
        lodger c_lodger(3,2011,4,57.3f,3,89.7f);
        // добавляем
        b.add(423,b_lodger);
        b.add(166,c_lodger);
        b.add(8,a_lodger);
        b.add(1,c_lodger);
        CHECK(b.get_Count_of_people() == 4);
        CHECK(elemToString({b.find(423)}) == "423 3 2014 4 60.3 3 87.7");
        CHECK(elemToString({b.find(166)}) == "166 3 2011 4 57.3 3 89.7");
        // удаляем
        b.erase(8);
        CHECK(elemToString({b.find(8)}) != "8 3 2014 4 60.3 3 87.7");
    }
}
