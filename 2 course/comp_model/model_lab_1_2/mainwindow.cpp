#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "telo.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), V0(20), a(45), object{V0,a}
{
    ui->setupUi(this);
    object.work();
    repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    int N = object.X.size();
    for (int i = 0; i < N; i++)
    {
        int h = this->height();
        double Ycoord = object.Y[i];
        painter.setPen(Qt::green);
        painter.setBrush(Qt::green);
        painter.drawEllipse(int(object.X[i]),int(h-Ycoord),2,2);
    }
}

void MainWindow::on_pushButton_clicked()
{
    object = {V0, a};
    object.work();
    int N = object.X.size();
    int new_w = int(object.X[N-1]) + 20;
    int new_h = int(object.Y[N-1]) + 20;
    if (new_w > width() && new_h > height()) resize(new_w,new_h);
    if (new_w > width()) resize(new_w,height());
    if (new_h > height()) resize(width(),new_h);
    repaint();
}

void MainWindow::on_spinBox_v_valueChanged(int arg1)
{
    V0 = arg1;
}

void MainWindow::on_spinBox_a_valueChanged(int arg1)
{
    a = arg1;
}
