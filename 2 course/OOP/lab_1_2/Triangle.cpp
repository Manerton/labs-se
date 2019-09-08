#include <iostream>
#include <string>
#include <cmath>
#include "Triangle.h"

using namespace std;

bool Triangle::Check(const double &a_in, const double &b_in, const double &c_in) const  // �������� �� �����������
{
    if (a_in > 0 && b_in > 0 && c_in > 0)
    {
        if ( (a_in + b_in > c_in) && (a_in + c_in > b_in) && (b_in + c_in > a_in) )
            return true;
        else throw "����� ���� ������ ���� ������ ������ ���� ������ ����� ������� �������.";
    } else {
        throw "� ������������ �� ����� ���� ������������� ������.";
    }
}

void Triangle::Display() const
{
    string s = toString();
    cout << "����� ������������: " << s << endl;
}

void Triangle::Read() // Read
{

    double a, b, c;
    cout << "������� ����� ������� a: ";
    cin >> a;
    cout << "������� ����� ������� b: ";
    cin >> b;
    cout << "������� ����� ������� c: ";
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
    cout << "������, �������� �� ��������� a: " << Height_a << endl;
    cout << "������, �������� �� ��������� b: " << Height_b << endl;
    cout << "������, �������� �� ��������� c: " << Height_c << endl;
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
    cout << "���� ����� a � b: " << ab << endl;
    cout << "���� ����� a � c: " << ac << endl;
    cout << "���� ����� b � c: " << bc << endl;
}

void Triangle::TriangleKind() const
{
    double ab, ac, bc;
    FindAngle(ab,ac,bc);
    if (ab > 90 || ac > 90 || bc > 90) cout << "����������� �������� ������������." << endl;
    if (ab < 90 && ac < 90 && bc < 90) cout << "����������� �������� �������������." << endl;
    if (ab == 90 || ac == 90 || bc == 90) cout << "����������� �������� �������������." << endl;
}

void Triangle::CompareTwoTriangles(const Triangle &second) const
{
    double eps = 0.0001;
    double square_1 = FindSquare();
    double square_2 = second.FindSquare();
    if (fabs(square_1 - square_2) < eps) cout << "������� ������������� �����." << endl;
    else if (square_1 > square_2) cout << "������� ������� ������������ ������ ������� �������." << endl;
    else if (square_1 < square_2) cout << "������� ������� ������������ ������ ������� �������." << endl;
    double P1 = FindPerimeter();
    double P2 = second.FindPerimeter();
    if (fabs(P1 - P2) < eps) cout << "��������� ������������� �����." << endl;
    else if (P1 > P2) cout << "�������� ������� ������������ ������ ��������� �������." << endl;
    else if (P1 < P2) cout << "�������� ������� ������������ ������ ��������� �������." << endl;
}

void Triangle::Podobie(const Triangle &second) const
{
    double k1, k2, k3;
    k1 = A / second.A;
    k2 = B / second.B;
    k3 = C / second.C;
    if (k1 == k2 && k2 == k3) cout << "������������ ��������, ����������� ������� k = " << k1 << endl;
    else cout << "������������ ����������" << endl;
}
