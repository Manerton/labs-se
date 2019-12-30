#ifndef VECTOR3D_H
#define VECTOR3D_H
#include "triad.h"


class vector3D : public Triad
{
public:    
    // -- конструктор от чисел-координат -- //
    vector3D(int64_t x = 0, int64_t y = 0, int64_t z = 0) noexcept : Triad(x,y,z) {}
    // -- конструктор принимающий объект базового класса -- //
    vector3D(const Triad& b) noexcept : Triad(b) {}
    // -- операции-методы присваивания -- //
    vector3D& operator+=(const vector3D &b);  // -- сложение векторов -- //
    vector3D& operator*=(const vector3D &b);  // -- скалярное произведение векторов -- //
    // -- функция, возвращающая объект базового класса -- //
    Triad return_baseclass_obj();
    // -- дружеские операции -- //
    friend vector3D operator+(const vector3D &a, const vector3D &b);
    friend vector3D operator*(const vector3D &a, const vector3D &b);
};

#endif // VECTOR3D_H
