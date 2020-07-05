#include "transistor.h"
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QPen>
#include <vector>
#include <QDebug>
#include "demo.h"
#include <random>
#include <thread>
#include <ctime>
#include <QTimer>
#include <QEventLoop>
#include "picturebox.h"
using namespace std;

Transistor::Transistor() : particleArr(30), animationArr(30)
{
    group = new QGraphicsItemGroup();
    movingParticles = new QGraphicsItemGroup();
    powerSource = new QGraphicsItemGroup();
    animTimer = new QTimer();
}

Transistor::~Transistor()
{
    delete group;
}

void Transistor::draw()
{
    QPen penBlack(Qt::black);
    penBlack.setWidth(3);
    // рисуем полупроводник
    QGraphicsRectItem* rect = new QGraphicsRectItem(0,0,w,h);
    rect->setPen(penBlack);
    rect->setBrush(Qt::white);
    group->addToGroup(rect);

    PictureBox::drawSign('n',{w/4-20,h-10},group);
    PictureBox::drawSign('p',{w-w/4-20,h-10},group);
    // рисуем электроны
    drawParticles();
    // рисуем зоны, в которую должны уходить атомы (для эффекта исчезания)
    drawBounds();
    // рисуем схему от источника тока
    drawPowerSource();
    // анимации
    prepareAnim();
    readyToShow = true;
    active = true;
}

void Transistor::drawPowerSource()
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
    path->moveTo(w/2,0);
    path->lineTo(w/2,h);
    QGraphicsPathItem* mainLine = new QGraphicsPathItem(*path);
    mainLine->setPath(*path);
    mainLine->setPen(pen);
    powerSource->addToGroup(mainLine);
    powerSource->setZValue(10);
    group->addToGroup(powerSource);
    // добавляем жирную линию со стороны минуса у источника тока
    QGraphicsLineItem* minusLine = new QGraphicsLineItem();
    minusLine->setLine(340,halfH-155,340,halfH-125);
    pen.setWidth(6);
    minusLine->setPen(pen);
    minusLine->setParentItem(mainLine);
    // знаки + и -
    PictureBox::drawSign('+',{700+40,halfH-40},powerSource);
    PictureBox::drawSign('-',{-68,halfH-40},powerSource);
    PictureBox::drawSign('+',{365,halfH-186},powerSource);
    PictureBox::drawSign('-',{300,halfH-185},powerSource);
}

void Transistor::drawParticles()
{
    for (int i = 0; i < 30; ++i)
    {
        particleArr[i] = new Particle();
        particleArr[i]->setGeometry({-20,5,particleSize,particleSize});
        particleArr[i]->setZValue(2);
        movingParticles->addToGroup(particleArr[i]);
    }
    group->addToGroup(movingParticles);
}

void Transistor::drawBounds()
{
    QGraphicsRectItem *outRight = new QGraphicsRectItem(w+1,0,particleSize*3,h);
    QGraphicsRectItem *outLeft = new QGraphicsRectItem(-3-32,0,particleSize*2,h);
    QGraphicsRectItem *outMid = new QGraphicsRectItem(w/2-6,4,12,h-6);
    outRight->setBrush(Qt::white);
    outLeft->setBrush(Qt::white);
    outMid->setBrush(Qt::white);
    outRight->setPen(QPen(Qt::NoPen));
    outLeft->setPen(QPen(Qt::NoPen));
    outMid->setPen(QPen(Qt::NoPen));
    outRight->setZValue(10);
    outLeft->setZValue(10);
    outMid->setZValue(5);
    group->addToGroup(outRight);
    group->addToGroup(outLeft);
    group->addToGroup(outMid);
}

void Transistor::prepareAnim()
{
    time_t seed = time(nullptr);
    mt19937 mt(seed);
    uniform_int_distribution<int> rand_y(4,h-particleSize-halfParticleSize); // в пределах проводника
    double prev_y = 0;
    for (int i = 0; i < 30; ++i)
    {
        double y = 0;
        do {
            y = rand_y(mt);
        } while (fabs(y - prev_y) < particleSize);
        prev_y = y;

        animationArr[i] = new QPropertyAnimation(particleArr[i],"geometry");
        animationArr[i]->setDuration(animDuration);
        animationArr[i]->setStartValue(QRectF(-20,y,particleSize,particleSize));
        animationArr[i]->setEndValue(QRectF(w/2-10,y,particleSize,particleSize));
        animationArr[i]->setLoopCount(-1);
    }
    connect(animTimer, &QTimer::timeout, this, &Transistor::resumeAnim);
    animTimer->start(83);
}

void Transistor::pause()
{
    if (active && readyToShow)
    {
        // обрабатываем случай, когда переключились на другую демонстрацию
        // а таймер, подготавливающий анимацию потока электрона ещё не закончился
        // т.е анимация не сформирована до конца, поэтому запустим её в следующий раз заново
        if (animTimer->isActive())
        {
            i=0;
            animTimer->stop();
            for (auto &anim : animationArr)
            {
                anim->stop();
                anim->targetObject()->setProperty(anim->propertyName(),anim->startValue());
            }
        }
        else    // иначе поставим анимацию на паузу
        {
            for (auto &anim : animationArr)
            {
                anim->pause();
            }
        }

        active = false;
    }
}

void Transistor::unpause()
{
    if (!active && readyToShow)
    {
        if (animationArr[0]->state() == QAbstractAnimation::State::Stopped) // если в прошлый раз остановили
        {
            animTimer->start(83);
        }
        else    // если поставили на паузу
        {
            for (auto &anim : animationArr)
            {
                anim->resume();
            }
        }
        active = true;
    }
}

void Transistor::resumeAnim()
{
    animationArr[i]->start();
    ++i;
    if (i == 30) animTimer->stop();
}
