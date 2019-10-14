#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <random>

using namespace std;

//Глобальные переменные для рандома
static time_t seed = time(nullptr);
static mt19937 mt(seed);
static uniform_int_distribution<int> rand_range(0,255); // от 0 до 255
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pb_frame = new PictureBox(ui->frame); // определяю фрейм, в котором рисую
    pb_frame->resize(ui->frame->size()); // указываю размер как у фрейма с оранжевой рамкой
    startFractal(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startFractal(int depth)
{
    QVector<coord>().swap(coords); // освобождаю занятую память
    coord a;
    a.x = pb_frame->width()/2;
    a.y = pb_frame->height()/2;
    a.size = sizeMin;
    if (depth != 0) getColors(depth);
    getCoords(a,depth);
    pb_frame->drawHDepth(coords);
}

void MainWindow::getCoords(coord a, int depth)
{
    coords.push_back(a);
    if (depth > 0)
    {
        a.color = colors.at(depth-1);
        a.size /= 2;
        coord tmp = a;
        depth--;
        tmp.x =  a.x - a.size;
        tmp.y =  a.y - a.size;
        getCoords(tmp, depth);

        tmp.x =  a.x - a.size;
        tmp.y =  a.y + a.size;
        getCoords(tmp, depth);

        tmp.x =  a.x + a.size;
        tmp.y =  a.y - a.size;
        getCoords(tmp, depth);

        tmp.x =  a.x + a.size;
        tmp.y =  a.y + a.size;
        getCoords(tmp, depth);
    }
}

void MainWindow::getColors(int depth)
{
    colors.reserve(depth);
    for (int i = 0; i < depth; i++)
    {
        colors[i] = QColor(rand_range(mt),rand_range(mt),rand_range(mt));
    }
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    startFractal(arg1 - 1);
    pb_frame->repaint();
}
