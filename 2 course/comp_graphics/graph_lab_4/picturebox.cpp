#include "picturebox.h"
#include <cmath>
using namespace std;

PictureBox::PictureBox(QWidget *parent) : QFrame(parent)
{
    m_Pixmap = QPixmap(parent->width()-2, parent->height()-2);
    empty = QPixmap(m_Pixmap.size());
}

void PictureBox::DrawTriangle(const Triangle &Tri, QPainter &painter)
{
    painter.setPen(Qt::black);
    QVector<QPointF> points = {Tri.A, Tri.B, Tri.C};
    painter.drawText(int(Tri.A.x()+3),int(Tri.A.y()+3), "A"); // буква A
    painter.drawText(int(Tri.B.x()+3),int(Tri.B.y()+3), "B"); // буква B
    painter.drawText(int(Tri.C.x()+3),int(Tri.C.y()+3), "C"); // буква C
    painter.drawPolygon(points);
}

void PictureBox::Drawline_D(int d)
{
    QPainter painter(&m_Pixmap);
    painter.setPen(Qt::black);
    painter.drawLine(0,d,m_Pixmap.width(),d);
    painter.drawText((m_Pixmap.width()-10),d, "d"); // буква d
}

void PictureBox::DrawAxes(int width, int height, QPainter &painter)
{
    painter.setPen(Qt::blue);
    painter.drawLine(0, height/2, width, height/2); // ось X
    painter.drawText(width-10, height/2-2, "X"); // буква X
    painter.drawLine(width/2, 0, width/2, height); // ось Y
    painter.drawText(width/2-7, 10, "Y"); // буква Y
}

void PictureBox::risovanie(const Triangle &Tri)
{
    reset();
    QPainter painter(&m_Pixmap);
    painter.eraseRect(0,0,m_Pixmap.width(),m_Pixmap.height()); // закрасим её белым цветом
    DrawAxes(m_Pixmap.width(),m_Pixmap.height(),painter);
    painter.setRenderHint(QPainter::Antialiasing, true);
    DrawTriangle(Tri,painter);
}

void PictureBox::reset()
{
    m_Pixmap = empty;
}

void PictureBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(1,1,m_Pixmap);
}
