#include "telo.h"
#include <QString>
#include <cmath>

double telo::getAngle(const double &gradus)
{
    double PI = acos(-1.0);
    return (gradus * PI)/180;
}

QString telo::work(double a)
{
    a = getAngle(a);
    double cosA = cos(a);
    double doubleV0 = (g*l*l) / (2*(l*tan(a)-h)*cosA*cosA);
    if (doubleV0 < 0) return "под таким углом мяч не кинуть.";
    int i = 0;
    X.push_back(0);
    Y.push_back(0);
    do{
        i++;
        X.push_back( X[i-1] + 0.01 );

        Y.push_back( X[i] * tan(a) - ((g * (X[i]) * (X[i])) / (2 * doubleV0 * cosA * cosA)) );
    } while (X[i] <= l);
    return QString::number(sqrt(doubleV0));
}


