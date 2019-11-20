#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "picturebox.h" // фрейм, в котором все рисуем

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    PictureBox *pb_frame; // указатель на фрейм
    Triangle Tri; // Треугольник описан в picturebox.h
    int d; // прямая y = d
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_pushButton_rotate_clicked(); // поворот
    void on_pushButton_compress_clicked(); // сжатие
    void on_pushButton_draw_clicked(); // нарисовать
    void on_pushButton_mirror_clicked(); // отразить относительно d
private:
    Triangle ToCenterCoordinateSystem(const Triangle &Tri) const; // преобразовать координаты к системе координат, находящейся в центре
    double getAngle (const double &gradus) const; // получить радианы из градусов
    void RotateTriangle(double angle); // повернуть треугольник
    void CompressTriangle(double value); // сжать
    void ReflectTriangle(double d); // отразить
};


#endif // MAINWINDOW_H
