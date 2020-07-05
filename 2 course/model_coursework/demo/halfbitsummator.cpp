#include "halfbitsummator.h"
#include "demo.h"
#include <QPen>
#include <QDebug>
#include "picturebox.h"

HalfBitSummator::HalfBitSummator() : textItems_array(6)
{
    group = new QGraphicsItemGroup();
    qDebug() << "halfbit c";
}

HalfBitSummator::~HalfBitSummator()
{
    delete group;
}

void HalfBitSummator::draw()
{
    QPainterPath* path = new QPainterPath();
    QPen pen = {Qt::black};
    pen.setWidth(3);
    double x = -30, y = -100;
    path->moveTo(x,y);
    x += 80;
    path->lineTo(x,y);
    drawInversion({x,y});
    x += 77;
    path->moveTo(x,y);
    x += 30;
    path->lineTo(x,y);
    drawAnd({x,y+15});
    x += 100;
    y += 13;
    path->moveTo(x,y);
    x += 100;
    path->lineTo(x,y);
    y += 30;
    path->lineTo(x,y);
    x += 40;
    path->lineTo(x,y);
    drawOr({-10+x,y+5});
    x += 101;
    y += 6;
    path->moveTo(x,y);
    path->lineTo(x+100,y);
    x = -30, y = -65;
    path->moveTo(x,y);
    x += 186;
    path->lineTo(x,y);
    x = 0;
    path->moveTo(x,y);
    y += 240;
    path->lineTo(x,y);
    path->moveTo(30,-100);
    path->lineTo(30,y-35);
    path->lineTo(156,y-35);
    path->moveTo(x,y);
    x += 156;
    path->lineTo(x,y);
    drawAnd({x,y-15});
    x += 100;
    y -= 13;
    path->moveTo(x,y);
    path->lineTo(x+345,y);
    x = 0, y = 55;
    path->moveTo(x,y);
    x += 50;
    path->lineTo(x,y);
    drawInversion({x,y});
    x += 77;
    path->moveTo(x,y);
    x += 30;
    path->lineTo(x,y);
    drawAnd({x,y-15});
    x += 100;
    y -= 13;
    path->moveTo(x,y);
    x+=100;
    path->lineTo(x,y);
    y -= 80;
    path->lineTo(x,y);
    x += 40;
    path->lineTo(x,y);
    path->moveTo(0,20);
    path->lineTo(156,20);
    QGraphicsPathItem* mainLine = new QGraphicsPathItem(*path);
    mainLine->setPen(pen);
    group->addToGroup(mainLine);
    PictureBox::drawSign('u',{605,135},group);
    PictureBox::drawSign('v',{600,-80},group);
    PrepareInputs();
    group->moveBy(70,0);
}

void HalfBitSummator::drawInversion(const QPointF &coords)
{
    QPainterPath* path = new QPainterPath();
    QPen pen = {Qt::black};
    pen.setWidth(3);
    path->moveTo(coords.x(),coords.y()+30);
    path->lineTo(coords.x(),coords.y()-30);
    path->lineTo(coords.x()+60,coords.y());
    path->lineTo(coords.x(),coords.y()+30);
    QGraphicsPathItem* mainLine = new QGraphicsPathItem(*path);
    mainLine->setPen(pen);
    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem({coords.x()+60,coords.y()-8,16,16},mainLine);
    circle->setPen(pen);
    group->addToGroup(mainLine);
}

void HalfBitSummator::drawAnd(const QPointF &coords)
{
    QPainterPath* path = new QPainterPath();
    QPen pen = {Qt::black};
    pen.setWidth(3);
    path->moveTo(coords.x(),coords.y()+40);
    path->lineTo(coords.x(),coords.y()-40);
    path->lineTo(coords.x()+60,coords.y()-40);
    path->moveTo(coords.x(),coords.y()+40);
    path->lineTo(coords.x()+60,coords.y()+40);
    QGraphicsPathItem* mainLine = new QGraphicsPathItem(*path);
    mainLine->setPen(pen);
    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem({coords.x()+20,coords.y()-40,80,80},mainLine);
    circle->setSpanAngle(2880);
    circle->setStartAngle(-1440);
    circle->setPen(pen);
    QGraphicsRectItem* emptyBox = new QGraphicsRectItem({coords.x()+50,coords.y()-38.45,15,76.8},mainLine);
    emptyBox->setBrush(Qt::white);
    emptyBox->setPen(QPen(Qt::NoPen));
    group->addToGroup(mainLine);
}

void HalfBitSummator::drawOr(const QPointF &coords)
{
    QPainterPath* path = new QPainterPath();
    QPen pen = {Qt::black};
    pen.setWidth(3);
    path->moveTo(coords.x(),coords.y()+40);
    path->quadTo({coords.x()+30,coords.y()},{coords.x(),coords.y()-40});
    path->lineTo(coords.x()+50,coords.y()-40);
    path->moveTo(coords.x(),coords.y()+41);
    path->lineTo(coords.x()+50,coords.y()+41);
    path->quadTo({coords.x()+80,coords.y()+42},{coords.x()+110,coords.y()});
    path->quadTo({coords.x()+80,coords.y()-42},{coords.x()+50,coords.y()-40});
    QGraphicsPathItem* mainLine = new QGraphicsPathItem(*path);
    mainLine->setPen(pen);
    group->addToGroup(mainLine);
}

void HalfBitSummator::PrepareInputs()
{
    textItems_array[0] = PictureBox::drawSign('0',{-70,-130},group); // x
    textItems_array[1] = PictureBox::drawSign('0',{-70,-90},group);  // y
    textItems_array[2] = PictureBox::drawSign('0',{188,-111},group); // 1
    textItems_array[3] = PictureBox::drawSign('0',{188,10},group);   // 2
    textItems_array[4] = PictureBox::drawSign('0',{188,133},group);  // 3
    textItems_array[5] = PictureBox::drawSign('0',{420,-80},group);  // 4
}

void HalfBitSummator::calculateAndDraw(bool x, bool y)
{
    textItems_array[0]->setPlainText({boolToChar(x)});
    textItems_array[1]->setPlainText({boolToChar(y)});
    SummatorLogic::HalfBitRes res = logic.calculateHalfBitFull(x,y);
    textItems_array[2]->setPlainText({boolToChar(res.oper1)});
    textItems_array[3]->setPlainText({boolToChar(res.oper2)});
    textItems_array[5]->setPlainText({boolToChar(res.v)});
    textItems_array[4]->setPlainText({boolToChar(res.u)});
}

char HalfBitSummator::boolToChar(bool x)
{
    char c = '0';
    if (x) c = '1';
    return c;
}
