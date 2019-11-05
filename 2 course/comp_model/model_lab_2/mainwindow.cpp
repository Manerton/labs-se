#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <cmath>
#include <QMessageBox>
#include <QPainter>
#include "telo.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), l(2), h(2), object{l,h}
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

    int N = object.X.size();
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::yellow);
    int hh = this->height();
    painter.drawEllipse(QPointF(l*50,hh-h*50),5,5);
    painter.setPen(Qt::red);
    painter.setBrush(Qt::red);
    for (int i = 0; i < N; i++)
    {
        double Ycoord = object.Y[i] * 50;
        double Xcoord = object.X[i]*50;
        painter.drawEllipse(int(Xcoord),int(hh-Ycoord),3,3);
    }
}

void MainWindow::on_pushButton_clicked()
{
    object = {l, h};
    QString result = object.work(50);
    QString result2 = object.work(80);
    QString result3 = object.work(10);

    ui->label_info->setText("Под углом 50 градусов: " + result + "\n" + "Под углом 80 градусов: " + result2 + "\n" + "Под углом 10 градусов: " + result3);

    repaint();
}

void MainWindow::on_SpinBox_l_valueChanged(double arg1)
{
    l = arg1;
    on_pushButton_clicked();
}

void MainWindow::on_SpinBox_h_valueChanged(double arg1)
{
    h = arg1;
    on_pushButton_clicked();
}
