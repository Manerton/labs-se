#include "picturebox.h"

picturebox::picturebox(QWidget *parent) : QFrame(parent)
{

}
void picturebox::paintEvent(QPaintEvent *) {
    int x, y; // левый верхний угол полосы
    int w, h; // ширина и высота полосы

    int x0, y0; // левый верхний угол флага
    x0 = 10;
    y0 = 10;
    w = 28;
    h = 60;

    x = x0;
    y = y0;
    QPainter painter(this);
      // зеленая полоса
      painter.fillRect(x,y,w,h, Qt::darkGreen);

      x += w;
      painter.fillRect(x,y,w,h, Qt::white);

      x += w;
      painter.fillRect(x,y,w,h, Qt::red);

      // контур
      painter.drawRect(x0,y0, w*3, h);
      // Подпись.
      painter.drawText(x0, y0 + h + 20, "Италия");

}
