#ifndef ELECTRON_H
#define ELECTRON_H
#include <QGraphicsObject>
#include <QRect>

class Particle : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(QRectF geometry READ geometry WRITE setGeometry);
    Q_INTERFACES(QGraphicsItem);
public:

    enum class ParticleType : uint8_t
    {
        electron = 0,
        hole = 1
    };

    explicit Particle();

    Particle(ParticleType _type) : type(_type)
    {
        if (_type == ParticleType::electron)
        {
            color = Qt::cyan;
        }
        else if (_type == ParticleType::hole)
        {
            color = Qt::green;
        }
    }

    // от QGraphicsItem
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    // параметр геометрии
    const QRectF geometry() const;
    void setGeometry(const QRectF &value);
private:
    QRectF rect;        // координаты
    ParticleType type;  // тип частицы
    QColor color;       // цвет частицы

    clock_t current_ticks, delta_ticks, fps;
};

#endif // ELECTRON_H
