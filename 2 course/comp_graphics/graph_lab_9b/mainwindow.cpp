#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGraph(ui->customplot);
}

void MainWindow::setupGraph(QCustomPlot *customPlot)
{
    // создадим пустой график функции
    QCPCurve *figure = new QCPCurve(customPlot->xAxis, customPlot->yAxis);
    figure->setName("x=13(cos(t)−cos(6,5t)/6,5), y=13(sin(t)−sin(6,5t)/6,5), t [0;4π]");
    // подготовим данные для графика
    const int pointCount = 500;
    QVector<QCPCurveData> dataFigure(pointCount);
    for (int i=0; i<pointCount; ++i)
    {
        double t = double(i)/(pointCount-1)*4*M_PI;
        double x = 13 * (cos(t)-cos(6.5 * t) / 6.5);
        double y = 13 * (sin(t)-sin(6.5 * t) / 6.5);
        dataFigure[i] = QCPCurveData(i, x, y);
    }
    // поместим в график данные
    figure->data()->set(dataFigure, true);
    // color the curves:
    figure->setPen(QPen(Qt::blue));
    figure->setBrush(QBrush(QColor(0, 0, 255, 20)));
    // настройки customplot
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->rescaleAxes();
    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica",9));

}

MainWindow::~MainWindow()
{
    delete ui;
}

