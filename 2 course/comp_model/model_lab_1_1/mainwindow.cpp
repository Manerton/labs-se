#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <cmath>
#include <QMessageBox>
#include <QPainter>
#include "telo.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), V0(40), h(20), object{V0,h}
{
    ui->setupUi(this);
    paintTimer = new QTimer(this); // создаю таймер
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(Qt::red);
    painter.setBrush(Qt::red);
    int hh = this->height();
    painter.drawLine(0,int(hh-h*4),width(),int(hh-h*4));
    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    painter.setPen(Qt::black);
    painter.setBrush(Qt::red);
    painter.drawEllipse(int(width())/2,int(hh-ycoord),5,5);
}

void MainWindow::get_new_ycoord()
{
    int N = object.Y.size();
    if (i < N)
    {
        ycoord = object.Y[i]*4;
        i++;
    }
    repaint();
}

void MainWindow::on_pushButton_clicked()
{
    disconnect(paintTimer, SIGNAL(timeout()), this, SLOT(get_new_ycoord()));
    connect(paintTimer, SIGNAL(timeout()), this, SLOT(get_new_ycoord()));
    paintTimer->start(5);
    object = {V0, h};
    i = 0;
    QString result = object.getTime();
    ui->label_info->setText(result);
}

void MainWindow::on_SpinBox_v_valueChanged(double arg1)
{
    V0 = arg1;
}

void MainWindow::on_SpinBox_h_valueChanged(double arg1)
{
    h = arg1;
}
