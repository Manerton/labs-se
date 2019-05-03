#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->plot->addGraph();
    ui->plot->xAxis->setLabel("x");
    ui->plot->yAxis->setLabel("y");

    ui->plot->xAxis->setRange(-10,10);
    ui->plot->yAxis->setRange(-10,10);
    ui->plot->xAxis->setAutoTickStep(false);
    ui->plot->yAxis->setAutoTickStep(false);
    ui->plot->xAxis->setTickStep(1);
    ui->plot->yAxis->setTickStep(1);

    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsNone);


    std::ifstream input("dots.txt");
    if (input) // проверка открытия
    {
        int input_x, input_y;
        while(!input.eof())
        {
            input >> input_x >> input_y;
            addPoint(input_x,input_y);
        }
        input.close();
    }

    plot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addPoint(int x, int y)
{
    qv_x.append(x);
    qv_y.append(y);
}

void MainWindow::plot()
{
    ui->plot->graph(0)->setData(qv_x, qv_y);
    ui->plot->replot();
    ui->plot->update();
}
