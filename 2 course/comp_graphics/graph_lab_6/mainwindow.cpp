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
    pb_frame->risovanie();
    pb_frame->repaint();
}

void MainWindow::on_pushButton_disable_clicked()
{
    pb_frame->disable();
    pb_frame->repaint();
}

void MainWindow::on_comboBox_lens_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Линза #1")
    {
        pb_frame->Lens = PictureBox::LensNumber::First;
    } else if (arg1 == "Линза #2")
    {
        pb_frame->Lens = PictureBox::LensNumber::Second;
    }
    pb_frame->disable();
}

void MainWindow::on_comboBox_pic_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Картинка #1")
    {
        pb_frame->Pic = PictureBox::PicNumber::First;
    } else if (arg1 == "Картинка #2")
    {
        pb_frame->Pic = PictureBox::PicNumber::Second;
    }
    pb_frame->disable();
}
