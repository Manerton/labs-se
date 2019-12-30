#include "doctest.h"
#include "triad.h"
#include "vector3d.h"

TEST_CASE("Тесты класса Triad в doctest")
{
    Triad t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        Triad d(3,4,155);
        CHECK(d.toString() == "3 4 155");

        Triad two;
        CHECK(two.toString() == "0 0 0");
        two = d;
        CHECK(two.toString() == "3 4 155");
        two = {184568, 0, 77};
        CHECK(two.toString() == "184568 0 77");
    }
    SUBCASE("Тест методов get и set") // -- создание и присваивание --
    {
        Triad d(3,4,155);
        CHECK(d.toString() == "3 4 155");
        CHECK(d.getNum1() == 3);
        CHECK(d.getNum2() == 4);
        CHECK(d.getNum3() == 155);
        Triad two;
        CHECK(two.toString() == "0 0 0");
        two.setNum1(424);
        CHECK(two.toString() == "424 0 0");
        two.setNum3(-15);
        CHECK(two.toString() == "424 0 -15");
        two.setNum2(5551);
        CHECK(two.toString() == "424 5551 -15");
        two.setNum1(0);
        CHECK(two.toString() == "0 5551 -15");
    }
    SUBCASE("Тест сложения Triad с числом")
    {
        Triad d(188, 0, -545);
        CHECK(d.toString() == "188 0 -545");
        d += 110;
        CHECK(d.toString() == "298 110 -435");
        Triad a(188, -20, 3);
        CHECK(a.toString() == "188 -20 3");
        a.add_to_num1(1311);
        CHECK(a.toString() == "1499 -20 3");
        a.add_to_num2(20);
        CHECK(a.toString() == "1499 0 3");
        a.add_to_num3(-10);
        CHECK(a.toString() == "1499 0 -7");
    }
    SUBCASE("Тест умножение Triad на число")
    {
        Triad d(188, 0, -545);
        CHECK(d.toString() == "188 0 -545");
        d *= 7;
        CHECK(d.toString() == "1316 0 -3815");
        Triad a(188, -20, 3);
        CHECK(a.toString() == "188 -20 3");
        a.mul_num1(2);
        CHECK(a.toString() == "376 -20 3");
        a.mul_num2(0);
        CHECK(a.toString() == "376 0 3");
        a.mul_num3(-10);
        CHECK(a.toString() == "376 0 -30");
    }
    SUBCASE("Тест проверки на равенство")          // -- проверка сравнения --
    {
        Triad a(188, 0, -545);
        Triad b(188, 144, -545);
        Triad c(188, 0, -545);
        Triad d(24542, -22, -111);

        CHECK(a != b);
        CHECK(a == c);
        CHECK(c != b);
        CHECK(c != d);
        CHECK(a != d);
    }
}

TEST_CASE("Тесты класса vector3D в doctest")
{
    vector3D t;         // -- нулевой объект – нужен всем --
    SUBCASE("Тест создание и присваивание") // -- создание и присваивание --
    {
        CHECK(t.toString() == "0 0 0");

        // -- проверка создания вектора -- //
        vector3D vec(3,4,155);
        CHECK(vec.toString() == "3 4 155");
        // -- демонстрация принципа подстановки -- //
        Triad two(vec);
        CHECK(two.toString() == "3 4 155");
        // -- еще одна -- //
        Triad twodd = vec;
        CHECK(twodd.toString() == "3 4 155");
        // -- еще одна, только с указателем -- //
        Triad *ptr_triad = new vector3D(5,11,-44);
        CHECK(ptr_triad->toString() == "5 11 -44");
        // -- инициализируем вектор объектом базового класса -- //
        vector3D dd(two);
        CHECK(dd.toString() == "3 4 155");
        // -- присвоим вектору объект базового класса -- //
        vector3D ddd;
        CHECK(ddd.toString() == "0 0 0");
        ddd = twodd;
        CHECK(ddd.toString() == "3 4 155");
        // -- uniform-инициализация -- //
        Triad uni = {184568, 0, 77};
        CHECK(uni.toString() == "184568 0 77");
    }
    SUBCASE("Тест методов get и set") // -- создание и присваивание --
    {
        vector3D d(3,4,155);
        CHECK(d.toString() == "3 4 155");
        CHECK(d.getNum1() == 3);
        CHECK(d.getNum2() == 4);
        CHECK(d.getNum3() == 155);
        vector3D two;
        CHECK(two.toString() == "0 0 0");
        two.setNum1(424);
        CHECK(two.toString() == "424 0 0");
        two.setNum3(-15);
        CHECK(two.toString() == "424 0 -15");
        two.setNum2(5551);
        CHECK(two.toString() == "424 5551 -15");
        two.setNum1(0);
        CHECK(two.toString() == "0 5551 -15");
    }
    SUBCASE("Тест сложения векторов")
    {
        vector3D vec1(5, 7, 33);
        CHECK(vec1.toString() == "5 7 33");
        vector3D vec2(-3, 3, 11);
        CHECK(vec2.toString() == "-3 3 11");
        vec1 += vec2;
        CHECK(vec1.toString() == "2 10 44");
    }
    SUBCASE("Тест скалярного умножения векторов")
    {
        vector3D vec1(5, 7, 33);
        CHECK(vec1.toString() == "5 7 33");
        vector3D vec2(-3, 3, 11);
        CHECK(vec2.toString() == "-3 3 11");
        vec1 *= vec2;
        CHECK(vec1.toString() == "-15 21 363");
    }
    SUBCASE("Тест проверки на равенство")          // -- проверка сравнения --
    {
        vector3D a(188, 0, -545);
        vector3D b(188, 144, -545);
        vector3D c(188, 0, -545);
        vector3D d(24542, -22, -111);

        CHECK(a != b);
        CHECK(a == c);
        CHECK(c != b);
        CHECK(c != d);
        CHECK(a != d);
    }
}
