#ifndef TELO_H
#define TELO_H
#include <QVector>

class telo
{
    double V0; // начальная скорость
    double h; // высота
    constexpr static double g = 9.806;
public:
    QVector<double> Y;
    telo(const double &inV0, const double &inh) : V0(inV0),  h(inh){ }
    QString getTime ();
    void work();
};

#endif // TELO_H
