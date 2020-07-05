#ifndef N_SEMICONDUCTOR_H
#define N_SEMICONDUCTOR_H

#include <QPropertyAnimation>
#include <vector>
#include "particle.h"

// визуализируем n-полупроводник
class N_semiconductor : public QObject
{
public:
    N_semiconductor();
    ~N_semiconductor();
    // рисуем процесс движения электронов в n-полупроводнике
    void draw();
    // рисуем схему источника питания
    void drawPowerSource();
    // для отрисовки одного пятивалентного иона по координате coords
    void drawIon(const QPointF &coords);
    // рисуем пятивалентные атомы-ионы
    void drawIons();
    // рисуем электроны
    void drawElectrons();
    // рисуем границы
    void drawBounds();
    // подготовка анимаций для электронов
    void prepareAnim();
    // поставить на паузу и снять с паузы визуализацию n-полупроводника
    // при переходе к другой визуализации
    void pause();
    void unpause();
// поля
    // массивы частиц и анимаций к ним
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
    // таймер, выпускающий частицы для эффекта потока частиц
    QTimer *animTimer = nullptr;
    // длительность анимации
    int animDuration = 2500;
    // индекс текущей выпускаемой частицы
    int i = 0;
     // активна ли сейчас анимация?
    bool active = false;
    // сформировались ли все элементы сцены и анимации?
    bool readyToShow = false;
public slots:
    // это для анимации потока электронов
    void resumeAnim();
};

#endif // N_SEMICONDUCTOR_H
