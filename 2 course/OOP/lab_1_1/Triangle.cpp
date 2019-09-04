#include <iostream>
#include <string>
#include <cmath>
#include "Triangle.h"

using namespace std;

bool Triangle::Check(const double &a_in, const double &b_in, const double &c_in)
{
    if (a_in > 0 && b_in > 0 && c_in)
    {
        if ( (a_in + b_in > c_in) && (a_in + c_in > b_in) && (b_in + c_in > a_in) )
            return true;
    }
    return false;
}

void Triangle::Display()
{
    string s = toString();
    cout << "����� ������������ - " << s << endl;
}

void Triangle::Read()
{
    double a_in, b_in, c_in;
    do {
    cout << "������� ����� ������� a: ";
    cin >> a_in;
    cout << "������� ����� ������� b: ";
    cin >> b_in;
    cout << "������� ����� ������� c: ";
    cin >> c_in;
    } while (!Check(a_in, b_in, c_in));
    Triangle::Init(a_in, b_in, c_in);
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

void Triangle::FindHeight()
{
    double Double_Square = FindSquare() * 2;
    double Height_a = Double_Square / A;
    double Height_b = Double_Square / B;
    double Height_c = Double_Square / C;
    cout << "������, �������� �� ��������� a: " << Height_a << endl;
    cout << "������, �������� �� ��������� b: " << Height_b << endl;
    cout << "������, �������� �� ��������� c: " << Height_c << endl;
}

void Triangle::FindAngle(double &Angle_ab, double &Angle_ac, double &Angle_bc)
{
    double Double_Square = FindSquare() * 2;
    double sin_Angle_ab = Double_Square / (A*B);
    double sin_Angle_ac = Double_Square / (A*C);
    double sin_Angle_bc = Double_Square / (B*C);
    Angle_ab = asin(sin_Angle_ab);
    Angle_ac = asin(sin_Angle_ac);
    Angle_bc = asin(sin_Angle_bc);
}

void Triangle::DisplayAngles(const double &Angle_ab, const double &Angle_ac, const double &Angle_bc)
{
    cout << "���� ����� a � b: " << Angle_ab;
    cout << "���� ����� a � c: " << Angle_ac;
    cout << "���� ����� b � c: " << Angle_bc;
}

void Triangle::TriangleKind()
{
    double ab, ac, bc;
    FindAngle(ab,ac,bc);
    if (ab > 90 || ac > 90 || bc > 90) cout << "����������� �������� ������������." << endl;
    if (ab < 90 && ac < 90 && bc < 90) cout << "����������� �������� �������������." << endl;
    if (ab == 90 || ac == 90 || bc == 90) cout << "����������� �������� �������������." << endl;
}

void Triangle::CompareTwoTriangles(const Triangle &second)
{
    double square_1 = FindSquare();
    double square_2 = second.FindSquare();
    if (square_1 > square_2) cout << "������� ������� ������������ ������ ������� �������." << endl;
    if (square_1 < square_2) cout << "������� ������� ������������ ������ ������� �������." << endl;
    if (square_1 == square_2) cout << "������� ������������� �����." << endl;
    double P1 = FindPerimeter();
    double P2 = second.FindPerimeter();
    if (P1 > P2) cout << "�������� ������� ������������ ������ ��������� �������." << endl;
    if (P1 < P2) cout << "�������� ������� ������������ ������ ��������� �������." << endl;
    if (P1 == P2) cout << "��������� ������������� �����." << endl;
}

void Triangle::Podobie(const Triangle &second)
{
    double k1, k2, k3;
    k1 = A / second.A;
    k2 = B / second.B;
    k3 = C / second.C;
    if (k1 == k2 && k2 == k3) cout << "������������ ��������" << endl;
    else cout << "������������ ����������" << endl;
}
