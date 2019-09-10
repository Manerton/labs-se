#include <iostream>
#include <string>
#include <cmath>
#include "Triangle.h"

using namespace std;

bool Triangle::Check(const double &a_in, const double &b_in, const double &c_in) const  // проверка на треугольник
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

void Triangle::Display() const
{
    string s = toString();
    cout << "Длины треугольника: " << s << endl;
}

void Triangle::Read() // Read
{

    double a, b, c;
    cout << "Введите длину стороны a: ";
    cin >> a;
    cout << "Введите длину стороны b: ";
    cin >> b;
    cout << "Введите длину стороны c: ";
    cin >> c;

    *this = Triangle(a,b,c);
}

double Triangle::FindPerimeter() const
{
    double result = A + B + C;
    return result;
}

double Triangle::FindSquare() const
{
    double p = FindPerimeter() / 2;
    double result_wo_sqrt = p * (p-A) * (p-B) * (p-C);
    double result = sqrt(result_wo_sqrt);
    return result;
}

void Triangle::FindHeight() const
{
    double Double_Square = FindSquare() * 2;
    double Height_a = Double_Square / A;
    double Height_b = Double_Square / B;
    double Height_c = Double_Square / C;
    cout << "Высота, падающая на основание a: " << Height_a << endl;
    cout << "Высота, падающая на основание b: " << Height_b << endl;
    cout << "Высота, падающая на основание c: " << Height_c << endl;
}

void Triangle::FindAngle(double &Angle_ab, double &Angle_ac, double &Angle_bc) const
{
    double cos_Angle_ab = ((A*A)+(B*B)-(C*C)) / (2*A*B);
    double cos_Angle_ac = ((A*A)+(C*C)-(B*B)) / (2*A*C);
    double cos_Angle_bc = ((B*B)+(C*C)-(A*A)) / (2*B*C);
    Angle_ab = acos(cos_Angle_ab) * 180 / M_PI;
    Angle_ac = acos(cos_Angle_ac) * 180 / M_PI;
    Angle_bc = acos(cos_Angle_bc) * 180 / M_PI;
}

void Triangle::DisplayAngles() const
{
    double ab, ac, bc;
    FindAngle(ab,ac,bc);
    cout << "Угол между a и b: " << ab << endl;
    cout << "Угол между a и c: " << ac << endl;
    cout << "Угол между b и c: " << bc << endl;
}

void Triangle::TriangleKind() const
{
    double ab, ac, bc;
    FindAngle(ab,ac,bc);
    if (ab > 90 || ac > 90 || bc > 90) cout << "Треугольник является тупоугольным." << endl;
    if (ab < 90 && ac < 90 && bc < 90) cout << "Треугольник является остроугольным." << endl;
    if (ab == 90 || ac == 90 || bc == 90) cout << "Треугольник является прямоугольным." << endl;
}

void Triangle::CompareTwoTriangles(const Triangle &second) const
{
    double eps = 0.0001;
    double square_1 = FindSquare();
    double square_2 = second.FindSquare();
    if (fabs(square_1 - square_2) < eps) cout << "Площади треугольников равны." << endl;
    else if (square_1 > square_2) cout << "Площадь первого треугольника больше площади второго." << endl;
    else if (square_1 < square_2) cout << "Площадь первого треугольника меньше площади второго." << endl;
    double P1 = FindPerimeter();
    double P2 = second.FindPerimeter();
    if (fabs(P1 - P2) < eps) cout << "Периметры треугольников равны." << endl;
    else if (P1 > P2) cout << "Периметр первого треугольника больше периметра второго." << endl;
    else if (P1 < P2) cout << "Периметр первого треугольника меньше периметра второго." << endl;
}

void Triangle::Podobie(const Triangle &second) const
{
    double k1, k2, k3;
    k1 = A / second.A;
    k2 = B / second.B;
    k3 = C / second.C;
    if (k1 == k2 && k2 == k3) cout << "Треугольники подобные, коэффициент подобия k = " << k1 << endl;
    else cout << "Треугольники неподобные" << endl;
}
