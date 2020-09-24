#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QClipboard>
#include <QKeyEvent>

// вывести вер. схему для данного p
void MainWindow::show_probality_scheme()
{
    ui->tableWidget_veroyat->setColumnCount(0); // очищаем таблицу
    double p = ui->doubleSpinBox_p->value(); // получаем текущий p
    const auto ps = vector_schemes.at(round(p*100)); // вероятностная схема для этого p
    const int xi_row = 0; // номер строки для xi в таблице (вер. схема)
    const int pi_row = 1; // для pi

    for (auto var : ps.getData())
    {
        const int column = var.first-1;
        ui->tableWidget_veroyat->insertColumn(column);
        ui->tableWidget_veroyat->setItem(xi_row,column,new QTableWidgetItem(QString::number(var.first)));
        ui->tableWidget_veroyat->setItem(pi_row,column,new QTableWidgetItem(QString::number(var.second)));
    }
    ui->label_entropy->setText("Энтропия для p = " + QString::number(p) + " равна: ");
    ui->lineEdit_entropy->setText(QString::number(ps.getEntropy()));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_p->setDisabled(true);
    ui->doubleSpinBox_p->setDisabled(true);
}

void MainWindow::setupGraph()
{
    auto customPlot = ui->widget_grafik;
    // создадим пустой график функции
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue));
    customPlot->xAxis->setLabel("p");
    customPlot->yAxis->setLabel("H(p)");
    customPlot->xAxis->setLabelFont(QFont("Helvetica",12));
    customPlot->yAxis->setLabelFont(QFont("Helvetica",12));
    // добавим точки на график
    for (int i = 1; i <= 100; ++i)
    {
        customPlot->graph(0)->addData(double(i)/100,vector_schemes[i].getEntropy());
    }
    // добавим возможность зумить и перемещаться по графику
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    // добавим точки максимума и минимума
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::darkGreen));
    customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 7));
    customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
    auto max = std::max_element(vector_schemes.begin(),vector_schemes.end(), [](probability_scheme a, probability_scheme b) { return a.getEntropy() < b.getEntropy();});
    customPlot->graph(1)->addData(double(max-vector_schemes.begin())/100,max->getEntropy());
    auto min = std::min_element(vector_schemes.begin()+1,vector_schemes.end(), [](probability_scheme a, probability_scheme b) { return a.getEntropy() < b.getEntropy();});
    customPlot->graph(1)->addData(double(min-vector_schemes.begin())/100,min->getEntropy());
    ui->lineEdit_max->setText("(" + QString::number(double(max-vector_schemes.begin())/100) + "; " + QString::number(max->getEntropy()) + ")");
    // перемасштабируем график и сделаем небольшой отступ
    customPlot->rescaleAxes();
    customPlot->xAxis->setRange(-0.1,1.1);
    customPlot->yAxis->setRangeLower(-0.1);
    customPlot->yAxis->moveRange(0.05);
    customPlot->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// чтобы можно было скопировать ячейку, строку или таблицу
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers()==Qt::ControlModifier)
    {
       if(event->key() == Qt::Key_C)
       {
           QClipboard *clipboard = QApplication::clipboard();
           QString toClipboard;
           int prev_row = ui->tableWidget_veroyat->selectedItems().at(0)->row(); // чтобы можно было копировать больше одной строки (многострочное копирование)
           for (auto var : ui->tableWidget_veroyat->selectedItems())
           {
               toClipboard += "\t";
               if (prev_row != var->row())
               {
                   toClipboard += "\r\n";
                   prev_row = var->row();
               }

               toClipboard += var->text();
           }
           clipboard->setText(toClipboard.right(toClipboard.size()-1)); // удаляю в начале лишний символ табуляции
       }
    }
}

void MainWindow::on_doubleSpinBox_p_valueChanged(double arg1)
{
    show_probality_scheme();
}

void MainWindow::on_pushButton_grafik_clicked()
{
    ui->pushButton_grafik->setVisible(false);
    ui->label_p->setDisabled(false);
    ui->doubleSpinBox_p->setDisabled(false);
    // рассчитываем вер. схемы для p от 0 до 1 с шагом 0.01
    vector_schemes.push_back(probability_scheme(1)); // добавляю в начало фиктивный элемент, чтобы не сдвигать весь массив и потом работать с индексом как round(p*100), например 0.33 => 33
    for (int i = 1; i <= 100; ++i)
    {
        vector_schemes.push_back(probability_scheme(double(i)/100));
    }
    show_probality_scheme();
    setupGraph();
}
