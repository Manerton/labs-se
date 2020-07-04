#include "diod.h"
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QPen>
#include <vector>
#include <QDebug>
#include "mainwindow.h"
#include <random>
#include <thread>
#include <ctime>
#include <QTimer>
#include <QEventLoop>
#include "picturebox.h"
using namespace std;

diod::diod() : particleArr(60), animationArr(40)
{
    group = new QGraphicsItemGroup();
    movingParticles = new QGraphicsItemGroup();
    idleParticles = new QGraphicsItemGroup();
    powerSourceOn = new QGraphicsItemGroup();
    powerSourceOff = new QGraphicsItemGroup();
    animTimer = new QTimer();
}

diod::~diod()
{
    delete group;
}

void diod::draw()
{
    QPen penBlack(Qt::black);
    penBlack.setWidth(3);
    // рисуем полупроводник
    QGraphicsRectItem* rect = new QGraphicsRectItem(0,0,w,h);
    rect->setPen(penBlack);
    rect->setBrush(Qt::white);
    group->addToGroup(rect);

    PictureBox::drawSign('p',{w/4-20,h-10},group);
    PictureBox::drawSign('n',{w-w/4-20,h-10},group);
    // рисуем электроны
    drawParticles();
    drawIdleParticles();
    // рисуем зоны, в которую должны уходить атомы (для эффекта исчезания)
    drawBounds();
    // рисуем схему от источника тока
    drawPowerSourceOn();
    drawPowerSourceOff();
    // анимации (последовательная группа анимаций)
    prepareAnim();
    readyToShow = true;
    active = true;
}

void diod::drawPowerSourceOn()
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
    powerSourceOn->addToGroup(mainLine);
    powerSourceOn->setZValue(10);
    group->addToGroup(powerSourceOn);
    // добавляем жирную линию со стороны минуса у источника тока
    QGraphicsLineItem* minusLine = new QGraphicsLineItem();
    minusLine->setLine(340,halfH-155,340,halfH-125);
    pen.setWidth(6);
    minusLine->setPen(pen);
    minusLine->setParentItem(mainLine);
    // знаки + и -
    PictureBox::drawSign('+',{700+40,halfH-40},powerSourceOn);
    PictureBox::drawSign('-',{-68,halfH-40},powerSourceOn);
    PictureBox::drawSign('+',{365,halfH-186},powerSourceOn);
    PictureBox::drawSign('-',{300,halfH-185},powerSourceOn);
}

void diod::drawPowerSourceOff()
{
    // рисуем схему источника тока
    QPainterPath* path = new QPainterPath();
    QPen pen = {Qt::black};
    pen.setWidth(2);
    path->moveTo(-2,halfH);
    path->lineTo(-30,halfH);
    path->lineTo(-30,halfH-140);
    path->lineTo(340,halfH-140);
    path->lineTo(340,halfH-170);
    path->lineTo(340,halfH-110);
    path->moveTo(702,halfH);
    path->lineTo(700+30,halfH);
    path->lineTo(700+30,halfH-140);
    path->lineTo(360,halfH-140);

    path->moveTo(w/2,0);
    path->lineTo(w/2,h);
    QGraphicsPathItem* mainLine = new QGraphicsPathItem(*path);
    mainLine->setPath(*path);
    mainLine->setPen(pen);
    powerSourceOff->addToGroup(mainLine);
    powerSourceOff->setZValue(10);
    group->addToGroup(powerSourceOff);
    // добавляем жирную линию со стороны минуса у источника тока
    QGraphicsLineItem* minusLine = new QGraphicsLineItem();
    minusLine->setLine(360,halfH-155,360,halfH-125);
    pen.setWidth(6);
    minusLine->setPen(pen);
    minusLine->setParentItem(mainLine);
    // знаки + и -
    PictureBox::drawSign('-',{700+40,halfH-40},powerSourceOff);
    PictureBox::drawSign('+',{-68,halfH-40},powerSourceOff);
    PictureBox::drawSign('-',{365,halfH-186},powerSourceOff);
    PictureBox::drawSign('+',{300,halfH-185},powerSourceOff);

    powerSourceOff->hide();
}

