#ifndef TELO_H
#define TELO_H
#include <QVector>

class telo
{
    double l; // расстояния до кольца
    double h; // высота до кольца
    constexpr static double g = 9.806;
public:
    QVector<double> X;
    QVector<double> Y;
    telo(const double &inl, const double &inh) : l(inl), h(inh){}
    double getAngle (const double &gradus);
    QString work (double a);
};

#endif // TELO_H
