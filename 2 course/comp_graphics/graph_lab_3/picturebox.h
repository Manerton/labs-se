#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QFrame>
#include <QPainter> // для пейнт евента
//#include "mainwindow.h"

struct coord
{
    int x, y; // координаты
    int size; // размер линии
    QColor color;
};

class PictureBox : public QFrame
{
    Q_OBJECT
    QPixmap m_Pixmap; // для рисования
    void drawH(int size, coord coord, QPainter *paint); // функция для отрисовки Н в какой-то точке coord с длиной линий size
public:
    explicit PictureBox(QWidget *parent = nullptr);
    void drawHDepth(const QVector<coord> &coords); // отрисовка всех Н
protected:
    virtual void paintEvent(QPaintEvent*); // функция, которая рисует что либо
};
#endif // PICTUREBOX_H

