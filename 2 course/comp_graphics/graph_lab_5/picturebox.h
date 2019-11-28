#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QFrame>
#include <QPainter> // для пейнт евента

enum class DrawObject {
    Line,
    Circle,
    Ellipse
};


class PictureBox : public QFrame
{
    Q_OBJECT
    QPixmap m_Pixmap; // для рисования
    QPixmap m_Grid; // сетка
    QPoint ToCenterCoordinateSystem(const int x, const int y) const;
    void DrawCircle(const int r, QPainter &painter); // нарисовать круг
    void DrawDirectLine(const int x, const int y, QPainter &painter); // нарисовать линию
    void DrawEllipse(const int a, const int b, QPainter &painter);
public:
    explicit PictureBox(QWidget *parent = nullptr);
    void risovanie(DrawObject obj); // функция рисования

    void DrawGrid(); // нарисовать сетку
    QPoint DirectLineStartPoint = {0,0};
    QPoint EllipseAB = {0,0};
    int CircleR;

protected:
    virtual void paintEvent(QPaintEvent*); // функция перерисовки, отрисовывает готовый пиксмап из ф-ции risovanie
};
#endif // PICTUREBOX_H

