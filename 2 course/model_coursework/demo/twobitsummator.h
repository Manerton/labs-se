#ifndef TWOBITSUMMATOR_H
#define TWOBITSUMMATOR_H
#include "halfbitsummator.h"

// берем основное от HalfBitSummator и расширяем
class TwoBitSummator : public HalfBitSummator
{
public:

    enum dataSymbols : uint8_t
    {
        a = 0,
        b,
        c,
        d,
        e,
        f,
        g,
        sum1_u,
        sum1_v,
        sum2_u,
        sum2_v,
        sum3_u,
        sum3_v
    };

    TwoBitSummator();

    virtual void draw() override;
    virtual void PrepareInputs() override;
    void calculateAndDraw(bool a, bool b, bool c, bool d);
    void drawBox(const QPointF& coords);
    // координаты для вывода входных и выходных данных
    // входные a, b, c, d
    QPointF *point_a;
    QPointF *point_b;
    QPointF *point_c;
    QPointF *point_d;
    // выходные e, f, g
    QPointF *point_e;
    QPointF *point_f;
    QPointF *point_g;
    // центр полубитного сумматора 1 и 3
    QPointF *point_sum1_centre;
    QPointF *point_sum3_centre;
    // чтобы цифра нарисовалась нормально, нужно сместить ее левее и выше
    QPointF normalize_coords_for_symbol(const QPointF &point);
};

#endif // TWOBITSUMMATOR_H
