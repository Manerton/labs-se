#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QMessageBox>
#include <QPainter>
#include "animal.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), v(10), l(0.2), crocodile{v,l}
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(Qt::yellow);
    painter.setPen(Qt::black);

    for (int i = 150; i < height(); i += 150)
    {
        painter.drawLine(0,i,width(),i);
    }
    for (int i = 150; i < width(); i += 150)
    {
        painter.drawLine(i,0,i,height());
    }
    painter.translate(width()/2, height()/2+100); // смещаю центр координат в центр


    for (int i = 0; i < 1000; ++i)
    {
        crocodile.work(8,15);
        auto N = crocodile.coord.size();

        painter.drawEllipse(crocodile.coord[N-1],3,3);

    }
    painter.setBrush(Qt::blue);
    painter.setPen(Qt::black);
    painter.drawEllipse(0,0,10,10);
}

void MainWindow::on_pushButton_clicked()
{
    crocodile = animal(v,l);
    repaint();
}

void MainWindow::on_SpinBox_v_valueChanged(double arg1)
{
    v = arg1;
    on_pushButton_clicked();
}

void MainWindow::on_SpinBox_l_valueChanged(double arg1)
{
    l = arg1;
    on_pushButton_clicked();
}
