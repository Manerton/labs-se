#ifndef HALFBITSUMMATOR_H
#define HALFBITSUMMATOR_H


#include <QPropertyAnimation>
#include <vector>
#include "particle.h"


class HalfBitSummator : public QObject
{
public:
    HalfBitSummator();
    ~HalfBitSummator();
    // рисуем схему полубитного сумматора
    void draw();
    // рисуем операцию "НЕ"
    void drawInversion(const QPointF &coords);
    // рисуем операцию И и ИЛИ
    void drawAnd(const QPointF &coords);
    void drawOr(const QPointF &coords);
    // рисуем входные и выходные данные
    void PrepareInputs();
    // вычисляем и рисуем
    void calculateAndDraw(bool x, bool y);
    char boolToChar(bool x);
    // массив с указателями на айтемы входных и выходных данных (при отрисовке)
    std::vector<QGraphicsTextItem*> textItems_array;
// поля
    // объявляем группу для элементов элементов
    QGraphicsItemGroup *group = nullptr;
    // константы для рисования
    const double w = 700;
    const double h = 150;
    const double halfH = h/2;
    const double particleSize = 16;
    const double halfParticleSize = particleSize/2;
};

#endif // HALFBITSUMMATOR_H
