#ifndef P_SEMICONDUCTOR_H
#define P_SEMICONDUCTOR_H

#include <QPropertyAnimation>
#include "particle.h"
#include <vector>

class p_semiconductor
{
public:
    p_semiconductor();
    ~p_semiconductor();
    // рисуем процесс движения электронов и дырки в p-полупроводнике
    void draw();
    // рисуем схему источника питания
    void drawPowerSource(); // функция для отрисовки источника тока
    // рисуем один атом по координате coords
    void drawAtom(QGraphicsItem* parent, const QPointF &coords);
    // рисуем все атомы по начальной координате coords
    void drawAtoms(QGraphicsItem* parent, const QPointF &coords);
    // рисуем электроны
    void drawElectrons(const QPointF &coords);
    // рисуем границы
    void drawBounds();
    // подготовка анимаций для электронов
    void prepareAnim(const QPointF &coords);
    // ручная подготовка первой и последней анимации
    // (поскольку электрон вылетает за границу к источнику тока)
    void prepareAnimFirstAndLast(const QPointF &coords);
    // сброс состояния анимации (электроны в исходные
    void resetAnimations();
    //
    void pause();
    void unpause();
// поля
    std::vector<Particle*> particleArr;
    std::vector<QPropertyAnimation*> animationArr;
    // объявляем группу для элементов элементов
    QGraphicsItemGroup *group = nullptr;
    // последовательная группа для анимаций
    QSequentialAnimationGroup* animation_group = nullptr;
    // константы для рисования
    const double w = 700;
    const double h = 150;
    const double halfH = h/2;
    const double betweenAtoms = 60;
    const double atomSize = 80;
    const double halfAtomSize = atomSize/2;
    const double particleSize = 16;
    const double halfParticleSize = particleSize/2;
    // скорость анимации
    int animDuration = 1000;
    int pauseDuration = 300;
    // активен и готов к демонстрации
    bool active = false;
    bool readyToShow = false;
};

#endif // P_SEMICONDUCTOR_H
