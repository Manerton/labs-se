#include "twobitsummator.h"
#include "picturebox.h"
#include <QDebug>

TwoBitSummator::TwoBitSummator()
{
    // тут больше чисел нужно выводить на экран
    textItems_array.resize(13);
    // входные
    point_a = new QPointF();
    point_b = new QPointF();
    point_c = new QPointF();
    point_d = new QPointF();
    // выходные
    point_e = new QPointF();
    point_f = new QPointF();
    point_g = new QPointF();
    // промежуточные
    point_sum1_centre = new QPointF();
    point_sum3_centre = new QPointF();
}

void TwoBitSummator::draw()
{
    QPainterPath* path = new QPainterPath();
    QPen pen = {Qt::black};
    pen.setWidth(3);
    double x = -30, y = -100;
    // стартовая точка
    *point_a = {x,y};
    path->moveTo(x,y);
    x += 100;
    path->lineTo(x,y);
    // рисуем полубитный сумматор 1
    drawBox({x,y+15});
    *point_sum1_centre = {x+80,y-14};
    x += 160;
    path->moveTo(x,y);
    x+= 373;
    path->lineTo(x,y);
    x = point_a->x();
    y = point_a->y()+250;
    path->moveTo(x,y);
    // запоминаем точку для входного значения 'd'
    *point_d = {x,y};
    x += 100;
    path->lineTo(x,y);
    // рисуем полубитный сумматор 2
    drawBox({x,y-15});
    x += 160;
    path->moveTo(x,y);
    x += 525;
    path->lineTo(x,y);
    // точка для выходного значения 'g'
    *point_g = {x+30,y};
    x = point_a->x();
    y = point_a->y()+40;
    path->moveTo(x,y);
    // точка для входного значения 'b'
    *point_b = {x,y};
    x+=30;
    path->lineTo(x,y);
    x+=30;
    y+=175;
    path->lineTo(x,y);
    x+=40;
    path->lineTo(x,y);
    x = point_b->x();
    y = point_b->y() + 175;
    // точка для входного значения 'c'
    *point_c = {x,y};
    path->moveTo(x,y);
    x+=30;
    path->lineTo(x,y);
    x+=30;
    y-=175;
    path->lineTo(x,y);
    x+=40;
    path->lineTo(x,y);
    x+=160;
    path->moveTo(x,y);
    x+=50;
    path->lineTo(x,y);
    y+=60;
    path->lineTo(x,y);
    x+=50;
    path->lineTo(x,y);
    // рисуем полубитный сумматор 3
    drawBox({x,y+15});
    *point_sum3_centre = {x+80,y-45};
    x+=160;
    path->moveTo(x,y);
    x+=50;
    path->lineTo(x,y);
    y-=60;
    path->lineTo(x,y);
    x+=60;
    path->lineTo(x,y);
    // рисуем операцию ИЛИ у буквы 'e'
    drawOr({x-8,y-20});
    x+=103;
    y-=20;
    path->moveTo(x,y);
    path->lineTo(x+50,y);
    // запоминаем точку для выходного значения 'e'
    *point_e = {x+80,y};
    x = point_c->x()+260;
    y = point_c->y();
    path->moveTo(x,y);
    x+= 50;
    path->lineTo(x,y);
    y-=80;
    path->lineTo(x,y);
    x+=50;
    path->lineTo(x,y);
    x+=160;
    path->moveTo(x,y);
    x+=263;
    path->lineTo(x,y);
    // точка для выходного значения 'f'
    *point_f = {x+31,y};
    QGraphicsPathItem* mainLine = new QGraphicsPathItem(*path);
    mainLine->setPen(pen);
    group->addToGroup(mainLine);

    PrepareInputs();

    group->moveBy(-10,0);
}

