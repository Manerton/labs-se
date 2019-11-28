#include <cmath>
#include <QDebug>
#include "picturebox.h"
using namespace std;

PictureBox::PictureBox(QWidget *parent) : QFrame(parent)
{
    resize(parent->width()-2, parent->height()-2); // размер как у родительского виджета, с небольшим отступом в 2 пикселя, чтобы не удалять оранжевую рамку фрейма
    m_Pixmap = QPixmap(size()); // инициализирую полотно, на котором буду рисовать
    m_Pixmap.fill(Qt::white); // заливаю белым цветом
    m_Grid = QPixmap(500,500); // создаю полотно для сетки, пусть будет 500x500
    m_Grid.fill(Qt::white); // ее тоже белым заливаю
    DrawGrid(); // формирую сетку и рисую ее на полотне, потом буду рисовать сетку как картинку, без повторного формирования сетки
}

QPoint PictureBox::ToCenterCoordinateSystem(const int x, const int y) const
{
    int width = m_Grid.width();
    int height = m_Grid.height();
    QPoint A = {width/2 - x*10 - 10, height/2 + y*10}; // смещаю к центру фрейма и масштабирую пиксели на размер одной клетки сетки
    return A;
}

void PictureBox::DrawDirectLine(const int x1, const int y1, QPainter &painter)
{ 
    int x, y, xc, yc, Px, Py, E;
    x = 0;
    y = 0;

    xc = m_Grid.width()/2;
    yc = m_Grid.height()/2;

    Px = x1;
    Py = y1;

    E = 2 * Py - Px;
    QPoint point = ToCenterCoordinateSystem(x,y);
    painter.fillRect(point.x(),point.y(),10,10,Qt::blue);

    for (int i = 1; i < Px; ++i)
    {
        ++x;
        if (E >= 0)
        {
            ++y;
            E += 2 * (Py - Px);
        }
        else E += 2 * Py;
        point = ToCenterCoordinateSystem(x,y);
        painter.fillRect(point.x(),point.y(),10,10,Qt::blue);
    }


}

void PictureBox::DrawGrid()
{
    QPainter painter(&m_Grid);
    painter.setPen(Qt::green);
    for (int i = 0; i < height(); i += 10)
    {
        painter.drawLine(0, i, width(), i); // горизонтальная линия
        painter.drawLine(i, 0, i, height()); // вертикальная
    }
    painter.setPen(Qt::black);
    painter.drawLine(0, 250, width(), 250); // ось X
    painter.drawLine(250, 0, 250, 500); // ось Y
}

void PictureBox::risovanie()
{
      m_Pixmap.fill(Qt::white); // очищаю фрейм (заливаю белым цветом)
      QPainter painter(&m_Pixmap);
      painter.setRenderHint(QPainter::Antialiasing, true);
      painter.drawPixmap(0,0,m_Grid); // рисую сетку
      DrawDirectLine(DirectLineStartPoint.x(),DirectLineStartPoint.y(),painter);
}

void PictureBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(1,1,m_Pixmap);
}
