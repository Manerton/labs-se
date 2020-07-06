#ifndef TRANSISTOR_H
#define TRANSISTOR_H


#include <QPropertyAnimation>
#include <vector>
#include "particle.h"

// демонстрация p-n перехода
class Transistor : public QObject
{
public:
    Transistor();
    ~Transistor();
    // рисуем процесс движения электронов в транзисторе n-p-n
    void draw();
    // рисуем схему источника питания
    void drawPowerSource();     // функция для отрисовки источника тока
    // рисуем движущиеся электроны и дырки
    void drawParticles();
    // рисуем границы (зоны, в которые уходят частицы и исчезают)
    void drawBounds();
    // подготовка анимаций для электронов
    void prepareAnim();
    // нарисовать стрелку
    void drawArrow(const QPointF &coords);
    // поставить на паузу и снять с паузы визуализацию транзистора
    // при переключении на другие демонстрации
    void pause();
    void unpause();
// поля
    // массивы с частицами
    std::vector<Particle*> particleArr;
    // массивы, где хранятся анимации для частиц
    std::vector<QPropertyAnimation*> animationArr;
    // объявляем общую группу для элементов
    QGraphicsItemGroup *group = nullptr;
    // константы для рисования
    const double w = 700;
    const double h = 150;
    const double halfH = h/2;
    const double particleSize = 16;
    const double halfParticleSize = particleSize/2;
    // таймер, выпускающий частицы для эффекта потока частиц
    QTimer *animTimer = nullptr;
    // группа движущихся частиц
    QGraphicsItemGroup* movingParticles = nullptr;
    // группа элементов для источника тока
    QGraphicsItemGroup* powerSource = nullptr;
    // длительность анимации
    int animDuration = 3000;
    // индекс текущей выпускаемой частицы
    int i = 0;
    // активна ли сейчас анимация?
    bool active = false;
    // сформировались ли все элементы сцены и анимации?
    bool readyToShow = false;
public slots:
    // это для анимации потока электронов
    void resumeAnim();
    void doSomething(int i);
};

#endif // TRANSISTOR_H
