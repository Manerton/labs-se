#include "particle.h"
#include <QPainter>
#include <QDebug>
#include <ctime>

Particle::Particle() : QGraphicsObject()
{
    // по умолчанию - электрон
    type = ParticleType::electron;
    color = Qt::cyan;
}

QRectF Particle::boundingRect() const
{
    return rect;
}

void Particle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    current_ticks = clock();

    //painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::black);
    painter->setBrush(color);
    painter->drawEllipse(rect);
    QPointF a = {rect.x() + rect.width() / 4, rect.y() + rect.height()/2};
    QPointF b = {rect.x()+ rect.width() - rect.width() / 4, rect.y() + rect.height()/2};

    painter->drawLine(a,b);
    if (type == ParticleType::hole)
    {
        a = {rect.x() + rect.width() / 2, rect.y() + rect.height() / 4};
        b = {rect.x() + rect.width() / 2, rect.y() + rect.height() - rect.height() / 4 };
        painter->drawLine(a,b);
    }
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

const QRectF Particle::geometry() const
{
    return rect;
}

void Particle::setGeometry(const QRectF &value)
{
    rect = value;

    update();


    delta_ticks = clock() - current_ticks;
    if(delta_ticks > 0)
    {
        fps = CLOCKS_PER_SEC / delta_ticks;
        //qDebug() << fps;
    }
}

