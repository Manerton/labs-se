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
        cout << "\n0. Создание треугольника Tri0 без инициализации (конструктор по умолчанию, т.е стороны равны 2, 3, 4) и последующий вывод треугольника Tri0." << endl;
        Triangle Tri0;
        Tri0.Display();
        cout << "\n1.1 Создание треугольника Tri1, инициализация значениями 2, 4, 5 и последующий вывод треугольника Tri1." << endl;
        Triangle Tri1(2,4,5);
        Tri1.Display();
        cout << "\n1.2. Создание треугольника TriTri, инициализация значениями 4, 5, 6 через { }, и последующий вывод треугольника TriTri." << endl;
        Triangle TriTri={4,5,6};
        TriTri.Display();
        cout << "\nРазмер класса в байтах: "; // в режиме pragma pack (1) будет такой же размер, так как все три поля класса одного типа double
        cout << sizeof(Tri1) << endl;
        cout << "\n2. Создание треугольника Tri2, инициализация значенями введенными с клавиатуры." << endl;
        Triangle Tri2;
        Tri2.Read();
        Tri2.Display();
        cout << "\n3. Создание треугольника Tri3, инициализация объектом Tri2 и последующий вывод Tri3." << endl;
        Triangle Tri3 = Tri2;
        Tri3.Display();
        cout << "\n4. Поиск периметра треугольника Tri3: ";
        cout << Tri3.FindPerimeter() << endl;
        cout << "\n5. Поиск площади Tri3: ";
        cout << Tri3.FindSquare() << endl;
        cout << "\n6. Определение вида треугольника Tri3: " << endl;
        int Kind = Tri3.TriangleKind();
        if (Kind == Triangle::pryamougolniy) cout << "Треугольник является прямоугольным." << endl;
        if (Kind == Triangle::typougolniy) cout << "Треугольник является тупоугольным." << endl;
        if (Kind == Triangle::ostrougolniy) cout << "Треугольник является остроугольным." << endl;
        if (Kind == Triangle::ravnostoroniy) cout << "Треугольник является равносторонним." << endl;
        if (Kind == Triangle::ravnobedreniy) cout << "Треугольник является равнобедренным." << endl;
        cout << "\n7. Вычисление высот треугольника Tri3: " << endl;
        cout << "Высота, лежащая на стороне A: " << Tri3.FindHeightA() << endl;
        cout << "Высота, лежащая на стороне B: " << Tri3.FindHeightB() << endl;
        cout << "Высота, лежащая на стороне C: " << Tri3.FindHeightC() << endl;
        cout << "\n8. Вычисление углов треугольника Tri3: " << endl;
        cout << "Угол между A и B: " << Tri3.FindAngleAB() << endl;
        cout << "Угол между A и C: " << Tri3.FindAngleAC() << endl;
        cout << "Угол между B и C: " << Tri3.FindAngleBC() << endl;
        cout << "\n9. Сравнение площадей треугольника Tri3 и Tri1: " << endl;
        if (Tri3.Compare_square(Tri1) == 0) cout << "Площади треугольников равны." << endl;
        if (Tri3.Compare_square(Tri1) == -1) cout << "Площадь левого треугольника меньше площади правого." << endl;
        if (Tri3.Compare_square(Tri1) == 1) cout << "Площадь левого треугольника больше площади правого." << endl;
        cout << "\n10. Сравнение периметров треугольника Tri3 и Tri1: " << endl;
        if (Tri3.Compare_perimeter(Tri1) == 0) cout << "Периметры треугольников равны." << endl;
        if (Tri3.Compare_perimeter(Tri1) == -1) cout << "Периметр левого треугольника меньше периметра правого." << endl;
        if (Tri3.Compare_perimeter(Tri1) == 1) cout << "Периметр левого треугольника больше периметра правого." << endl;
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
