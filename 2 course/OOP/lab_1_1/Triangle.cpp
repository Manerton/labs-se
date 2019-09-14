#include <iostream>
#include <string>
#include <cmath>
#include "Triangle.h"

using namespace std;

bool Triangle::Check(const double &a_in, const double &b_in, const double &c_in) const // проверка на треугольник
{
    if (a_in > 0 && b_in > 0 && c_in > 0)
    {
        if ( (a_in + b_in > c_in) && (a_in + c_in > b_in) && (b_in + c_in > a_in) )
            return true;
        else throw "сумма длин каждых двух сторон должна быть больше длины третьей стороны.";
    } else {
        throw "у треугольника не может быть отрицательных сторон.";
    }
}

double Triangle::FindHeight(const double &side) const noexcept // поиск высоты по стороне
{
    double Double_Square = FindSquare() * 2;
    double height = Double_Square / side;
    return height;
}

void Triangle::Display() const noexcept
{
    string s = toString();
    cout << "Длины треугольника: " << s << endl;
}

void Triangle::Read()
{
    double a, b, c;
    cout << "Введите длину стороны a: ";
    cin >> a;
    cout << "Введите длину стороны b: ";
    cin >> b;
    cout << "Введите длину стороны c: ";
    cin >> c;

    Init(a, b, c);
}

double Triangle::FindPerimeter() const noexcept
{
    return A + B + C;
}

double Triangle::FindSquare() const noexcept
{
    double p = FindPerimeter() / 2;
    double result_wo_sqrt = p * (p-A) * (p-B) * (p-C);
    return sqrt(result_wo_sqrt);
}

double Triangle::FindAngle(const double &A, const double &B, const double &C) const noexcept // теорема косинусов
{
    double cos_angle = ((A*A)+(B*B)-(C*C)) / (2*A*B);
    double angle = acos(cos_angle) * 180 / M_PI;
    return angle;
}

double Triangle::FindHeightA() const noexcept
{
    return FindHeight(A);
}

double Triangle::FindHeightB() const noexcept
{
    return FindHeight(B);
}

double Triangle::FindHeightC() const noexcept
{
    return FindHeight(C);
}

double Triangle::FindAngleAB() const noexcept
{
    return FindAngle(A,B,C);
}

double Triangle::FindAngleAC() const noexcept
{
    return FindAngle(A,C,B);
}

double Triangle::FindAngleBC() const noexcept
{
    return FindAngle(B,C,A);
}

static double eps = 0.0001; // точность для след. трех функций

int Triangle::TriangleKind() const noexcept // возвращает цифру - тип треугольника
{
    double ab, ac, bc;
    ab = FindAngleAB();
    ac = FindAngleAC();
    bc = FindAngleBC();
    if (fabs(ab - 90) < eps || fabs(ac - 90) < eps || fabs(bc - 90) < eps) return 0; // прямоугольный
    else if (ab > 90 || ac > 90 || bc > 90) return 1; // тупоугольный
    else return -1; // остроугольный
}

int Triangle::Compare_square(const Triangle &second) const noexcept
{
    double square_1 = FindSquare();
    double square_2 = second.FindSquare();
    if (fabs(square_1 - square_2) < eps) return 0; // площади равны
    else if (square_1 < square_2) return -1; // площадь первого меньше
    else return 1; // площадь первого больше
}

int Triangle::Compare_perimeter(const Triangle &second) const noexcept
{
    double P1 = FindPerimeter();
    double P2 = second.FindPerimeter();
    if (fabs(P1 - P2) < eps) return 0; // периметры равны
    else if (P1 < P2) return -1; // периметр первого меньше
    else return 1; //периметр первого больше
}

bool Triangle::Podobie(const Triangle &second) const
{
    double k1, k2, k3;
    k1 = A / second.A;
    k2 = B / second.B;
    k3 = C / second.C;
    if (fabs(k1-k2) < eps && fabs(k2-k3) < eps) return true;
    return false;
}
