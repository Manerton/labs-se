#include "telo.h"
#include <QString>
#include <limits>
#include <cmath>

QString telo::getTime()
{
    // начинаем полет с земли (координата Y = 1)
    Y.push_back(1);
    double time = 0;
    const double time_inc = 0.01;
    while (Y.back() >= 0)
    {
        time += time_inc;
        Y.push_back( V0*time - ((g*time*time)/2) );
    }

    // чтобы в конце пути точка как бы "лежала" на земле
    Y.push_back(1);
    const double eps = std::numeric_limits<double>::epsilon();
    if (fabs(V0 * V0 - 2*g*h) < eps)
    {
        double t = V0/g;
        QString tt = QString::number(t);
        return "Высота достигается 1 раз через " + tt + " секунд.";
    }

    if (V0*V0 < 2*g*h)
    {
        return "Высота недостижима при такой скорости.";
    }

    const double t1 = (V0 - sqrt(V0*V0 - 2*g*h))/g;
    const double t2 = (V0 + sqrt(V0*V0 - 2*g*h))/g;
    const QString tt1 = QString::number(t1);
    const QString tt2 = QString::number(t2);
    return "Высота достигается 2 раза через " + tt1 + " и " + tt2 + " секунд.";

}

