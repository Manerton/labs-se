#ifndef TELO_H
#define TELO_H
#include <QVector>
#include <QPoint>

enum class direction {
    forward = 0,
    right = 1,
    down = 2,
    left = 3
};

class animal
{
    double v; // скорость крокодила в км/ч
    double l; // расстояние в км, пройденное им до поворота
    direction getRotation(); // получить поворот в зависимости от вероятности
    direction getStartDirection();
    direction global_direction;
    friend direction& operator+=(const direction &a, const direction &b);
public:
    animal(const double &_v, const double &_l) : v(_v), l(_l){}
    QVector<QPoint> coord; // координаты местоположений крокодила
    void work (int time, int k = 10);

};

#endif // TELO_H
