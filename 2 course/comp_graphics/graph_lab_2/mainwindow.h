#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "picturebox.h" // фрейм, в котором все рисуем
#include <QTimer> // таймер, для кнопки пауза / продолжить

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Star // звезда
{
    double x, y; // координаты
    QColor color; // цвет
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int speed = 1; // начальная скорость
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void RandomizeStars(int N = 500); // функция, генерирующая рандомные координаты звезд, по умолчанию 500 звезд
    double newY_Coord(double x1, double y1, double x); // новые координаты звезд при смещении в центр
    QTimer *paintTimer; // таймер
    void setSpeed(int k); // метод для установки скорости
private slots:
    void ToCentre();
    void FromCentre();
    void on_horizontalSlider_valueChanged(int value);
    void on_pushButton_3_clicked();
    void on_checkBox_toggled(bool checked);

    void on_pushButton_start_clicked();

    void on_pushButton_pause_clicked();

private:
    Ui::MainWindow *ui;
    PictureBox *pb_frame; // указатель на фрейм
};


#endif // MAINWINDOW_H
