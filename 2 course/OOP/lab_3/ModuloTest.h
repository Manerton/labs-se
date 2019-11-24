#ifndef MODULOTEST_H_INCLUDED
#define MODULOTEST_H_INCLUDED

#include "modulo.h"
#include "UnitTest.h"

using namespace UnitTest;
// -- в тестовых методах пишутся вызовы макроса check() --
class ModuloTest: public Test
{
    Modulo t;
public:
    // - определение чистой виртуальной функции run() класса Test --
    void run() override                 // -- вызов тестовых методов --
    {
        testCreate();		// -- тест конструкторов и присваивания --
        testAdd();			// -- тест сложения --
        testMinus();        // -- тест вычитания --
        testMult();         // -- тест умножения --
        testDiv();          // -- тест деления --
        testCompare();		// -- тест сравнения
        test_zero_module_exception();		// -- тест исключений --
        test_different_modules_exception();
        test_inverse_elem_not_exist_exception();
    }
    // -- тестовые методы --
    void testCreate()   		// -- создание и присваивание --
    {
        Modulo two;      	check(t == two);
                            check(two.toString() == "0 mod 10");
        Modulo d(3,7);      check(d.toString() == "3 mod 7");
        Modulo dd(10,6);    check(dd.toString() == "4 mod 6");
        two = dd;           check(two.toString() == "4 mod 6");
        two = {3, 7};      check(d.toString() == "3 mod 7");
        two = {5, 12};      check(d.toString() == "7 mod 11");    	// -- ошибка -- должно быть 5 mod 12 --
    }
    void testAdd()       		// -- тестирование сложения --
    {
        Modulo one;
        one += 1;
        check(one.toString() == "1 mod 10");
        Modulo two = (one + 1);
        check (two.toString() == "2 mod 10");
        check((1 + one) == two);
        check((two+one).toString() == "3 mod 10");
        check((two+=one).toString() == "3 mod 10");
        check(two.toString() == "3 mod 10");
        Modulo aa(3,7);
        Modulo bb(5,7);
        check ((aa+bb).toString() == "1 mod 7");
        check ((aa+4).toString() == "0 mod 7");
        check ((aa++).toString() == "3 mod 7");
        check (aa.toString() == "4 mod 7");
        check ((++aa).toString() == "5 mod 7");
        check ((4+bb).toString() == "2 mod 7");
        check ((1+bb).toString() == "5 mod 7"); // -- ошибка - должно быть 6 mod 7 --
    }
    void testMinus()       		// -- тестирование вычитания --
    {
        Modulo one(3, 8);
        one -= 2;
        check(one.toString() == "1 mod 8");

        Modulo zero = (one - 1);
        check (zero.toString() == "0 mod 8");
        check((1 - one) == zero);
        Modulo four(4,8);
        check((four-one).toString() == "3 mod 8");
        check((four-=one).toString() == "3 mod 8");
        check(four.toString() == "3 mod 8");
        Modulo aa(3,7);
        Modulo bb(5,7);
        check ((aa-bb).toString() == "5 mod 7");
        check ((aa-4).toString() == "6 mod 7");
        check ((aa--).toString() == "3 mod 7");
        check (aa.toString() == "2 mod 7");
        check ((--aa).toString() == "1 mod 7");
        check ((1-bb).toString() == "5 mod 7"); // -- ошибка - должно быть 3 mod 7 --
    }
    void testMult()       		// -- тестирование умножения --
    {
        Modulo three(1, 6);
        three *= 3;
        check(three.toString() == "3 mod 6");

        Modulo zero = (three * 6);
        check (zero.toString() == "0 mod 6");
        check((2 * three) == zero);
        Modulo four(4,6);
        check((four*three).toString() == "0 mod 6");
        check((four*=three).toString() == "0 mod 6");
        check(four.toString() == "0 mod 6");
        Modulo aa(3,7);
        Modulo bb(5,7);
        check ((aa*bb).toString() == "1 mod 7");
        check ((aa*4).toString() == "5 mod 7");
        check ((2*bb).toString() == "5 mod 7"); // -- ошибка - должно быть 3 mod 7 --
    }
    void testDiv()       		// -- тестирование деления --
    {
        Modulo two(3, 7);
        two /= 5;
        check(two.toString() == "2 mod 7");

        Modulo six = (two / 5);
        check (six.toString() == "6 mod 7");
        check((5 / six) == two);
        Modulo four(4,7);
        check((four/two).toString() == "2 mod 7");
        check((four/=two).toString() == "2 mod 7");
        check(four.toString() == "2 mod 7");
        Modulo aa(3,7);
        Modulo bb(4,7);
        check ((aa/bb).toString() == "3 mod 7"); // -- ошибка - должно быть 6 mod 7 --
    }
    void testCompare()    		// -- проверка сравнения --
    {
        Modulo a = {1, 5};
        Modulo b = {3, 5};
        Modulo c = {1, 5};
        Modulo d = {4, 5};

        check(a < b);
        check(d > b);
        check(b < a);       // -- ошибка --
        check(a <= b);
        check(a <= c);
        check(b >= c);
        check(a >= c);
        check(a == c);
        check(b == c);      // -- ошибка --
    }
    void test_zero_module_exception()
    {
        try                       // -- это работает правильно
        {
            Modulo F(5,0);     // -- должен генерить исключение --
            fail();                 // -- проваленный тест по исключениям --
            error("zero module exception undetected");     // -- без препроцессора не обойтись --
        }
        catch(Modulo::zero_module_exception &)
        {
            success();
        }
    }
    void test_different_modules_exception()
    {
        try                       // -- это работает правильно
        {
            Modulo a(1, 5);
            Modulo b(3,7);
            Modulo c = a+b;
            c = {5,3};
            fail();                 // -- проваленный тест по исключениям --
            error("different modules exception undetected");     // -- без препроцессора не обойтись --
        }
        catch(Modulo::different_modules_exception &)
        {
            success();
        }
    }
    void test_inverse_elem_not_exist_exception()
    {
        try                       // -- это работает правильно
        {
            Modulo g(3,8);
            Modulo k(4,8);
            Modulo c = g / k;
            c = {5,3};
            fail();                 // -- проваленный тест по исключениям --
            error("[inverse elem not exist] exception undetected");     // -- без препроцессора не обойтись --
        }
        catch(Modulo::inverse_elem_not_exist_exception &)
        {
            success();
        }
    }

};
#endif  // MODULOTEST_H_INCLUDED
