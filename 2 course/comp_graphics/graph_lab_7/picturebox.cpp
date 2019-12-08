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
}

void PictureBox::DrawDirectLine(QPoint q1, QPoint q2, QPainter &painter)
{
    int x1, y1,x2,y2, Px, Py, E;
    x1 = q1.x() / 10;
    y1 = q1.y() / 10;
    x2 = q2.x() / 10;
    y2 = q2.y() / 10;

    Px = abs(x2 - x1);
    Py = abs(y2 - y1);

    bool signx = x1 < x2;
    bool signy = y2 > y1;

    E = 0;
    painter.fillRect(x1*10+1,y1*10+1,9,9,Qt::blue);
    if (Px >= Py)
    {
        E = 2 * Py - Px;
        for (int i = 0; i < Px; ++i)
        {
            if (signx) ++x1;
            else --x1;
            if (E >= 0)
            {
                if (signy) ++y1;
                else --y1;
                E += 2 * (Py - Px);
            }
            else E += 2 * Py;
            painter.fillRect(x1*10+1,y1*10+1,9,9,Qt::blue);
        }
    }
    else
    {
        E = 2 * Px - Py;
        for (int i = 0; i < Py; ++i)
        {
            if (signy) ++y1;
            else --y1;
            if (E >= 0)
            {
                if (signx) ++x1;
                else --x1;
                E += 2 * (Px - Py);
            }
            else E += 2 * Px;
            painter.fillRect(x1*10+1,y1*10+1,9,9,Qt::blue);
        }
    }


}

void PictureBox::mousePressEvent(QMouseEvent *event)
{
    QPoint p = event->pos();
    p.rx() /= 10;
    p.ry() /= 10;
    p *= 10;
    vertex.push_back(p);
    QPainter painter(&m_Pixmap);
    if (vertex.size() > 1)
    {
        DrawDirectLine(vertex[vertex.size()-2], vertex[vertex.size()-1],painter);
    } else
    {
        painter.fillRect(p.x()+1,p.y()+1,9,9,Qt::blue);
    }
    repaint();

}

void PictureBox::risovanie()
{
      m_Pixmap.fill(Qt::white); // очищаю фрейм (заливаю белым цветом)
      QPainter painter(&m_Pixmap);
      painter.setRenderHint(QPainter::Antialiasing, true);
      painter.drawPixmap(0,0,m_Grid); // рисую сетку
}

void PictureBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(1,1,m_Pixmap);
}
