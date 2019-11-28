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
    pb_frame->risovanie(obj);
    pb_frame->repaint();
}

void MainWindow::on_spinBox_X_valueChanged(int arg1)
{
    pb_frame->DirectLineStartPoint.setX(arg1);
}

void MainWindow::on_spinBox_Y_valueChanged(int arg1)
{
    pb_frame->DirectLineStartPoint.setY(arg1);
}

void MainWindow::on_spinBox_a_valueChanged(int arg1)
{
    pb_frame->EllipseAB.setX(arg1);
}

void MainWindow::on_spinBox_b_valueChanged(int arg1)
{
    pb_frame->EllipseAB.setY(arg1);
}

void MainWindow::on_spinBox_R_valueChanged(int arg1)
{
    pb_frame->CircleR = arg1;
}

void MainWindow::disableSpinBoxes()
{

    ui->spinBox_R->setDisabled(true);
    ui->spinBox_a->setDisabled(true);
    ui->spinBox_b->setDisabled(true);
    ui->spinBox_X->setDisabled(true);
    ui->spinBox_Y->setDisabled(true);
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    disableSpinBoxes();
    if (arg1 == "Прямая")
    {
        ui->spinBox_X->setEnabled(true);
        ui->spinBox_Y->setEnabled(true);
        obj = DrawObject::Line;
    } else if (arg1 == "Эллипс")
    {
        ui->spinBox_a->setEnabled(true);
        ui->spinBox_b->setEnabled(true);
        obj = DrawObject::Ellipse;
    } else if (arg1 == "Окружность")
    {
        ui->spinBox_R->setEnabled(true);
        obj = DrawObject::Circle;
    }
}





