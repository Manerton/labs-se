#ifndef PICTUREBOX_H
#define PICTUREBOX_H

#include <QFrame>
#include <vector>
#include <QPainter> // для пейнт евента
#include <stack>
#include <QMouseEvent>

class PictureBox : public QFrame
{
    Q_OBJECT
    QPixmap m_Pixmap; // для рисования
    void mousePressEvent ( QMouseEvent * event );
    void mouseMoveEvent ( QMouseEvent * event );

    int xmin = 100, ymin = 100, xmax = 300, ymax = 300; // -- размеры отсекающего окна -- //
    int x0 = 50,y0 = 50,x1 = 350, y1 = 350;    // -- координаты отрезка -- //
public:
    explicit PictureBox(QWidget *parent = nullptr);
    void liang_barsky_clipper(int xmin, int xmax, int ymin, int ymax);    // -- демонстрация отсечения произвольным окном, алгоритм Лианга-Барски (Liang-Barsky) -- //
protected:
    virtual void paintEvent(QPaintEvent*); // функция перерисовки, отрисовывает готовый пиксмап из ф-ции risovanie
};
#endif // PICTUREBOX_H

