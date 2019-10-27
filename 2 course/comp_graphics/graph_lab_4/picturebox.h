#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QFrame>
#include <QPainter> // для пейнт евента

struct Triangle // треугольник
{
    QPointF A; // точки треугольника
    QPointF B;
    QPointF C;
};

class PictureBox : public QFrame
{
    Q_OBJECT
    QPixmap m_Pixmap; // для рисования
    QPixmap empty; // для reset
public:
    explicit PictureBox(QWidget *parent = nullptr);
    void risovanie(const Triangle &Tri); // функция рисования
    void DrawTriangle(const Triangle &Tri, QPainter &painter); // нарисовать треугольник
    void DrawAxes(int width, int height, QPainter &painter); // нарисовать оси системы координат
    void Drawline_D(int d); // нарисовать прямую y = d
    void reset(); // очищает pixmap от старых следов при вызове

protected:
    virtual void paintEvent(QPaintEvent*); // функция перерисовки, отрисовывает готовый пиксмап из ф-ции risovanie
};
#endif // PICTUREBOX_H