void diod::drawParticles()
{
    for (int i = 0; i < 40; ++i)
    {
        if (i % 2 == 0)
        {
            particleArr[i] = new Particle();
            particleArr[i]->setGeometry({-20,5,particleSize,particleSize});
        }
        else
        {
            particleArr[i] = new Particle(Particle::ParticleType::hole);
            particleArr[i]->setGeometry({725,5,particleSize,particleSize});
        }
        particleArr[i]->setZValue(2);
        movingParticles->addToGroup(particleArr[i]);
    }
    group->addToGroup(movingParticles);
}

void diod::drawIdleParticles()
{
    time_t seed = time(nullptr);
    mt19937 mt(seed);
    uniform_int_distribution<int> rand_x(halfParticleSize,particleSize*2); // небольшой разброс
    double y = 20;
    for (int i = 40; i < 46; ++i)
    {
        particleArr[i] = new Particle();
        particleArr[i]->setGeometry({double(rand_x(mt)),double(y),particleSize,particleSize});
        particleArr[i]->setZValue(2);
        idleParticles->addToGroup(particleArr[i]);
        y+=20;
    }
    y = 20;
    for (int i = 46; i < 50; ++i)
    {
        particleArr[i] = new Particle();
        particleArr[i]->setGeometry({double(rand_x(mt)+38),double(y),particleSize,particleSize});
        particleArr[i]->setZValue(2);
        idleParticles->addToGroup(particleArr[i]);
        y+=20;
    }
    y = 20;
    for (int i = 50; i < 56; ++i)
    {
        particleArr[i] = new Particle(Particle::ParticleType::hole);
        particleArr[i]->setGeometry({double(w-rand_x(mt)-particleSize),double(y),particleSize,particleSize});
        particleArr[i]->setZValue(2);
        idleParticles->addToGroup(particleArr[i]);
        y+=20;
    }
    y = 20;
    for (int i = 56; i < 60; ++i)
    {
        particleArr[i] = new Particle(Particle::ParticleType::hole);
        particleArr[i]->setGeometry({double(w-rand_x(mt)-particleSize*3),double(y),particleSize,particleSize});
        particleArr[i]->setZValue(2);
        idleParticles->addToGroup(particleArr[i]);
        y+=20;
    }
    group->addToGroup(idleParticles);
    idleParticles->hide();
}

void diod::drawBounds()
{
    QGraphicsRectItem *outRight = new QGraphicsRectItem(w+1,0,particleSize*2,h);
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

void diod::prepareAnim()
{
    time_t seed = time(nullptr);
    mt19937 mt(seed);
    uniform_int_distribution<int> rand_y(4,h-particleSize-halfParticleSize); // в пределах проводника
    double prev_y_for_elec = 0;
    double prev_y_for_hole = 0;
    for (int i = 0; i < 40; ++i)
    {
        double y = 0;
        double xStart = -20;
        double xEnd = w/2-10;
        if (i % 2 == 1)
        {
            xStart = w+particleSize;
            xEnd = w/2-4;
            do {
                y = rand_y(mt);
            } while (fabs(y - prev_y_for_hole) < particleSize);
            prev_y_for_hole = y;
        } else
        {
            do {
                y = rand_y(mt);
            } while (fabs(y - prev_y_for_elec) < particleSize);
            prev_y_for_elec = y;
        }

        animationArr[i] = new QPropertyAnimation(particleArr[i],"geometry");
        animationArr[i]->setDuration(animDuration);
        animationArr[i]->setStartValue(QRectF(xStart,y,particleSize,particleSize));
        animationArr[i]->setEndValue(QRectF(xEnd,y,particleSize,particleSize));
        animationArr[i]->setLoopCount(-1);
    }
    connect(animTimer, &QTimer::timeout, this, &diod::resumeAnim);
    animTimer->start(62);
}

void diod::pause()
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

void diod::unpause()
{
    if (!active && readyToShow)
    {
        if (animationArr[0]->state() == QAbstractAnimation::State::Stopped) // если в прошлый раз остановили
        {
            animTimer->start(62);
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

void diod::resumeAnim()
{
    animationArr[i]->start();
    ++i;
    if (i == 40) animTimer->stop();
}

void diod::changePolarity()
{
    if (powerSourceOn->isVisible())
    {
        powerSourceOn->hide();
        movingParticles->hide();
        powerSourceOff->show();
        idleParticles->show();
    }
    else
    {
        powerSourceOff->hide();
        idleParticles->hide();
        powerSourceOn->show();
        movingParticles->show();
    }
}
