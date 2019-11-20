#ifndef TELO_H
#define TELO_H
#include <QVector>
#include <QPoint>

enum class direction {
    left,
    right,
    forward
};

class animal
{
    double v; // скорость крокодила в км/ч
    double l; // расстояние в км, пройденное им до поворота
    direction getDirection(); // получить поворот в зависимости от вероятности
    direction prev_direction; // в каком направлении мы шли до этого шага
public:
    animal(const double &_v, const double &_l) : v(_v), l(_l){}
    QVector<QPoint> coord; // координаты местоположений крокодила
    void work (int time, int k = 10);

};

#endif // TELO_H
