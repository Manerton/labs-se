#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    load_from_file();
    setupGraph(ui->customplot);
}

void MainWindow::load_from_file()
{
    std::ifstream file("file.txt");
    if (file)
    {
        double cost;
        std::string name;
        while(!file.eof())
        {
            std::string s;
            getline(file, s);
            std::istringstream ss(s);
            ss >> std::quoted(name) >> cost;
            v_cost.push_back(cost);
            v_name.push_back(QString::fromStdString(name));
        }
    }
}

void MainWindow::setupGraph(QCustomPlot *customPlot)
{
    // читаем данные из файла:
    QVector<double> x(v_name.size());
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);    // -- добавим на горизонтальную ось названия -- //
    for (int i=0; i < x.size(); ++i)
    {
        x[i] = i;
        textTicker->addTick(i, v_name[i]);
    }

    // создаем график:
    QCPGraph *graph1 = customPlot->addGraph();
    graph1->setData(x, v_cost);
    customPlot->xAxis->setTicker(textTicker);

    graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    graph1->setPen(QPen(QColor(120, 120, 120), 2));

    // настройки всякие, цвета, толщина и так далее
    customPlot->xAxis->setLabel("Названия игрушек");
    customPlot->yAxis->setLabel("Стоимость игрушек");
    customPlot->xAxis->setLabelColor(Qt::white);
    customPlot->yAxis->setLabelColor(Qt::white);
    customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridVisible(false);
    customPlot->yAxis->grid()->setSubGridPen(Qt::NoPen);
    customPlot->yAxis->grid()->setSubGridVisible(false);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    customPlot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));

    customPlot->rescaleAxes();
    customPlot->xAxis->setRange(0,3);
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

}

MainWindow::~MainWindow()
{
    delete ui;
}

