#include "queue.h"
#include "doctest.h"
#include <iostream>
#include <chrono>
#include <random>
#include <fstream>

using std::cout;
using std::endl;
using namespace std::chrono;
using sec = duration<double, std::ratio<1>>;
using lodger = Queue::Container::value_t;


std::string readFromFile(const std::string &flName)
{
    std::string A;
    std::ifstream B(flName);
    std::getline(B,A,'\0');
    B.close();
    return A;
}

// -- непосредственно тесты -

TEST_CASE("Тесты класса hash")
{
    SUBCASE("Тесты добавления, удаления, изменения в таблице для 1-ой хеш-функции")
    {
        hash b(hash::divideHash); // хеш-функция остаток от деления ключа на размер таблицы
        lodger a_lodger(3,2017,4,62.3f,3,83.7f);
        lodger b_lodger(3,2014,4,60.3f,3,87.7f);
        lodger c_lodger(3,2011,4,57.3f,3,89.7f);
        // добавляем
        b.insert(423,b_lodger); // коллизия, так как 423 % 257 = 166
        b.insert(166,c_lodger);
        b.insert(8,a_lodger);
        b.insert(1,c_lodger);
        CHECK(b.count() == 4); // добавилось
        // корректно ищется по этим ключам
        CHECK(elemToString(b.find(423)) == "423 3 2014 4 60.3 3 87.7");
        CHECK(elemToString(b.find(166)) == "166 3 2011 4 57.3 3 89.7");
        // изменяем
        b.replace(8,c_lodger);
        CHECK(elemToString(b.find(8)) == "8 3 2011 4 57.3 3 89.7");
        // удаляем
        b.erase(8);
        CHECK(elemToString(b.find(8)) != "8 3 2014 4 60.3 3 87.7");
    }
    SUBCASE("Тесты для 2-ой хеш-функции")
    {
        // считать байты ключа коэффициентами многочлена,
        // вычислить значение многочлена по схеме Горнера для х = 23,
        // получить остаток от деления на размер таблицы
        hash b(hash::gornerDivide); // хеш-функция остаток от деления ключа на размер таблицы
        lodger b_lodger(3,2014,4,60.3f,3,87.7f);
        lodger c_lodger(3,2011,4,57.3f,3,89.7f);
        // добавляем
        b.insert(423,b_lodger);
        CHECK(b.callCurrentHashFunc(423) == 109);
        // проверяем индексы и ключи
        CHECK(((b.begin()+109)->key()) == b.get_key(b.find(423)));
        b.insert(166,c_lodger);
        CHECK(b.callCurrentHashFunc(166) == 159);
        CHECK(((b.begin()+159)->key()) == b.get_key(b.find(166)));
    }
    SUBCASE("Тесты для 3-ой хеш-функции")
    {
        // считать байты ключа коэффициентами многочлена,
        // вычислить значение многочлена по схеме Горнера для х = 23,
        // получить остаток от деления на размер таблицы
        hash b(hash::gornerHighlight); // хеш-функция остаток от деления ключа на размер таблицы
        lodger b_lodger(3,2014,4,60.3f,3,87.7f);
        lodger c_lodger(3,2011,4,57.3f,3,89.7f);
        // добавляем
        b.insert(423,b_lodger);
        CHECK(b.callCurrentHashFunc(423) == 3);
        // проверяем индексы и ключи
        CHECK(((b.begin()+3)->key()) == b.get_key(b.find(423)));
        b.insert(166,c_lodger);
        CHECK(b.callCurrentHashFunc(166) == 6);
        CHECK(((b.begin()+6)->key()) == b.get_key(b.find(166)));
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
    SUBCASE("Тесты добавлений в таблицу")
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
        CHECK(elemToString({423,b.find(423)}) == "423 3 2014 4 60.3 3 87.7");
        CHECK(elemToString({166,b.find(166)}) == "166 3 2011 4 57.3 3 89.7");
        // изменяем
        b.replace(8,c_lodger);
        CHECK(elemToString({8,b.find(8)}) == "8 3 2011 4 57.3 3 89.7");
        // удаляем
        b.erase(8);
        CHECK(elemToString({8,b.find(8)}) != "8 3 2014 4 60.3 3 87.7");
    }
}
