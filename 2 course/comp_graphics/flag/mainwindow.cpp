#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "picturebox.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    picturebox *flag_frame = new picturebox(ui->frame);
    flag_frame->resize(357,115);
}

MainWindow::~MainWindow()
{
    delete ui;
}
