#include "picturebox.h"
#include "mainwindow.h"
#include <cmath>
#include <QDebug>
#include <array>
#include <algorithm>
using namespace std;

PictureBox::PictureBox(QWidget *parent) : QFrame(parent)
{
    resize(parent->width()-2, parent->height()-2); // размер как у родительского виджета, с небольшим отступом в 2 пикселя, чтобы не удалять оранжевую рамку фрейма
    m_Pixmap = QPixmap(size()); // инициализирую полотно, на котором буду рисовать
    m_Pixmap.fill(Qt::white); // заливаю белым цветом
}

void PictureBox::liang_barsky_clipper(int xmin, int xmax, int ymin, int ymax)
{
    QPainter painter(&m_Pixmap);
    m_Pixmap.fill(Qt::white);

    // -- при рисовании окна и линии мышкой, может возникнуть такое, что координаты не отсортированы по возрастанию -- //
    if (xmin > xmax) swap(xmin,xmax);
    if (ymin > ymax) swap(ymin,ymax);
    painter.setRenderHint(QPainter::Antialiasing, true); // -- включим сглаживание -- //

    int h = height();

    // -- объявление переменных для неравенств -- //
    double p1 = -(x1 - x0);  // -delta (x)
    double p2 = -p1;         // delta(x)
    double p3 = -(y1 - y0);  // -delta(y)
    double p4 = -p3;         // delta(y)

    double q1 = x0 - xmin;   // -- левая граница окна -- //
    double q2 = xmax - x0;   // -- правая граница окна -- //
    double q3 = y0 - ymin;   // -- нижняя граница окна -- //
    double q4 = ymax - y0;   // -- верхняя граница окна -- //

    // -- рисование отсекающего окна -- //
    painter.drawRect(xmin, h-ymax, xmax-xmin, ymax-ymin);

    // -- если p[i] равен нулю, значит линия параллельна границе окна -- //
    if (int(p1) == 0 || int(p3) == 0)
    {
        painter.drawText(QPoint{10,10},"Линия параллельна границе окна!");
    }
    // -- нужно для каждой границы окна посчитать u, где u = q[i] / p[i] -- //
    // -- для границ, у которых p[i] < 0, нужно сохранить u в массив negarr -- //
    // -- для границ, у которых p[i] > 0, нужно сохранить u в массив posarr -- //
    // -- далее для новой первой точки xn0 нам нужно отобрать наибольший элемент из negarr (в котором помимо u лежит 0) - это параметр для точки xn0} -- //
    // -- далее для новой второй точки xn1 нам нужно отобрать наибольший элемент из posarr (в котором помимо u лежит 1)- это параметр для точки xn0} -- //
    array<double,3> posarr, negarr;
    posarr[0] = 1;  // -- кладем в массив 1 -- //
    negarr[0] = 0;  // -- сюда ноль -- //
    // -- иначе деление на ноль -- //
    if (int(p1) != 0)
    {
        double u1 = q1 / p1;
        double u2 = q2 / p2;
        if (p1 < 0)
        {
            negarr[1] = u1; // p1 < 0, поэтому помещаем u1 в negarr
            posarr[1] = u2; // так как p2 это -p1, то p2 > 0 и поэтому помещаем u2 в posarr
        }
        else
        {
            negarr[1] = u2; // тоже самое
            posarr[1] = u1;
        }
    }
    // проделываем тоже самое, уже для p3 и p4
    if (int(p3) != 0)
    {
        double u3 = q3 / p3;
        double u4 = q4 / p4;
        if (p3 < 0)
        {
            negarr[2] = u3;
            posarr[2] = u4;
        }
        else
        {
            negarr[2] = u4;
            posarr[2] = u3;
        }
    }

    double xn0, yn0, xn1, yn1;  // -- получаем координаты нового отрезка (который внутри окна) -- //
    double rn0, rn1;    // -- параметры t для параметрической формы отрезка внутри окна -- //
    rn0 = *max_element(negarr.begin(),negarr.end()); // Максимум отрицательного массива
    rn1 = *min_element(posarr.begin(),posarr.end()); // Минимум положительного массива

    if (rn0 > rn1)
    {
        // -- линия находится вне рассматриваемой области и не должна быть отображена -- //
        painter.drawText(QPoint{10,10},"Линия находится вне рассматриваемой области!");
    }
    else
    {
        // -- вычисляем через параметрическую форму координаты -- //
        xn0 = x0 + p2 * rn0;
        yn0 = y0 + p4 * rn0;

        xn1 = x0 + p2 * rn1;
        yn1 = y0 + p4 * rn1;

        painter.setPen(QPen(Qt::blue,3));
        painter.drawLine(int(round(xn0)), int(h-round(yn0)), int(round(xn1)), int(h-round(yn1)));

        painter.setPen(QPen(Qt::green,3,Qt::DashLine));
        painter.drawLine(int(round(x0)), int(h-round(y0)), int(round(xn0)), int(h-round(yn0)));
        painter.drawLine(int(round(x1)), int(h-round(y1)), int(round(xn1)), int(h-round(yn1)));
    }
}

void PictureBox::mousePressEvent(QMouseEvent *event)
{
    QPoint p = event->pos(); // -- получаю координаты -- //
    if (event->button() == Qt::LeftButton)
    {
        x0 = p.x();
        y0 = height() - p.y();
        x1 = x0;
        y1 = y0;
    }
    else if (event->button() == Qt::RightButton)
    {
        xmin = p.x();
        ymin = height() - p.y();
        xmax = xmin;
        ymax = ymin;
    }
    liang_barsky_clipper(xmin,xmax,ymin,ymax);
    repaint();
}

void PictureBox::mouseMoveEvent(QMouseEvent *event)
{
    QPoint p = event->pos(); // -- получаю координаты -- //
    if (event->buttons() == Qt::LeftButton)
    {
        x1 = p.x();
        y1 = height() - p.y();
    }
    else if (event->buttons() == Qt::RightButton)
    {
        xmax = p.x();
        ymin = height() - p.y();
    }
    liang_barsky_clipper(xmin,xmax,ymin,ymax);

    repaint();
}

void PictureBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(1,1,m_Pixmap);
}
