#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pb_frame = new PictureBox(ui->frame); // определяю фрейм, в котором рисую
    pb_frame->resize(ui->frame->size()); // указываю размер как у фрейма с оранжевой рамкой
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

Triangle MainWindow::ToCenterCoordinateSystem(const Triangle &Tri) const
{
    double width = pb_frame->width();
    double height = pb_frame->height();
    QPointF A = {Tri.A.x() + width/2, height/2 - Tri.A.y()}; // смещаю к центру фрейма
    QPointF B = {Tri.B.x() + width/2, height/2 - Tri.B.y()};
    QPointF C = {Tri.C.x() + width/2, height/2 - Tri.C.y()};
    return Triangle{A,B,C};
}

void MainWindow::RotateTriangle(double angle) // поворачиваю через аффиные преобразования
{
    angle = getAngle(angle);
    double newX = (Tri.A.x() - Tri.B.x()) * cos(angle) - (Tri.A.y() - Tri.B.y()) * sin(angle) + Tri.B.x();
    double newY = (Tri.A.x() - Tri.B.x()) * sin(angle) + (Tri.A.y() - Tri.B.y()) * cos(angle) + Tri.B.y();
    Tri.A = {newX, newY};

    newX = (Tri.C.x() - Tri.B.x()) * cos(angle) - (Tri.C.y() - Tri.B.y()) * sin(angle) + Tri.B.x();
    newY = (Tri.C.x() - Tri.B.x()) * sin(angle) + (Tri.C.y() - Tri.B.y()) * cos(angle) + Tri.B.y();
    Tri.C = {newX, newY};
}

void MainWindow::on_pushButton_draw_clicked()
{
    QPointF A = {ui->doubleSpinBox_A_X->value(), ui->doubleSpinBox_A_Y->value()};
    QPointF B = {ui->doubleSpinBox_B_X->value(), ui->doubleSpinBox_B_Y->value()};
    QPointF C = {ui->doubleSpinBox_C_X->value(), ui->doubleSpinBox_C_Y->value()};
    Tri = {A,B,C};
    pb_frame->risovanie(ToCenterCoordinateSystem(Tri));
    pb_frame->repaint();
}
