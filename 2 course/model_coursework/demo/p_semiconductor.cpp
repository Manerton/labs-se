#include "p_semiconductor.h"
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QPen>
#include <vector>
#include <QDebug>
#include "demo.h"
#include <thread>
using namespace std;

P_semiconductor::P_semiconductor() : particleArr(10), animationArr(11)
{
    group = new QGraphicsItemGroup();
    animation_group = new QSequentialAnimationGroup();
}

P_semiconductor::~P_semiconductor()
{
    delete group;
    delete animation_group;
}

void P_semiconductor::draw()
{
    QPen penBlack(Qt::black);
    penBlack.setWidth(3);
    // рисуем полупроводник
    QGraphicsRectItem* rect = new QGraphicsRectItem(0,0,w,h);
    rect->setRect(0,0,w,h);
    rect->setPen(penBlack);
    rect->setBrush(Qt::white);
    group->addToGroup(rect);
    // рисуем атомы
    drawAtoms(rect,{30,halfH-halfAtomSize});

    double xForElec = w-30-halfParticleSize;
    double yForElec = halfH-halfParticleSize;
    // рисуем электроны
    drawElectrons({xForElec,yForElec});
    // рисуем зоны, в которую должны уходить атомы (для эффекта исчезания)
    drawBounds();
    // рисуем схему от источника тока
    drawPowerSource();
    // анимации (последовательная группа анимаций)
    prepareAnim({xForElec,yForElec});

    readyToShow = true;
    active = true;
}

void P_semiconductor::drawPowerSource()
{
    // рисуем схему источника тока
    QPainterPath* path = new QPainterPath();
    QPen pen = {Qt::black};
    pen.setWidth(2);
    path->moveTo(-2,halfH);
    path->lineTo(-30,halfH);
    path->lineTo(-30,halfH-140);
    path->lineTo(340,halfH-140);
    path->moveTo(702,halfH);
    path->lineTo(700+30,halfH);
    path->lineTo(700+30,halfH-140);
    path->lineTo(360,halfH-140);
    path->lineTo(360,halfH-170);
    path->lineTo(360,halfH-110);
    QGraphicsPathItem* mainLine = new QGraphicsPathItem(*path);
    mainLine->setPath(*path);
    mainLine->setPen(pen);
    mainLine->setZValue(5);
    group->addToGroup(mainLine);
    // добавляем жирную линию со стороны минуса у источника тока
    QGraphicsLineItem* minusLine = new QGraphicsLineItem();
    minusLine->setLine(340,halfH-155,340,halfH-125);
    pen.setWidth(6);
    minusLine->setPen(pen);
    minusLine->setParentItem(mainLine);
    // знаки + и -
    PictureBox::drawSigns("+",{700+40,halfH-40},group);
    PictureBox::drawSigns("-",{-68,halfH-40},group);
    PictureBox::drawSigns("+",{365,halfH-186},group);
    PictureBox::drawSigns("-",{300,halfH-185},group);
}

void P_semiconductor::drawAtom(QGraphicsItem *parent, const QPointF &coords)
{
    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(0,0,80,80,parent);
    circle->setPos(coords);
    QPen pen = {Qt::black};
    pen.setWidth(2);
    circle->setPen(pen);
    circle->setBrush(Qt::white);
    Particle *elec1 = new Particle();
    Particle *elec2 = new Particle();
    Particle *hole1 = new Particle(Particle::ParticleType::hole);
    Particle *hole2 = new Particle(Particle::ParticleType::hole);
    elec1->setParentItem(circle);
    elec1->setGeometry({40-8,-7,16,16});
    elec1->setZValue(1);
    elec2->setParentItem(circle);
    elec2->setGeometry({40-8,80-10,16,16});
    elec2->setZValue(1);
    hole1->setParentItem(circle);
    hole1->setGeometry({80-8,40-8,16,16});
    hole1->setZValue(1);
    hole2->setParentItem(circle);
    hole2->setGeometry({-8,40-8,16,16});
    hole2->setZValue(1);
}

void P_semiconductor::drawAtoms(QGraphicsItem *rect, const QPointF &coords)
{
    double x = coords.x();
    double y = coords.y();
    drawAtom(rect,{x,y});
    x += betweenAtoms + atomSize;
    drawAtom(rect,{x,y});
    x += betweenAtoms + atomSize;
    drawAtom(rect,{x,y});
    x += betweenAtoms + atomSize;
    drawAtom(rect,{x,y});
    x += betweenAtoms + atomSize;
    drawAtom(rect,{x,y});
}

