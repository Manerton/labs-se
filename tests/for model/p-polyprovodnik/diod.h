#ifndef DIOD_H
#define DIOD_H

#include <QPropertyAnimation>
#include <vector>
#include "particle.h"


class diod : public QObject
{
public:
    diod();
    ~diod();
    // рисуем процесс движения электронов и дырки в p-полупроводнике
    void draw();
    // рисуем схему источника питания
    void drawPowerSourceOn();  // функция для отрисовки источника тока (прямое напряжение)
    void drawPowerSourceOff(); // функция для отрисовки источника тока (обратное напряжение)
    // рисуем электроны и дырки
    void drawParticles();
    void drawIdleParticles();
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
    QGraphicsItemGroup* movingParticles = nullptr;
    QGraphicsItemGroup* idleParticles = nullptr;
    QGraphicsItemGroup* powerSourceOn = nullptr;
    QGraphicsItemGroup* powerSourceOff = nullptr;
    int animDuration = 2500;
    int i = 0;
    bool active = false;
    bool readyToShow = false;
public slots:
    // это для анимации потока электронов
    void resumeAnim();
    void changePolarity();
};

#endif // DIOD_H
