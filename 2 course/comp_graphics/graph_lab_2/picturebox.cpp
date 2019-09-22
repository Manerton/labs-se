#include "picturebox.h"
#include "mainwindow.h"
#include <QPicture>
using namespace std;

extern QVector<Star> Stars; // указываю, что использую глобальный вектор звезд в этом файле

PictureBox::PictureBox(QWidget *parent) : QFrame(parent)
{
    m_Pixmap = QPixmap(parent->width()-2, parent->height()-2);
}

void PictureBox::risovanie()
{
    QPaintDevice* pointer = &m_Picture;
    if (trail) pointer = &m_Pixmap;

    QPainter painter(pointer);

    QColor(0, 0, 0, 0);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    int N = Stars.size();
    for (int i = 0; i < N; i++)
    {
        painter.setPen(Stars[i].color);
        painter.setBrush(Stars[i].color);
        painter.drawEllipse(static_cast<int>(Stars[i].x),static_cast<int>(Stars[i].y),2,2);
    }
}

void PictureBox::reset()
{
    QPixmap m_new = QPixmap(m_Pixmap.size());
    m_Pixmap.swap(m_new);
}

void PictureBox::paintEvent(QPaintEvent *)
{

    QPainter painter(this);
    if (trail) {painter.drawPixmap(1,1,m_Pixmap);}

    else {painter.drawPicture(0,0,m_Picture); reset();}

}