void P_semiconductor::drawElectrons(const QPointF &coords)
{
    double x = coords.x();
    double y = coords.y();
    for (int i = 0; i < 10; ++i)
    {
        particleArr[i] = new Particle();
        particleArr[i]->setZValue(2);
        particleArr[i]->setGeometry({x,y,particleSize,particleSize});
        if (i % 2 == 1) x -= betweenAtoms;
        else x -= atomSize;
        group->addToGroup(particleArr[i]);
    }

}

void P_semiconductor::drawBounds()
{
    QGraphicsRectItem *outRight = new QGraphicsRectItem(w+2,0,30,h);
    QGraphicsRectItem *outLeft = new QGraphicsRectItem(-4-30,0,particleSize*2,h);
    outRight->setBrush(Qt::white);
    outLeft->setBrush(Qt::white);
    outRight->setPen(QPen(Qt::NoPen));
    outLeft->setPen(QPen(Qt::NoPen));
    outRight->setZValue(5);
    outLeft->setZValue(5);
    group->addToGroup(outRight);
    group->addToGroup(outLeft);
}

void P_semiconductor::prepareAnim(const QPointF &coords)
{
    // создадим последовательную группу анимаций
    double xForElec = coords.x();
    double yForElec = coords.y();
    // настроим вручную первую и последнюю
    prepareAnimFirstAndLast({xForElec,yForElec});
    xForElec -= atomSize;
    // настраиваем все остальные анимации, стартовые и конечные позиции
    // длительность анимации и пауз, добавляем анимации в группу
    for (int i = 1; i < 10; ++i)
    {
        animationArr[i] = new QPropertyAnimation(particleArr[i],"geometry");
        animationArr[i]->setDuration(animDuration);
        double startX = xForElec;
        double endX;
        if (i % 2 == 1)
        {
            endX = xForElec + atomSize;
            xForElec -= betweenAtoms;
        }
        else
        {
            endX = xForElec + betweenAtoms;
            xForElec -= atomSize;
        }
        animationArr[i]->setStartValue(QRectF(startX,yForElec,particleSize,particleSize));
        animationArr[i]->setEndValue(QRectF(endX,yForElec,particleSize,particleSize));
        animation_group->addPause(pauseDuration);
        animation_group->addAnimation(animationArr[i]);
    }
    // добавляем последнюю анимацию в группу
    animation_group->addPause(pauseDuration);
    animation_group->addAnimation(animationArr[10]);
    animation_group->addPause(pauseDuration*3);
    // ставим бесконечный цикл
    animation_group->setLoopCount(-1);
    animation_group->start();
}

void P_semiconductor::prepareAnimFirstAndLast(const QPointF &coords)
{
    double xForElec = coords.x();
    double yForElec = coords.y();
    // первая
    animationArr[0] = new QPropertyAnimation(particleArr[0],"geometry");
    animationArr[0]->setDuration(animDuration);
    animationArr[0]->setStartValue(QRectF(xForElec,yForElec,particleSize,particleSize));
    animationArr[0]->setEndValue(QRectF(xForElec+30+particleSize,yForElec,particleSize,particleSize));
    animation_group->addAnimation(animationArr[0]);
    animation_group->addPause(pauseDuration);
    // последняя
    animationArr[10] = new QPropertyAnimation(particleArr[0],"geometry");
    animationArr[10]->setDuration(animDuration);
    animationArr[10]->setStartValue(QRectF(-particleSize,yForElec,particleSize,particleSize));
    animationArr[10]->setEndValue(QRectF(30-halfParticleSize,yForElec,particleSize,particleSize));
}

void P_semiconductor::resetAnimations()
{
    qDebug() << "reset";
    // проходим по всем анимациям в массиве анимаций и устанавливаем их на стартовые позиции
    for (auto &anim : animationArr)
    {
        anim->targetObject()->setProperty(anim->propertyName(),anim->startValue());
    }
}

void P_semiconductor::pause()
{
    if (active && readyToShow)
    {
        animation_group->pause();
        active = false;
    }
}

void P_semiconductor::unpause()
{
    if (!active && readyToShow)
    {
        animation_group->resume();
        active = true;
    }
}
