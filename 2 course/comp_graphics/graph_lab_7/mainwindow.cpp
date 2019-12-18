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

void MainWindow::on_pushButton_draw_clicked()
{
    pb_frame->risovanie();
    pb_frame->repaint();
}


void MainWindow::on_pushButton_draw_zatravka_clicked()
{
    pb_frame->risovanie_zatravka();
    pb_frame->repaint();
}

void MainWindow::on_pushButton_draw_zatravka_line_clicked()
{
    pb_frame->risovanie_zatravka_line();
    pb_frame->repaint();
}
