#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QClipboard>
#include <QKeyEvent>
#include <QLocale>

// вывести вер. схему для данного p
void MainWindow::show_probality_scheme()
{
    ui->tableWidget_veroyat->setColumnCount(0); // очищаем таблицу
    auto k = ui->spinBox_k->value(); // получаем текущий k
    const auto ps = vector_schemes.at(k); // вероятностная схема для этого k
    const int xi_row = 0; // номер строки для xi в таблице (вер. схема)
    const int pi_row = 1; // для pi

    for (auto var : ps.getData())
    {
        const int column = var.first;
        ui->tableWidget_veroyat->insertColumn(column);
        ui->tableWidget_veroyat->setItem(xi_row,column,new QTableWidgetItem(QString::number(var.first)));
        ui->tableWidget_veroyat->setItem(pi_row,column,new QTableWidgetItem(QLocale().toString(var.second)));
    }
    ui->label_entropy->setText("Энтропия для k = " + QString::number(k) + " равна: ");
    ui->lineEdit_entropy->setText(QString::number(ps.getEntropy()));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_k->setVisible(false);
    ui->spinBox_k->setVisible(false);
}

void MainWindow::setupGraph()
{
    auto customPlot = ui->widget_grafik;
    // создадим пустой график функции
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue));
    customPlot->xAxis->setLabel("k");
    customPlot->yAxis->setLabel("H(k)");
    customPlot->xAxis->setLabelFont(QFont("Helvetica",12));
    customPlot->yAxis->setLabelFont(QFont("Helvetica",12));
    auto n = ui->spinBox_n->value();
    // добавим точки на график
    for (int i = 0; i <= n; ++i)
    {
        customPlot->graph(0)->addData(i,vector_schemes[i].getEntropy());
    }
    // добавим возможность зумить и перемещаться по графику
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    // добавим точки максимума и минимума
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(Qt::darkGreen));
    customPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 7));
    customPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
    auto max = std::max_element(vector_schemes.begin(),vector_schemes.end(), [](probability_scheme a, probability_scheme b) { return a.getEntropy() < b.getEntropy();});
    customPlot->graph(1)->addData(max-vector_schemes.begin(),max->getEntropy());

    auto min = vector_schemes.begin();
    while (min != vector_schemes.end())
    {
        min = std::min_element(min,vector_schemes.end(), [](probability_scheme a, probability_scheme b) { return a.getEntropy() < b.getEntropy();});
        customPlot->graph(1)->addData(min-vector_schemes.begin(),min->getEntropy());
        ++min;
    }
    ui->lineEdit_max->setText("(" + QString::number(max-vector_schemes.begin()) + "; " + QString::number(max->getEntropy()) + ")");
    // перемасштабируем график и сделаем небольшой отступ
    customPlot->rescaleAxes();
    customPlot->xAxis->setRange(-1,n+1);
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

void MainWindow::on_spinBox_k_valueChanged(int)
{
    show_probality_scheme();
}

void MainWindow::on_pushButton_grafik_clicked()
{
    ui->pushButton_grafik->setVisible(false);   // скрываем кнопку
    // получаем значение n и m
    const auto n = ui->spinBox_n->value();
    const auto m = ui->spinBox_m->value();
    // запрещаем вводить в поля n и m
    ui->spinBox_n->setDisabled(true);
    ui->spinBox_m->setDisabled(true);
    // позволяем вводить в поле для k
    ui->label_k->setVisible(true);
    ui->spinBox_k->setVisible(true);
    ui->spinBox_k->setMaximum(n);
    // рассчитываем вер. схемы для k от 0 до n с шагом 1
    for (int i = 0; i <= n; ++i)
    {
        vector_schemes.push_back(probability_scheme(n,i,m));
    }
    show_probality_scheme();
    setupGraph();
}

void MainWindow::on_spinBox_m_valueChanged(int arg1)
{
    if (arg1 > ui->spinBox_n->value())
    {
        ui->spinBox_m->setValue(arg1-1);
    }
}
