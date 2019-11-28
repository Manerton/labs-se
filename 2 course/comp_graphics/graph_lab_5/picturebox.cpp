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
    QPainter painter(&m_Pixmap);
    painter.drawPixmap(0,0,m_Grid); // рисую сетку в самом начале, при запуске программы
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
    int x, y, Px, Py, E;
    x = 0;
    y = 0;

    Px = x1;
    Py = y1;

    E = 2 * Py - Px;
    QPoint point = ToCenterCoordinateSystem(x,y);
    painter.fillRect(point.x()+1,point.y()+1,9,9,Qt::blue);

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
        painter.fillRect(point.x()+1,point.y()+1,9,9,Qt::blue);
    }
}

void PictureBox::DrawCircle(const int r, QPainter &painter)
{
    int x, y, l, r2, dst, t, s, e, ca, cd;
    r2 = r*r; // квадрат радиуса, используется в промежуточных результатах
    dst = 4 * r2;
    l = int(double(r) / sqrt(2)); // расстояние от оси до октанта (диагонали квадранта, где находится окружность)
    t = 0;
    s = -4 * r2 * r;
    e = (-s/2) - 3 * r2;
    ca = -6 * r2;
    cd = -10 * r2;
    x = 0;
    y = r+1; // функция ToCenter... сдвигает начало координат в 3 квадрант, поэтому добавляю +1 к "y", чтобы перейти за ось Ox ко 2 квадранту

    QPoint point = ToCenterCoordinateSystem(x,-y); // так как функция сдвигает в 3 квадрант, а мне нужен 2 квадрант (3 октант), то передаю "y" с минусом
    painter.fillRect(point.x()+1,point.y()+1,9,9,Qt::blue);

    for (int i = 0; i < l; ++i)
    {
        ++x;
        if (e >= 0)
        {
            e += t + ca;
        }
        else
        {
            --y;
            e += t - s + cd;
            s += dst;
        }
        t -= dst;
        point = ToCenterCoordinateSystem(x,-y);
        painter.fillRect(point.x()+1,point.y()+1,9,9,Qt::blue);
    }
}

void PictureBox::DrawEllipse(const int a, const int b, QPainter &painter)
{
    int x, y, a2, b2, l, dds, ddt, t, s, e, ca, cd;
    a2 = a*a;
    b2 = b*b;
    dds = 4 * a2;
    ddt = 4 * b2;
    l = int(double(a2) / sqrt(a2+b2));
    t = 0;
    s = -4 * a2 * b;
    e = (-s/2) - 2 * b2 - a2;
    ca = -6 * b2;
    cd = ca - 4 * a2;
    x = 1; // функция ToCenter... сдвигает начало координат в 3 квадрант, поэтому добавляю +1 к "x", чтобы перейти за ось Oy к 1 квадранту
    y = b+1; // аналогично и здесь, чтобы перейти за ось Ox

    QPoint point = ToCenterCoordinateSystem(-x,-y); // А координаты с "-", чтобы отображать их в 1 квадранте
    painter.fillRect(point.x()+1,point.y()+1,9,9,Qt::blue);

    for (int i = 0; i < l; ++i)
    {
        ++x;
        if (e >= 0)
        {
            e += t + ca;
        }
        else
        {
            --y;
            e += t - s + cd;
            s += dds;
        }
        t -= ddt;
        point = ToCenterCoordinateSystem(-x,-y);
        painter.fillRect(point.x()+1,point.y()+1,9,9,Qt::blue);
    }
    l = y-1;
    e -= t/2 + s/2 + b2 + a2;
    ca = -6*a2;
    cd = ca - 4*b2;

    for (int i = 0; i < l; ++i)
    {
        --y;
        if (e <= 0 || x-1 == a)
        {
            e += -s - ca;
        }
        else
        {
            ++x;
            e += t - s + cd;
            t -= ddt;
        }
        s += dds;
        point = ToCenterCoordinateSystem(-x,-y);
        painter.fillRect(point.x()+1,point.y()+1,9,9,Qt::blue);
    }
}

void PictureBox::DrawGrid()
{
    QPainter painter(&m_Grid);
    int w = m_Grid.width();
    int h = m_Grid.height();
    painter.setPen(Qt::green);
    for (int i = 0; i < h; i += 10)
    {
        painter.drawLine(0, i, w, i); // горизонтальная линия
        painter.drawLine(i, 0, i, h); // вертикальная
    }
    painter.setPen(Qt::black);
    painter.drawLine(0, h/2, w, h/2); // ось X
    painter.drawLine(w/2, 0, w/2, h); // ось Y
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawLine(0,0,w,h);
    painter.drawLine(0,h,w,0);

}

void PictureBox::risovanie(DrawObject obj)
{
      m_Pixmap.fill(Qt::white); // очищаю фрейм (заливаю белым цветом)
      QPainter painter(&m_Pixmap);
      painter.setRenderHint(QPainter::Antialiasing, true);
      painter.drawPixmap(0,0,m_Grid); // рисую сетку
      if (obj == DrawObject::Line) DrawDirectLine(DirectLineStartPoint.x(),DirectLineStartPoint.y(),painter);
      else if (obj == DrawObject::Circle) DrawCircle(CircleR, painter);
      else if (obj == DrawObject::Ellipse) DrawEllipse(EllipseAB.x(),EllipseAB.y(), painter);

}

void PictureBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(1,1,m_Pixmap);
}
