#ifndef TELO_H
#define TELO_H
#include <QVector>

class telo
{
    double V0; // начальная скорость
    double a; // угол в градусах
    constexpr static double g = 9.806;
public:
    QVector<double> X;
    QVector<double> Y;
    telo(const double &inV0, const double &ina) : V0(inV0)
    {
        a = getAngle(ina);
    }
    double getAngle (const double &gradus);
    void work ();
};

#endif // TELO_H
