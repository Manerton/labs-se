#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer> // таймер
#include "picturebox.h" // фрейм, в котором все рисуем

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


struct Star // звезда
{

    double x, y; // текущие координаты
    double dx, dy; // шаги изменения координат при схождении
    double size; // размер
    QColor color; // цвет
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void RandomizeStars(); // функция, генерирующая рандомные координаты звезд, по умолчанию 500 звезд
    double newY_Coord(double x1, double y1, double x); // новые y-координаты звезд при смещении в центр
    QTimer *paintTimer; // таймер
    void setSpeed(int k); // метод для установки скорости
private slots:
    void ToCentre(); // схождение звезд
    void FromCentre(); // расхождение
    void on_speedSlider_valueChanged(int value);
    void on_checkBox_trail_toggled(bool checked);
    void on_pushButton_tocentre_clicked();
    void on_pushButton_fromcentre_clicked();
    void on_pushButton_pause_clicked();
    void on_pushButton_create_clicked();
    void on_doubleSpinBox_minsize_valueChanged(double arg1);
    void on_doubleSpinBox_maxsize_valueChanged(double arg1);

    void on_spinBox_count_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    PictureBox *pb_frame; // указатель на фрейм
    int count = 500; // количество звезд
    int speed = 1; // начальная скорость
    double min_size = 1;
    double max_size = 1;
};


#endif // MAINWINDOW_H