void TwoBitSummator::PrepareInputs()
{
    QPointF sum1_u = *point_sum1_centre;
    sum1_u.rx() -= 30;
    QPointF sum1_v = {sum1_u.x() + 30, sum1_u.y()};
    QPointF sum2_u = {sum1_u.x(), sum1_u.y()+225};
    QPointF sum2_v = {sum2_u.x() + 30, sum2_u.y()};
    QPointF sum3_u = *point_sum3_centre;
    sum3_u.rx() -= 30;
    sum3_u.ry() += 35;
    QPointF sum3_v = {sum3_u.x() + 30, sum3_u.y()};
    textItems_array[dataSymbols::a] = PictureBox::drawSign('0',normalize_coords_for_symbol(*point_a),group);     // a
    textItems_array[dataSymbols::b] = PictureBox::drawSign('0',normalize_coords_for_symbol(*point_b),group);     // b
    textItems_array[dataSymbols::c] = PictureBox::drawSign('0',normalize_coords_for_symbol(*point_c),group);     // c
    textItems_array[dataSymbols::d] = PictureBox::drawSign('0',normalize_coords_for_symbol(*point_d),group);     // d
    textItems_array[dataSymbols::e] = PictureBox::drawSign('0',normalize_coords_for_symbol(*point_e),group);     // e
    textItems_array[dataSymbols::f] = PictureBox::drawSign('0',normalize_coords_for_symbol(*point_f),group);     // f
    textItems_array[dataSymbols::g] = PictureBox::drawSign('0',normalize_coords_for_symbol(*point_g),group);     // f
    textItems_array[dataSymbols::sum1_u] = PictureBox::drawSign('0',sum1_u,group);                               // sum1_u
    textItems_array[dataSymbols::sum1_v] = PictureBox::drawSign('0',sum1_v,group);                               // sum1_v
    textItems_array[dataSymbols::sum2_u] = PictureBox::drawSign('0',sum2_u,group);                               // sum2_u
    textItems_array[dataSymbols::sum2_v] = PictureBox::drawSign('0',sum2_v,group);                               // sum2_v
    textItems_array[dataSymbols::sum3_u] = PictureBox::drawSign('0',sum3_u,group);                               // sum3_u
    textItems_array[dataSymbols::sum3_v] = PictureBox::drawSign('0',sum3_v,group);                               // sum3_v
}

void TwoBitSummator::calculateAndDraw(bool a, bool b, bool c, bool d)
{
    textItems_array[dataSymbols::a]->setPlainText({boolToChar(a)});
    textItems_array[dataSymbols::b]->setPlainText({boolToChar(b)});
    textItems_array[dataSymbols::c]->setPlainText({boolToChar(c)});
    textItems_array[dataSymbols::d]->setPlainText({boolToChar(d)});

    SummatorLogic::TwoBitRes res = logic.calculateTwoBit(a,b,c,d);
    textItems_array[dataSymbols::sum1_u]->setPlainText({boolToChar(res.sum1.first)});
    textItems_array[dataSymbols::sum1_v]->setPlainText({boolToChar(res.sum1.second)});
    textItems_array[dataSymbols::sum2_u]->setPlainText({boolToChar(res.sum2.first)});
    textItems_array[dataSymbols::sum2_v]->setPlainText({boolToChar(res.sum2.second)});
    textItems_array[dataSymbols::sum3_u]->setPlainText({boolToChar(res.sum3.first)});
    textItems_array[dataSymbols::sum3_v]->setPlainText({boolToChar(res.sum3.second)});
    textItems_array[dataSymbols::e]->setPlainText({boolToChar(res.e)});
    textItems_array[dataSymbols::f]->setPlainText({boolToChar(res.f)});
    textItems_array[dataSymbols::g]->setPlainText({boolToChar(res.g)});
}

void TwoBitSummator::drawBox(const QPointF &coords)
{
    QGraphicsRectItem* rect = new QGraphicsRectItem(coords.x(),coords.y()-45,160,90);
    QPen pen = {Qt::black};
    pen.setWidth(3);
    rect->setPen(pen);
    group->addToGroup(rect);
}

QPointF TwoBitSummator::normalize_coords_for_symbol(const QPointF &point)
{
    QPointF p = point;
    p.rx() -= 28;
    p.ry() -= 24;
    return p;
}

