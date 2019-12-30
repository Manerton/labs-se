#include "vector3d.h"

vector3D &vector3D::operator+=(const vector3D &b)
{
    int64_t x2 = b.getNum1();
    int64_t y2 = b.getNum2();
    int64_t z2 = b.getNum3();
    add_to_num1(x2);
    add_to_num2(y2);
    add_to_num3(z2);
    return *this;
}

vector3D &vector3D::operator*=(const vector3D &b)
{
    int64_t x2 = b.getNum1();
    int64_t y2 = b.getNum2();
    int64_t z2 = b.getNum3();
    mul_num1(x2);
    mul_num2(y2);
    mul_num3(z2);
    return *this;
}

Triad vector3D::return_baseclass_obj()
{
    Triad t (*this);
    return t;
}

vector3D operator+(const vector3D &a, const vector3D &b)
{
    vector3D t = a;
    t += b;
    return t;
}

vector3D operator*(const vector3D &a, const vector3D &b)
{
    vector3D t = a;
    t *= b;
    return t;
}
