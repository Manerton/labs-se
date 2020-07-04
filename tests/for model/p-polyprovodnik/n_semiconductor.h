#ifndef N_SEMICONDUCTOR_H
#define N_SEMICONDUCTOR_H

#include <QPropertyAnimation>
#include <vector>
#include "particle.h"


class n_semiconductor : public QObject
{
public:
    n_semiconductor();
    ~n_semiconductor();
    // рисуем процесс движения электронов и дырки в p-полупроводнике
    void draw();
    // рисуем схему источника питания
    void drawPowerSource(); // функция для отрисовки источника тока
    // рисуем пятивалентные атомы-ионы
    void drawIon(const QPointF &coords);
    void drawIons();
    // рисуем электроны
    void drawElectrons();
    // рисуем границы
    void drawBounds();
    // подготовка анимаций для электронов
    void prepareAnim();
    // поставить на паузу и снять с паузы визуализацию n-полупроводника
    void pause();
    void unpause();
// поля
    std::vector<Particle*> particleArr;
    std::vector<QPropertyAnimation*> animationArr;
    // объявляем группу для элементов элементов
    QGraphicsItemGroup *group = nullptr;
    // константы для рисования
    const double w = 700;
    const double h = 150;
    const double halfH = h/2;
    const double particleSize = 16;
    const double halfParticleSize = particleSize/2;
    // скорость анимации
    QTimer *animTimer = nullptr;
    int animDuration = 2500;
    int i = 0;
    bool active = false;
    bool readyToShow = false;
public slots:
    // это для анимации потока электронов
    void resumeAnim();
};

#endif // N_SEMICONDUCTOR_H
