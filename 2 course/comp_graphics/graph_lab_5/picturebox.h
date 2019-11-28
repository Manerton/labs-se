#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QFrame>
#include <QPainter> // для пейнт евента

class PictureBox : public QFrame
{
    Q_OBJECT
    QPixmap m_Pixmap; // для рисования
    QPixmap m_Grid; // сетка
    QPoint ToCenterCoordinateSystem(const int x, const int y) const;
public:
    explicit PictureBox(QWidget *parent = nullptr);
    void risovanie(); // функция рисования
    void DrawDirectLine(const int x, const int y, QPainter &painter); // нарисовать треугольник
    void DrawGrid(); // нарисовать сетку
    QPoint DirectLineStartPoint = {0,0};

protected:
    virtual void paintEvent(QPaintEvent*); // функция перерисовки, отрисовывает готовый пиксмап из ф-ции risovanie
};
#endif // PICTUREBOX_H

