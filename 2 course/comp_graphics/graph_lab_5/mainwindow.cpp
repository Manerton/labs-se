#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pb_frame = new PictureBox(ui->frame); // определяю фрейм, в котором рисую
}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::getAngle(const double &gradus) const
{
    double PI = acos(-1.0);
    return (gradus * PI)/180;
}

void MainWindow::on_pushButton_draw_clicked()
{
    pb_frame->risovanie();
    pb_frame->repaint();
}

void MainWindow::on_spinBox_A_X_valueChanged(int arg1)
{
    pb_frame->DirectLineStartPoint.setX(arg1);
}

void MainWindow::on_spinBox_A_Y_valueChanged(int arg1)
{
    pb_frame->DirectLineStartPoint.setY(arg1);
}
