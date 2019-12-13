#include <cmath>
#include <QDebug>
#include <QtTest/QTest>
#include <algorithm>
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

void PictureBox::DrawDirectLine(QPoint q1, QPoint q2, QColor color, QPainter &painter, bool write_points)    // -- рисование отрезков через алгоритм Брезенхема -- //
{
    QVector<QPoint> intersections_points;
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
    intersections_points.push_back(QPoint(x1*10,y1*10));
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
            intersections_points.push_back(QPoint(x1*10,y1*10));
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
            intersections_points.push_back(QPoint(x1*10,y1*10));
        }
    }

    if (write_points) intersections.append(intersections_points);
}
void PictureBox::mousePressEvent(QMouseEvent *event)
{
    QPoint p = event->pos(); // -- получаю координаты -- //
    p.rx() /= 10;   // -- округляю их вниз (например 347;284 -> 340;280)
    p.ry() /= 10;
    p *= 10;
    vertex.push_back(p);
    QPainter painter(&m_Pixmap);
    if (event->button() == Qt::LeftButton)
    {
        if (vertex.size() > 1)
        {
            DrawDirectLine(vertex[vertex.size()-2], vertex[vertex.size()-1],Qt::blue,painter,true);
        } else
        {
            painter.fillRect(p.x()+1,p.y()+1,9,9,Qt::blue);
        }
    }
    if (event->button() == Qt::RightButton)
    {
       painter.fillRect(p.x()+1,p.y()+1,9,9,Qt::blue);
    }
    repaint();
}

void PictureBox::sort_intersections()
{
    // -- сортируем сверху вниз -- //
    std::sort(intersections.begin(),intersections.end(),
         [](const QPoint& l, const QPoint& r) {  // -- лямбда-функция - критерий -- //
            if (l.y() == r.y())
                return (l.x() <= r.x());
            return (l.y() < r.y());
         });
}

void PictureBox::find_local_min_max() // -- ищем точку с минимальным и максимальным y -- //
{                                                           // -- иначе говоря, ищем локальные минимум и максимум фигуры -- //
    auto minmax = minmax_element(vertex.begin(),vertex.end(),
                            [](const QPoint& l, const QPoint& r) {  // -- лямбда-функция - критерий -- //
                                return l.y() < r.y();
                            });
    min_y = minmax.first->y();
    max_y = minmax.second->y();
}

void PictureBox::fill_figure(QPainter &painter)
{
    int w = width();

    auto N = intersections.size() - 1;
    for (auto i = 0; i < N; ++i) // -- пройдемся по вектору с точками пересечений ребер и сканирующих строк -- //
    {
        QColor color = Qt::gray; // -- изначально цвет фона -- //
        const int x = intersections[i].x();
        const int y = intersections[i].y();
        DrawDirectLine(QPoint(0,y), QPoint(x-10,y),color,painter);  // -- заливаем слева от фигуры -- //
        while (intersections[i].y() == intersections[i+1].y() && i < N)
        {
            if (abs(intersections[i].x() - intersections[i+1].x()) > 10)    // -- если между двумя пересечениями есть пространство -- //
            {
                if (intersections[i-1].y() == intersections[i].y())
                {
                     if ((std::find(vertex.begin(),vertex.end(),intersections[i]) != vertex.end()))
                     {
                         if (color == Qt::gray) color = Qt::green;
                         else color = Qt::gray;
                     }
                }
                if (color == Qt::gray) color = Qt::green;
                else color = Qt::gray;
                int x = intersections[i].x() + 10;
                int y = intersections[i].y();
                int x2 = intersections[i+1].x() - 10;
                int y2 = intersections[i+1].y();
                DrawDirectLine(QPoint(x,y), QPoint(x2,y2),color,painter);
            }
            ++i;
        }
        DrawDirectLine(QPoint(intersections[i].x()+10,y), QPoint(w,y),Qt::gray,painter);
        QTest::qWait(100);
        update();
    }
}

void PictureBox::fill() // -- заливаем фигуру -- //
{
    QPainter painter(&m_Pixmap);
    int h = height();
    int w = width();
    QImage image = m_Pixmap.toImage(); // -- чтобы получить доступ к пикселям -- //
    min_y = h; // -- по умолчанию, если фигура не задана - зальем фоном фрейм (т.е заливаем до конца окна по 'y') -- //
    if (!vertex.empty()) // -- если фигура нарисована, то заливаем до первой точки многоугольника -- //
    {
        find_local_min_max();
    }
    // -- заливаю фон до фигуры -- //
    for (int y = 0; y < min_y; y += 10)
    {
        DrawDirectLine(QPoint(0,y),QPoint(w,y),Qt::gray,painter);
        update();
        QTest::qWait(100);  // -- для задержки, вообще, лучше было бы через таймер, но тогда необходимо менять логику программы -- //
    }
    fill_figure(painter); // -- заливка фигуры -- //

    // -- заливаю фон после фигуры -- //
    for (int y = max_y+10; y < h; y += 10)
    {
        DrawDirectLine(QPoint(0,y),QPoint(w,y),Qt::gray,painter);
        update();
        QTest::qWait(100);  // -- для задержки, вообще, лучше было бы через таймер, но тогда необходимо менять логику программы -- //
    }
}

void PictureBox::risovanie()
{
    sort_intersections();   // -- сортирую вектор пересечений ребер и сканирующих строк -- //
    auto last = unique(intersections.begin()+1,intersections.end()-1); // -- удаляю дубликаты -- //
    intersections.erase(last,intersections.end()-1);
    fill();
}

void PictureBox::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(1,1,m_Pixmap);
}
