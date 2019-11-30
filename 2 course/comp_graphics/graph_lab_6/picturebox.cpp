#include <cmath>
#include <QDebug>
#include "picturebox.h"
using namespace std;

PictureBox::PictureBox(QWidget *parent) : QFrame(parent)
{
    resize(parent->width()-2, parent->height()-2); // размер как у родительского виджета, с небольшим отступом в 2 пикселя, чтобы не удалять оранжевую рамку фрейма
    m_Pixmap = QPixmap(size()); // инициализирую полотно, на котором буду рисовать
    QPainter painter(&m_Pixmap);
    img1.load(":/img.jpg");
    img2.load(":/img2.jpg");
    image = img1;
    painter.drawImage(0,0,image);
}

void PictureBox::DrawLens(QPainter &painter)
{
    int kx, ky, r;
    double l;
    r = 150;
    QImage lens = image;
    kx = lens.width()/2;
    ky = lens.height()/2;
    for (int x = 0; x < lens.width(); ++x)
    {
        for (int y = 0; y < lens.height(); ++y)
        {
            int dx = x-kx;
            int dy = y-ky;
            l = sqrt(dx*dx + dy*dy);
            if (l < r)
            {
                double a = atan2(dy,dx);
                double d = l * (r - l) / (a - l);
                l += d/3;
                int Qx = int(kx+l*cos(a));
                int Qy = int(ky+l*sin(a));

                QRgb pixel = image.pixel(Qx,Qy);
                lens.setPixel(x,y,pixel);
            }
        }
    }
    painter.drawImage(0,0,lens);
}

void PictureBox::DrawLens2(QPainter &painter)
{
    int kx, ky, r;
    double l;
    r = 150;
    QImage lens = image;
    kx = lens.width()/2;
    ky = lens.height()/2;
    for (int x = 0; x < lens.width(); ++x)
    {
        for (int y = 0; y < lens.height(); ++y)
        {
            int dx = x-kx;
            int dy = y-ky;
            l = sqrt(dx*dx + dy*dy);
            if (l < r)
            {
                double a = atan2(dy,dx);
                double d = l * 50 / (a - l);
                l += d/2;
                int Qx = int(kx+l*cos(a));
                int Qy = int(ky+l*sin(a));

                QRgb pixel = image.pixel(Qx,Qy);
                lens.setPixel(x,y,pixel);
            }
        }
    }
    painter.drawImage(0,0,lens);
}

void PictureBox::disable()
{
    m_Pixmap.fill(Qt::white);
    QPainter painter(&m_Pixmap);
    if (Pic == PicNumber::First) image = img1;
    else if (Pic == PicNumber::Second) image = img2;
    painter.drawImage(0,0,image);
}

void PictureBox::risovanie()
{
    QPainter painter(&m_Pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    if (Lens == LensNumber::First) DrawLens(painter);
    else if (Lens == LensNumber::Second) DrawLens2(painter);
}

void PictureBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(1,1,m_Pixmap);
}
