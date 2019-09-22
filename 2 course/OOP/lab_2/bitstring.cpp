#include "bitstring.h"



bool BitString::Check(const long long f1_in, const long long f2_in) const // проверка числа, чтобы оно было двоичным и неотрицательным
{
    if (f1_in > 0 && f2_in > 0)
    {
        if ()
            return true;
        else throw "сумма длин каждых двух сторон должна быть больше длины третьей стороны.";
    } else {
        throw "у треугольника не может быть отрицательных сторон.";
    }
}

