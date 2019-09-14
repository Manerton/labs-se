#include <iostream>
#include <string>
#include "Triangle.h"

using namespace std;

int main()
{

    system("chcp 1251 > nul");
    cout << "Лабораторная работа №1. Вариант 7.\nАвтор: Катунин Сергей. ДИПРБ-21.\n" << endl;
    cout << "Условие: Разработать класс Triangle, представляющий собой треугольник на плоскости, определенный по трем сторонам." << endl << endl;
    cout << "Требуется реализовать следующие методы: вычисление площади и периметра, определение вида треугольника, вычисление высот, вычисление углов," << endl;
    cout << "сравнение треугольников по площади и по периметру, определение подобия треугольников по трем сторонам." << endl << endl;
    try // ищем исключения, которые выбрасываются в блоке try и отправляем их для обработки в блок(и) catch
    {
        cout << "1. Создание треугольника Tri1, инициализация значениями 2, 4, 5 и последующий вывод треугольника Tri1." << endl;
        Triangle Tri1;
        Tri1.Init(2, 4, 5);
        Tri1.Display();
        cout << "\nРазмер класса в байтах: "; // в режиме pragma pack (1) будет такой же размер, так как все три поля класса одного типа double
        cout << sizeof(Tri1) << endl;
        cout << "\n2. Создание треугольника Tri2, инициализация значенями введенными с клавиатуры." << endl;
        Triangle Tri2;
        Tri2.Read();
        Tri2.Display();
        cout << "\n3. Создание треугольника Tri3, инициализация объектом Tri2 и последующий вывод Tri3." << endl;
        Triangle Tri3;
        Tri3.Init(Tri2);
        Tri3.Display();
        cout << "\n4. Поиск периметра треугольника Tri3: ";
        cout << Tri3.FindPerimeter() << endl;
        cout << "\n5. Поиск площади Tri3: ";
        cout << Tri3.FindSquare() << endl;
        cout << "\n6. Определение вида треугольника Tri3: " << endl;
        if (Tri3.TriangleKind() == 0) cout << "Треугольник является прямоугольным." << endl;
        if (Tri3.TriangleKind() == 1) cout << "Треугольник является тупоугольным." << endl;
        if (Tri3.TriangleKind() == -1) cout << "Треугольник является остроугольным." << endl;
        cout << "\n7. Вычисление высот треугольника Tri3: " << endl;
        cout << "Высота, лежащая на стороне A: " << Tri3.FindHeightA() << endl;
        cout << "Высота, лежащая на стороне B: " << Tri3.FindHeightB() << endl;
        cout << "Высота, лежащая на стороне C: " << Tri3.FindHeightC() << endl;
        cout << "\n8. Вычисление углов треугольника Tri3: " << endl;
        cout << "Угол между A и B: " << Tri3.FindAngleAB() << endl;
        cout << "Угол между A и C: " << Tri3.FindAngleAC() << endl;
        cout << "Угол между B и C: " << Tri3.FindAngleBC() << endl;
        cout << "\n9. Сравнение площадей треугольника Tri3 и Tri1: " << endl;
        if (Tri3.Compare_square(Tri1) == 0) cout << "Площадь треугольников равны." << endl;
        if (Tri3.Compare_square(Tri1) == -1) cout << "Площадь первого треугольника меньше площади второго." << endl;
        if (Tri3.Compare_square(Tri1) == 1) cout << "Площадь первого треугольника больше площади второго." << endl;
        cout << "\n10. Сравнение периметров треугольника Tri3 и Tri1: " << endl;
        if (Tri3.Compare_perimeter(Tri1) == 0) cout << "Периметры треугольников равны." << endl;
        if (Tri3.Compare_perimeter(Tri1) == -1) cout << "Периметр первого треугольника меньше периметра второго." << endl;
        if (Tri3.Compare_perimeter(Tri1) == 1) cout << "Периметр первого треугольника больше периметра второго." << endl;
        cout << "\n11. Определение подобия треугольников Tri3 и Tri1: " << endl;
        if(Tri3.Podobie(Tri1)) cout << "Треугольники подобные." << endl;
        else cout << "Треугольники неподобные." << endl;

    }
    catch (const char* exception) // обработка исключений типа const char*
    {
        cerr << "Ошибка: " << exception << endl;
    }
    return 0;
}
