#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <set>
#include <string>
#include "model.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->entropy_formula->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_fromFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, ("Открыть файл"), "C://", "Текстовый файл (*.txt)");
    qDebug() << filename;
}

void MainWindow::on_pushButton_finishText_clicked()
{
    // очищаем старую таблицу (если была)
    ui->tableWidget_single->setRowCount(0);
    // необходимые константы
    const QString str = ui->lineEdit_text->text();
    const size_t N = str.length();
    const int nomer_column  = 0;
    const int X_column      = 1;
    const int n_column      = 2;
    const int W_column      = 3;
    // создаем мультимножество
    multiset<QChar> A(str.begin(),str.end());
    size_t row_index = 0;
    // проходим по уникальным элементам
    for (auto it = A.begin(); it != A.end(); it = A.upper_bound(*it))
    {
        const auto symbol = *it;
        // получаем количество повторений элемента в множестве
        const auto count = A.count(symbol);
        ui->tableWidget_single->insertRow(row_index);
        ui->tableWidget_single->setItem(row_index,X_column,new QTableWidgetItem((symbol == " ") ? "пробел" : QString(symbol)));
        ui->tableWidget_single->setItem(row_index,n_column,new QTableWidgetItem(QString::number(count)));
        ui->tableWidget_single->setItem(row_index,W_column,new QTableWidgetItem(QString::number(count) + " / " + QString::number(N)));
        ++row_index;
    }
    // сортируем в порядке убывания вероятностей
    ui->tableWidget_single->sortItems(n_column,Qt::DescendingOrder);
    const size_t row_count = ui->tableWidget_single->rowCount();
    for (size_t i = 0; i < row_count; ++i)
    {
        ui->tableWidget_single->setItem(i,nomer_column,new QTableWidgetItem(QString::number(i+1)));
    }
    ui->tableWidget_single->insertRow(row_index);
    ui->tableWidget_single->setItem(row_index,nomer_column,new QTableWidgetItem("∑"));
    ui->tableWidget_single->setItem(row_index,n_column,new QTableWidgetItem(QString::number(A.size())));
    ui->tableWidget_single->setItem(row_index,W_column,new QTableWidgetItem("1"));

    Model m(ui->tableWidget_single);
    ui->lineEdit_singleEntropy->setText(QString::number(m.calculate_singleEntropy()));

    // очищаем старую таблицу (если была)
    ui->tableWidget_double->setRowCount(0);
}
