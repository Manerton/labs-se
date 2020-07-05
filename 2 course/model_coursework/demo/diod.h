#ifndef DIOD_H
#define DIOD_H

#include <QPropertyAnimation>
#include <vector>
#include "particle.h"

// демонстрация p-n перехода
class Diod : public QObject
{
public:
    Diod();
    ~Diod();
    // рисуем процесс p-n перехода
    void draw();
    // рисуем схему источника питания
    void drawPowerSourceOn();  // функция для отрисовки источника тока (прямое напряжение)
    void drawPowerSourceOff(); // функция для отрисовки источника тока (обратное напряжение)
    // рисуем движущиеся электроны и дырки
    void drawParticles();
    // рисуем статичные (при обратном напряжении) частицы
    void drawIdleParticles();
    // рисуем границы (зоны, в которые уходят частицы и исчезают)
    void drawBounds();
    // подготовка анимаций для электронов
    void prepareAnim();
    // поставить на паузу и снять с паузы визуализацию диода
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
    // группа статичных частиц
    QGraphicsItemGroup* idleParticles = nullptr;
    // группа элементов для источника тока в прямом подключении
    QGraphicsItemGroup* powerSourceOn = nullptr;
    // группа элементов для источника тока в обратном подключении
    QGraphicsItemGroup* powerSourceOff = nullptr;
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
    // для смены полярности подключаемого источника тока
    void changePolarity();
};

#endif // DIOD_H
