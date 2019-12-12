#include <cmath>
#include <QDebug>
#include <QTimer>
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

void PictureBox::DrawDirectLine(QPoint q1, QPoint q2, QColor color, QPainter &painter)    // -- рисование отрезков через алгоритм Брезенхема -- //
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
    painter.fillRect(x1*10+1,y1*10+1,9,9,color);
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
            painter.fillRect(x1*10+1,y1*10+1,9,9,color);
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
            painter.fillRect(x1*10+1,y1*10+1,9,9,color);
        }
    }

}

void PictureBox::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint p = event->pos(); // -- получаю координаты -- //
        p.rx() /= 10;   // -- округляю их вниз (например 347;284 -> 340;280)
        p.ry() /= 10;
        p *= 10;
        vertex.push_back(p);
        QPainter painter(&m_Pixmap);
        if (vertex.size() > 1)
        {
            DrawDirectLine(vertex[vertex.size()-2], vertex[vertex.size()-1],Qt::blue,painter);
        } else
        {
            painter.fillRect(p.x()+1,p.y()+1,9,9,Qt::blue);
        }
        repaint();
    }
}

void PictureBox::find_intersections()
{
    int h = height();
    int w = width();
    arr_scanlines.reserve(50); // -- 50 сканирующих линий через каждые 10 пикселей -- //
    QImage image = m_Pixmap.toImage(); // -- чтобы получить доступ к пикселям -- //
    for (int y = 5; y < h; y += 10)
    {
        scanline line;
        for (int x = 0; x < w; x += 10)
        {
            if( image.pixelColor(x+1,y+1) == Qt::blue ) { line.push_back(QPoint(x,y));}
        }
        if (!line.empty()) arr_scanlines.push_back(line);
    }
}

void PictureBox::fill()
{
    QPainter painter(&m_Pixmap);
    QTimer paintTimer;
    int first_y = height();
    if (!arr_scanlines.empty()) first_y = arr_scanlines[0][0].y();
    for (int y = 0; y < first_y-10; y += 10)
    {
        paintTimer.start(100);
        while (paintTimer.remainingTime() != 0)
        {

        }
        DrawDirectLine(QPoint(0,y),QPoint(width(),y),Qt::gray,painter);
        repaint();
    }
}



void PictureBox::risovanie()
{
      //m_Pixmap.fill(Qt::white); // очищаю фрейм (заливаю белым цветом)
      //QPainter painter(&m_Pixmap);
      //painter.setRenderHint(QPainter::Antialiasing, true);
      find_intersections();
      fill();
      //painter.drawPixmap(0,0,m_Grid); // рисую сетку
}

void PictureBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(1,1,m_Pixmap);
}
