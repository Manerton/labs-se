#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *) {

      QPainter painter(this);
      QPolygon polygon;
      painter.setRenderHint(QPainter::Antialiasing, true);
      painter.setPen(QPen(Qt::Dense6Pattern,2));
      for (int i=0; i<=360; i+=20)
      {
          painter.drawLine(0, i, 360, i);
          painter.drawLine(i, 0, i, 360);

      }
      int x0 = 40, y0 = 80; // нач. координаты
      int x = x0, y = y0; // координаты точки

      polygon << QPoint(x,y);
      x += 120;
      polygon << QPoint(x,y);
      y += 40;
      polygon << QPoint(x,y);
      x += 80;
      polygon << QPoint(x,y);
      y -= 80;
      polygon << QPoint(x,y);
      x += 20;
      polygon << QPoint(x,y);
      y += 80;
      polygon << QPoint(x,y);
      x += 20;
      polygon << QPoint(x,y);
      y += 60;
      polygon << QPoint(x,y);
      x += 20;
      polygon << QPoint(x,y);
      y += 20;
      polygon << QPoint(x,y);
      x -= 240;
      polygon << QPoint(x,y);
      y -= 100;
      polygon << QPoint(x,y);
      x -= 20;
      polygon << QPoint(x,y);
      y -= 20;
      polygon << QPoint(x,y);
      painter.setPen(QPen(Qt::black,4));
      painter.drawPolyline(polygon);
      painter.setBrush(Qt::white);
      painter.drawRect(80,100,40,40);
      painter.setBrush(Qt::red);
      x = 230;
      y = 200;
      painter.drawEllipse(QPoint(x,y),13,13);
      x -= 60;
      painter.drawEllipse(QPoint(x,y),13,13);
      x -= 60;
      painter.drawEllipse(QPoint(x,y),13,13);


}

