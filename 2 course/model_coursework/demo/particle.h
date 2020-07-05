#ifndef ELECTRON_H
#define ELECTRON_H

#include <QGraphicsObject>

// класс, изображающую частицу, у которой есть свойство geometry
// свойство необходимо для анимаций
class Particle : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(QRectF geometry READ geometry WRITE setGeometry);
    Q_INTERFACES(QGraphicsItem);
public:
    // тип частицы
    enum class ParticleType : uint8_t
    {
        electron = 0,   // электрон
        hole = 1        // дырка
    };

    explicit Particle();

    // цвет в зависимости от вида частицы
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

    // методы от QGraphicsItem
    // занятая область частицой
    virtual QRectF boundingRect() const;
    // метод отрисовки
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    // параметр геометрии
    const QRectF geometry() const;
    void setGeometry(const QRectF &value);
private:
    QRectF rect;        // координаты
    ParticleType type;  // тип частицы
    QColor color;       // цвет частицы
};

#endif // ELECTRON_H
