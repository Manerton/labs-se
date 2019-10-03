#include "mainwindow.h"
#include "picturebox.h"
using namespace std;

extern QVector<Star> Stars; // указываю, что использую глобальный вектор звезд в этом файле

PictureBox::PictureBox(QWidget *parent) : QFrame(parent)
{
    m_Pixmap = QPixmap(parent->width()-2, parent->height()-2);
    empty = QPixmap(m_Pixmap.size());
}

void PictureBox::risovanie()
{
    if (!trail) {reset();}
    QPainter painter(&m_Pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    int N = Stars.size();
    for (int i = 0; i < N; i++)
    {
        painter.setPen(Stars[i].color);
        painter.setBrush(Stars[i].color);
        painter.drawEllipse(QPointF(Stars[i].x, Stars[i].y),Stars[i].size,Stars[i].size);
    }
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
