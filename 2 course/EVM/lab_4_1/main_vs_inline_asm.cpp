#include <iostream>
#include <cassert>

using namespace std;

int asm_func(int x, int y)
{
    int drob1 = 0;    // результат первой дроби: (5x-10y)/(2y)
    int res = 0;    // результат всего выражения
    __asm
       {
       mov eax, x       // поместим значение переменной x в регистр eax
       mov ebx, y       // поместим значение y -> ebx
       imul eax, 5      // умножим eax на 5 и получим 5x
       imul ebx, 10     // умножим ebx на 10 и получим 10y
       sub eax, ebx     // вычтем и получим (5x-10y)
       mov ebx, y       // поместим значение y -> ebx
       add ebx, ebx     // y + y = 2y
       cdq              // преобразовываем двойное слово в учетверенное (иначе переполнение)
                        // cdq копирует знак (бит 31) из EAX в каждую битовую позицию регистра EDX
       idiv ebx         // деление (5x-10y) / 2y, edx:eax / ebx, результат в eax, остаток в edx
       mov drob1, eax   // сохраним результат первой дроби
       mov eax, ebx     // помещаем 2y в eax
       imul eax, 3      // умножаем на 3 и получаем 6y
       mov ebx, x       // помещаем x в ebx
       imul ebx, 3      // умножаем на 3 и получаем 3x
       sub eax, ebx     // получаем выражение (6y-3x)
       mov ebx, y
       imul ebx, ebx    // получаем y^2
       imul ebx, 8      // получаем 8y^2
       cdq
       idiv ebx         // деление (6y-3x) / (8y^2)
       mov ebx, eax     // кладем результат в ebx - будущий делитель
       mov eax, drob1   // кладем в eax главный числитель
       cdq
       idiv ebx
       mov res, eax
    }
    return res;
}

inline int func(int x, int y)
{
    return ((5*x - 10*y)/(y+y))/((6*y-3*x)/(8*y*y));
}

int main()
{
    system("chcp 1251 > nul");
    cout << "Лабораторная работа №4. Проект 1. Вариант 2.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;

    int res = asm_func(5,1);
    assert(res == func(5,1));
    cout << res << endl;

    res = asm_func(2,-1);
    assert(res == func(2,-1));
    cout << res << endl;

    res = asm_func(-1,1);
    assert(res == func(-1,1));
    cout << res << endl;

    res = asm_func(33,2);
    assert(res == func(33,2));
    cout << res << endl;
    return 0;
}
