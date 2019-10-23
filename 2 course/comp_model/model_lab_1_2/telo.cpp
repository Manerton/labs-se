#include "telo.h"
#include <cmath>

double telo::getAngle(const double &gradus)
{
    double PI = acos(-1.0);
    return (gradus * PI)/180;
}

void telo::work()
{
    int i = 0;
    X.push_back(0);
    Y.push_back(0);
    while (Y[i] >= 0)
    {
        i++;
        X.push_back( X[i-1] + 0.01 );
        double cosA = cos(a);
        Y.push_back( X[i] * tan(a) - ((g * (X[i]) * (X[i])) / (2 * V0 * V0 * cosA * cosA)) );
    }
}
