#ifndef HALFBITSUMMATOR_H
#define HALFBITSUMMATOR_H

#include <vector>
#include <QGraphicsItem>
#include "summatorlogic.h"

class HalfBitSummator : public QObject
{
public:
    explicit HalfBitSummator();
    virtual ~HalfBitSummator();
    // рисуем схему полубитного сумматора
    virtual void draw();
    // рисуем операцию "НЕ"
    void drawInversion(const QPointF &coords);
    // рисуем операцию И
    void drawAnd(const QPointF &coords);
    // операция ИЛИ
    void drawOr(const QPointF &coords);
    // рисуем входные и выходные данные сумматора
    virtual void PrepareInputs();
    // вычисляем и рисуем (вычисляем через logic)
    void calculateAndDraw(bool x, bool y);
    // перевод bool в символ "1" если true, и "0" если false
    char boolToChar(bool x);
    // массив с указателями на текстовые айтемы входных и выходных данных (при отрисовке)
    std::vector<QGraphicsTextItem*> textItems_array;
// поля
    // функциональная часть сумматора
    SummatorLogic logic;
    // объявляем группу для элементов
    QGraphicsItemGroup *group = nullptr;
};

#endif // HALFBITSUMMATOR_H
