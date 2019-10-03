#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <random>

using namespace std;

extern QVector<Star> Stars; // объявляю глобальный вектор звезд
QVector<Star> Stars; // пока без размера

//Глобальные переменные для рандома
static time_t seed = time(nullptr);
static mt19937 mt(seed);
static uniform_int_distribution<int> distribution; // это рандомное число без диапазона
static uniform_int_distribution<int> rand_range(0,255); // от 0 до 255
static uniform_int_distribution<int> rand_range2(100,300);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pb_frame = new PictureBox(ui->frame); // определяю фрейм, в котором рисую
    pb_frame->resize(ui->frame->size()); // указываю размер как у фрейма с оранжевой рамкой
    RandomizeStars(); // получаю рандомные координаты звезд
    pb_frame->risovanie(); // рисую
    paintTimer = new QTimer(this); // создаю таймер
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RandomizeStars()
{
    int x_centre = (pb_frame ->width())/2;
    int y_centre = (pb_frame ->height())/2;
    uniform_real_distribution<double> rand_range_size(min_size,max_size);
    Stars.resize(count);
    for (int i = 0; i < count; i++)
    {
        Stars[i].x = distribution(mt) % pb_frame->width();
        Stars[i].y = distribution(mt) % pb_frame->height();
        Stars[i].color.setRgb(rand_range(mt),rand_range(mt),rand_range(mt));
        double step = rand_range2(mt);
        Stars[i].dx = (x_centre - Stars[i].x) / step;
        Stars[i].dy = (y_centre - Stars[i].y) / step;
        Stars[i].size = rand_range_size(mt);
    }
}

double MainWindow::newY_Coord(double x1, double y1, double x)
{
    int height = pb_frame->height() / 2;
    int width = pb_frame->width() / 2;
    if (fabs(x1-width) < 0.0001) return height; // иначе деление на ноль

    double y = (y1 - height) * (x - width) / (x1 - width) + height;
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
            double Ty;
            Stars[i].x = static_cast<int>(Stars[i].x);
            if (Stars[i].x < (pb_frame->width() / 2))
            {
                Ty = newY_Coord(Stars[i].x -= 1, Stars[i].y, Stars[i].x += 2);
            }
            else {
                Ty = newY_Coord(Stars[i].x += 1, Stars[i].y, Stars[i].x -= 2);
            }
            Stars[i].y = Ty;
        }
    }
    pb_frame->risovanie();
    pb_frame->repaint();
}

void MainWindow::FromCentre()
{
    int N = Stars.size();
    for (int j = 0; j < speed; j++)
    {
        for (int i = 0; i < N; i++)
        {
           Stars[i].x -= Stars[i].dx;
           Stars[i].y -= Stars[i].dy;
        }
    }
    pb_frame->risovanie();
    pb_frame->repaint();
}

void MainWindow::on_speedSlider_valueChanged(int value)
{
    setSpeed(value);
}

void MainWindow::on_pushButton_fromcentre_clicked()
{
    disconnect(paintTimer, SIGNAL(timeout()), this, nullptr);
    connect(paintTimer, SIGNAL(timeout()), this, SLOT(FromCentre()));
    paintTimer->start(10);
}

void MainWindow::on_checkBox_trail_toggled(bool checked)
{
    if (checked) pb_frame->trail = true;
    else pb_frame->trail = false;
}

void MainWindow::on_pushButton_tocentre_clicked()
{
    disconnect(paintTimer, SIGNAL(timeout()), this, nullptr);
    connect(paintTimer, SIGNAL(timeout()), this, SLOT(ToCentre()));
    paintTimer->start(10);

}

void MainWindow::on_pushButton_pause_clicked()
{
    if (paintTimer->isActive()) {paintTimer->stop();}
    else {paintTimer->start(10);}

}

void MainWindow::on_pushButton_create_clicked()
{
    disconnect(paintTimer, SIGNAL(timeout()), this, nullptr);
    RandomizeStars();
    pb_frame->reset();
    pb_frame->risovanie();
    pb_frame->repaint();
}

void MainWindow::on_doubleSpinBox_minsize_valueChanged(double arg1)
{
    min_size = arg1;
    if (ui->doubleSpinBox_maxsize->value() < arg1) ui->doubleSpinBox_maxsize->setValue(arg1);
    ui->doubleSpinBox_maxsize->setMinimum(arg1);
}


void MainWindow::on_doubleSpinBox_maxsize_valueChanged(double arg1)
{
    max_size = arg1;
}

void MainWindow::on_spinBox_count_valueChanged(int arg1)
{
    count = arg1;
}
