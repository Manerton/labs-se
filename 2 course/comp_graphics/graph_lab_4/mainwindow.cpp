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

void MainWindow::CompressTriangle(double value) // умножаю координаты x и y каждой точки на value
{
    Tri.A *= value;
    Tri.B *= value;
    Tri.C *= value;
}

void MainWindow::ReflectTriangle(double d) // сначала отражаю относительно оси y, потом смещаю точки так, чтобы было отражение относительно прямой d
{
    if (d < pb_frame->height()/2)
    {
        Tri.A.setY(Tri.A.y() * -1 + 2*fabs(pb_frame->height()/2 - d));
        Tri.B.setY(Tri.B.y() * -1 + 2*fabs(pb_frame->height()/2 - d));
        Tri.C.setY(Tri.C.y() * -1 + 2*fabs(pb_frame->height()/2 - d));
    } else {
        Tri.A.setY(Tri.A.y() * -1 - 2*fabs(pb_frame->height()/2 - d));
        Tri.B.setY(Tri.B.y() * -1 - 2*fabs(pb_frame->height()/2 - d));
        Tri.C.setY(Tri.C.y() * -1 - 2*fabs(pb_frame->height()/2 - d));
    }

}

void MainWindow::on_pushButton_rotate_clicked()
{
    RotateTriangle(-90); // поворачиваю на угол -пи / 2
    pb_frame->risovanie(ToCenterCoordinateSystem(Tri)); // отрисовываю преобразовав координаты к новой системе координат (центр фрейма)
    pb_frame->repaint();
}

void MainWindow::on_pushButton_compress_clicked()
{
    CompressTriangle(0.5); // сжимаю на 0.5
    pb_frame->risovanie(ToCenterCoordinateSystem(Tri));
    pb_frame->repaint();
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

void MainWindow::on_pushButton_mirror_clicked()
{
    if (ui->spinBox_d->value() != d) ui->pushButton_draw->click(); // если изменили значение d и нажали на кнопку "отразить" снова, то перерисовываем треугольник с оригинальными координатами
    d = ui->spinBox_d->value();
    if (d > 0) ReflectTriangle(d);
    pb_frame->risovanie(ToCenterCoordinateSystem(Tri));
    pb_frame->Drawline_D(d);
    pb_frame->repaint();
}
