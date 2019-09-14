#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    PictureBox *pb_frame = new PictureBox(ui->frame);
    pb_frame->resize(357,115);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void PictureBox::paint(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawRect(0,0,50,50);
}
