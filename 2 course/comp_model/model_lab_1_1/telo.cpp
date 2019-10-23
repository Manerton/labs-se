#include "telo.h"
#include <QString>
#include <cmath>

QString telo::getTime()
{
    Y.push_back(1);
    double time = 0;
    for (int i = 0; Y[i] >= 0; i++)
    {
        time += 0.01;
        Y.push_back( V0*time - ((g*time*time)/2) );
    }
    Y.push_back(1);
    if (fabs(V0 * V0 - 2*g*h) < 0.001)
    {
        double t = V0/g;
        QString tt = QString::number(t);
        return "Высота достигается 1 раз через " + tt + " секунд.";
    } else {
        if (V0*V0 < 2*g*h) return "Высота недостижима при такой скорости.";
        else {
            double t1, t2;
            t1 = (V0 - sqrt(V0*V0 - 2*g*h))/g;
            t2 = (V0 + sqrt(V0*V0 - 2*g*h))/g;
            QString tt1 = QString::number(t1);
            QString tt2 = QString::number(t2);
            return "Высота достигается 2 раза через " + tt1 + " и " + tt2 + " секунд.";
        }
    }
}

