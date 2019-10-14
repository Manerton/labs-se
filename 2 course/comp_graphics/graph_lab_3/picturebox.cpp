#include "picturebox.h"
using namespace std;

PictureBox::PictureBox(QWidget *parent) : QFrame(parent)
{
    m_Pixmap = QPixmap(parent->width()-2, parent->height()-2);
}

void PictureBox::drawH(int size, coord P, QPainter *paint) // в данном методе мы просто рисуем три линии, которые и являются буквой Н
{
    paint->drawLine(P.x-(size/2), P.y, P.x+(size/2), P.y);
    paint->drawLine(P.x-(size/2), P.y-(size/2), P.x-(size/2), P.y+(size/2));
    paint->drawLine(P.x+(size/2), P.y-(size/2), P.x+(size/2), P.y+(size/2));
}

void PictureBox::drawHDepth(const QVector<coord> &coords)
{
    QPainter painter(&m_Pixmap);
    painter.eraseRect(0,0,m_Pixmap.width(),m_Pixmap.height()); // закрасим её белым цветом
    int coords_size = coords.size();
    for (int i=0; i < coords_size; ++i)
    {
        painter.setPen(QPen(coords[i].color,1)); // задаем кисть рандомного цвета толщиной 1
        drawH(coords[i].size,coords[i],&painter); // и рисуем
    }
}


void PictureBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(1,1,m_Pixmap);
}
