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

Transistor::Transistor() : particleArr(33), animationArr(30)
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

    PictureBox::drawSigns("n",{(w/2-60)/2,h-10},group);
    PictureBox::drawSigns("p",{w/2+10,h-10},group);
    PictureBox::drawSigns("n",{w-(w/2-60)/2,h-10},group);
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
    group->moveBy(0,-70);
}

void Transistor::drawPowerSource()
{
    // рисуем схему источника тока
    QPainterPath* path = new QPainterPath();
    QPen pen = {Qt::black};
    pen.setWidth(2);
    path->moveTo(-2,halfH);
    path->lineTo(-30,halfH); // 140 -> 200 = 60
    path->lineTo(-30,halfH+200);
    path->lineTo(320,halfH+200);
    path->moveTo(702,halfH);
    path->lineTo(700+30,halfH);
    path->lineTo(700+30,halfH+200);
    path->lineTo(380,halfH+200);
    path->lineTo(380,halfH+230);
    path->lineTo(380,halfH+170);
    path->moveTo(340,halfH+230);
    path->lineTo(340,halfH+170);
    path->moveTo(w/2-60,0);
    path->lineTo(w/2-60,h);
    path->moveTo(w/2+60,0);
    path->lineTo(w/2+60,h);
    path->moveTo(-15,halfH);
    path->lineTo(-15,halfH+140);
    path->lineTo(w/4,halfH+140);
    path->moveTo(w/4+20,halfH+110);
    path->lineTo(w/4+20,halfH+170);
    path->moveTo(w/4+20,halfH+140);
    path->lineTo(w/2,halfH+140);
    path->lineTo(w/2,h);
    QGraphicsPathItem* mainLine = new QGraphicsPathItem(*path);
    mainLine->setPath(*path);
    mainLine->setPen(pen);
    powerSource->addToGroup(mainLine);
    powerSource->setZValue(10);
    group->addToGroup(powerSource);
    // добавляем жирную линию со стороны минуса у источника тока
    QGraphicsLineItem* minusLine = new QGraphicsLineItem();
    minusLine->setLine(320,halfH+185,320,halfH+215);
    pen.setWidth(6);
    minusLine->setPen(pen);
    minusLine->setParentItem(mainLine);
    QGraphicsLineItem* minusLine2 = new QGraphicsLineItem();
    minusLine2->setLine(360,halfH+185,360,halfH+215);
    minusLine2->setPen(pen);
    minusLine2->setParentItem(mainLine);
    QGraphicsLineItem* minusLine3 = new QGraphicsLineItem();
    minusLine3->setLine(w/4,halfH+125,w/4,halfH+155);
    minusLine3->setPen(pen);
    minusLine3->setParentItem(mainLine);
    QGraphicsLineItem* baseLine = new QGraphicsLineItem();
    baseLine->setLine(w/2-60+2,h-2,w/2+60-2,h-2);
    baseLine->setPen(pen);
    baseLine->setParentItem(mainLine);
    baseLine->setZValue(11);
    // знаки + и -
    PictureBox::drawSigns("+",{700+40,halfH-40},powerSource);
    PictureBox::drawSigns("-",{-68,halfH-40},powerSource);
    PictureBox::drawSigns("+",{380,halfH+190},powerSource);
    PictureBox::drawSigns("-",{290,halfH+190},powerSource);
    // стрелки
    drawArrow({w/4-50,halfH+120});
    drawArrow({w/4-50,halfH+180});
    drawArrow({w/4+400,halfH+180});
    // E база-эмиттер
    PictureBox::drawSigns("E б-э",{w/4+40,halfH+140},powerSource,18);
    PictureBox::drawSigns("E к-э",{w-80,halfH+200},powerSource,18);
}

void Transistor::drawParticles()
{
    for (int i = 0; i < 33; ++i)
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
    QGraphicsRectItem *outLeft = new QGraphicsRectItem(-3-40,0,42,h);
    QGraphicsRectItem *outMid = new QGraphicsRectItem(w/2-60,h,120,particleSize*2);
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
    double offset_x = 0;
    for (int i = 0; i < 3; ++i)
    {
        double y = 0;
        do {
            y = rand_y(mt);
        } while (fabs(y - prev_y) < particleSize+halfParticleSize);
        prev_y = y;

        animationArr[i] = new QPropertyAnimation(particleArr[i],"geometry");
        animationArr[i]->setDuration(animDuration/2+500);
        animationArr[i]->setStartValue(QRectF(-40,y,particleSize+halfParticleSize,particleSize+halfParticleSize));
        animationArr[i]->setEndValue(QRectF(w/2-40+offset_x,y,particleSize+halfParticleSize,particleSize+halfParticleSize));
        animationArr[i]->setLoopCount(-1);
        offset_x += particleSize+halfParticleSize;
    }
    connect(animationArr[0], &QPropertyAnimation::currentLoopChanged, this, [this]{Transistor::doSomething(0);});
    connect(animationArr[1], &QPropertyAnimation::currentLoopChanged, this, [this]{Transistor::doSomething(1);});
    connect(animationArr[2], &QPropertyAnimation::currentLoopChanged, this, [this]{Transistor::doSomething(2);});

    prev_y = 0;
    for (int i = 3; i < 30; ++i)
    {
        double y = 0;
        do {
            y = rand_y(mt);
        } while (fabs(y - prev_y) < particleSize+halfParticleSize);
        prev_y = y;

        animationArr[i] = new QPropertyAnimation(particleArr[i],"geometry");
        animationArr[i]->setDuration(animDuration);
        animationArr[i]->setStartValue(QRectF(-20,y,particleSize,particleSize));
        animationArr[i]->setEndValue(QRectF(w+20,y,particleSize,particleSize));
        animationArr[i]->setLoopCount(-1);
    }
    connect(animTimer, &QTimer::timeout, this, &Transistor::resumeAnim);
    animTimer->start(100);
}

void Transistor::drawArrow(const QPointF &coords)
{
    QGraphicsLineItem* arrow = new QGraphicsLineItem();
    QPen pen = {Qt::black};
    pen.setWidth(2);
    arrow->setPen(pen);
    arrow->setLine(coords.x(),coords.y(),coords.x()+30,coords.y());
    group->addToGroup(arrow);
    QGraphicsLineItem* forArrow1 = new QGraphicsLineItem();
    forArrow1->setPen(pen);
    forArrow1->setLine(coords.x()+1,coords.y()+1,coords.x()+10,coords.y()+4);
    forArrow1->setParentItem(arrow);
    QGraphicsLineItem* forArrow2 = new QGraphicsLineItem();
    forArrow2->setPen(pen);
    forArrow2->setLine(coords.x()+1,coords.y()-1,coords.x()+10,coords.y()-4);
    forArrow2->setParentItem(arrow);
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
            animTimer->start(100);
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

void Transistor::doSomething(int i)
{
    animationArr[i]->pause();
    double offset_X = 0;
    if (i == 1) offset_X = particleSize+halfParticleSize;
    if (i == 2) offset_X = (particleSize+halfParticleSize)*2;
    QPropertyAnimation* anim = new QPropertyAnimation(particleArr[30+i],"geometry");
    anim->setDuration(animDuration/5);
    anim->setStartValue(animationArr[i]->endValue());
    anim->setEndValue(QRectF(w/2-40+offset_X,h,particleSize+8,particleSize+8));
    anim->start(QAbstractAnimation::DeletionPolicy::DeleteWhenStopped);
    connect(anim, &QPropertyAnimation::finished, animationArr[i], &QPropertyAnimation::resume);
}
