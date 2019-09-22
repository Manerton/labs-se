#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <random>
#include <QTimer>
#include <QMessageBox>

using namespace std;

extern QVector<Star> Stars; // объявляю глобальный вектор звезд
QVector<Star> Stars; // пока без размера

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pb_frame = new PictureBox(ui->frame); // определяю фрейм, в котором рисую
    pb_frame->resize(ui->frame->size()); // указываю размер как у фрейма с оранжевой рамкой
    RandomizeStars(500); // получаю координаты звезд, а пейнт евент сам рисует
    pb_frame->risovanie();
    paintTimer = new QTimer(this);
    connect(paintTimer, SIGNAL(timeout()), this, SLOT(ToCentre()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RandomizeStars(int N)
{
    time_t seed = time(nullptr);
    mt19937 mt(seed);
    uniform_int_distribution<int> distribution; // это рандомное число без диапазона
    uniform_int_distribution<int> rand_range(0,255); // от 0 до 255
    Stars.resize(N); // указываю количество звезд через параметр функции
    for (int i = 0; i < N; i++)
    {
        Stars[i].x = distribution(mt) % pb_frame->width();
        Stars[i].y = distribution(mt) % pb_frame->height();
        Stars[i].color.setRgb(rand_range(mt),rand_range(mt),rand_range(mt));
    }
}

double MainWindow::newY_Coord(double x1, double y1, double x)
{
    int height = pb_frame->height();
    int width = pb_frame->width();
    double y = (y1 - height / 2) * (x - width / 2) / (x1 - width / 2) + height / 2;
    return y;
}

void MainWindow::setSpeed(int k)
{
    speed = k;
}

void MainWindow::ToCentre()
{
    int N = Stars.size();
    for (int j = 0; j < speed; j++)
    {
        for (int i = 0; i < N; i++)
        {
            if (Stars[i].x < (pb_frame->width() / 2))
            {
                double Tx = newY_Coord(Stars[i].x -= 1, Stars[i].y, Stars[i].x += 2);
                if (Tx > 0.005 || Tx < pb_frame->height())
                {
                Stars[i].y = Tx;
                }
            }
            else
            {
                double Tx = newY_Coord(Stars[i].x += 1, Stars[i].y, Stars[i].x -= 2);
                if (Tx > 0.005 || Tx < pb_frame -> height())
                {
                Stars[i].y = Tx;
                }
            }
        }
        pb_frame->risovanie();
        pb_frame->update();
    }
}

void MainWindow::un_vcentr()
{
    int N = Stars.size();
    for (int j = 0; j < speed; j++)
    {
        for (int i = 0; i < N; i++)
        {
            if (Stars[i].x < (pb_frame->width() / 2))
            {
               Stars[i].y = newY_Coord(Stars[i].x -= 1, Stars[i].y, Stars[i].x -= 2);
            }
            else
            {
              Stars[i].y = newY_Coord(Stars[i].x += 1, Stars[i].y, Stars[i].x += 2);
            }
        }
        pb_frame->update();
    }
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    setSpeed(value);
}

void MainWindow::on_pushButton_3_clicked()
{
    connect(paintTimer, SIGNAL(timeout()), this, SLOT(un_vcentr()));
    paintTimer->start(10);
}

void MainWindow::on_checkBox_toggled(bool checked)
{
    if (checked) pb_frame->trail = true;
    else pb_frame->trail = false;
}

void MainWindow::on_pushButton_start_clicked()
{
    paintTimer->start(10);
}

void MainWindow::on_pushButton_pause_clicked()
{
    paintTimer->stop();
}
