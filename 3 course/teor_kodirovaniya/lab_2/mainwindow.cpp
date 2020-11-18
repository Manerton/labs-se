#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <set>
#include <string>
#include <fstream>
#include <QMessageBox>
#include "model.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->singleEntropy_formula->setVisible(false);
    //ui->uslovnayaEntropy_formula->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_fromFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, ("Открыть файл"), "C://", "Текстовый файл (*.txt)");
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        str = file.readAll();
        ui->lineEdit_text->setText(str);
    } else QMessageBox::warning(this,"Ошибка!","Не удалось открыть файл " + filename);
    file.close();

    startWork();
}

void MainWindow::on_pushButton_finishText_clicked()
{
    str = ui->lineEdit_text->text();
    startWork();
}

void MainWindow::prepareTable(QTableWidget* table, std::multiset<QString> &set)
{
    // очищаем старую таблицу (если была)
    table->setRowCount(0);
    size_t row_index = 0;
    // константы
    const size_t N = set.size();
    const auto nomer_column = Model::columns::nomer_column;
    const auto X_column     = Model::columns::X_column;
    const auto n_column     = Model::columns::n_column;
    const auto W_column     = Model::columns::W_column;
    // проходим по уникальным элементам
    for (auto it = set.begin(); it != set.end(); it = set.upper_bound(*it))
    {
        const auto symbol = *it;
        // получаем количество повторений элемента в множестве
        const auto count = set.count(symbol);
        table->insertRow(row_index);
        table->setItem(row_index,X_column,new QTableWidgetItem((symbol == " ") ? "пробел" : QString(symbol)));
        table->setItem(row_index,n_column,new QTableWidgetItem(QString::number(count)));
        table->setItem(row_index,W_column,new QTableWidgetItem(QString::number(count) + " / " + QString::number(N)));
        ++row_index;
    }
    // сортируем в порядке убывания вероятностей
    table->sortItems(n_column,Qt::DescendingOrder);
    const size_t row_count = table->rowCount();
    for (size_t i = 0; i < row_count; ++i)
    {
        table->setItem(i,nomer_column,new QTableWidgetItem(QString::number(i+1)));
    }
    table->insertRow(row_index);
    table->setItem(row_index,nomer_column,new QTableWidgetItem("∑"));
    table->setItem(row_index,n_column,new QTableWidgetItem(QString::number(set.size())));
    table->setItem(row_index,W_column,new QTableWidgetItem("1"));
}

void MainWindow::startWork()
{
    if (str.length() != 0)
    {
        const size_t N = str.length();
        // создаем мультимножество
        multiset<QString> A(str.begin(),str.end());
        // готовим схему однобуквенных сочетаний
        prepareTable(ui->tableWidget_single,A);
        // 1. считаем энтропию, приходящуюся в среднем на одну букву
        Model M;
        double H_A = M.calculate_singleEntropy(ui->tableWidget_single);
        ui->lineEdit_singleEntropy->setText(QString::number(H_A));
        // создаем мультимножество для двубуквенных сочетаний
        multiset<QString> AB;
        for (size_t i = 0; i < N-1; ++i) AB.insert(str.mid(i,2));
        AB.insert(str.right(1) + str.left(1));
        // готовим схему двухбуквенных сочетаний
        prepareTable(ui->tableWidget_double,AB);
        // 2. считаем энтропию, приходящуюся на одно двубуквенное сочетание
        double H_AB = M.calculate_singleEntropy(ui->tableWidget_double);
        ui->lineEdit_doubleEntropy->setText(QString::number(H_AB));
        // 3. считаем условную энтропию H(B/A)
        double H_B_over_A = M.calculate_uslovnayaEntropy(ui->tableWidget_single,ui->tableWidget_double);
        ui->lineEdit_uslovnayaEntropy->setText(QString::number(H_B_over_A));
        //    считаем условную энтропию H(A/B)
        QTableWidget *table_BA = new QTableWidget();
        table_BA->setColumnCount(4);
        multiset<QString> BA;
        QString reverse_str(str.length(),' ');
        reverse_copy(str.begin(),str.end(),reverse_str.begin());
        for (size_t i = 0; i < N-1; ++i) BA.insert(reverse_str.mid(i,2));
        BA.insert(reverse_str.right(1) + reverse_str.left(1));
        prepareTable(table_BA,BA);
        double H_A_over_B = M.calculate_uslovnayaEntropy(ui->tableWidget_single,table_BA);
        ui->lineEdit_uslovnayaEntropy2->setText(QString::number(H_A_over_B));
        //    считаем информацию I(A,B)
        double info = H_A - H_A_over_B;
        ui->lineEdit_info->setText(QString::number(info));
        //  4. считаем длину кода
        auto m = ui->tableWidget_single->rowCount()-1;
        int l = M.calculateLengthOfCode(m);
        ui->lineEdit_lengthOfCode->setText(QString::number(l));
        //  5. избыточность
        double Dp = M.calculateDp(H_A,m);
        double Ds = M.calculateDs(H_A,H_B_over_A);
        ui->lineEdit_Dp->setText(QString::number(Dp));
        ui->lineEdit_Ds->setText(QString::number(Ds));
        ui->lineEdit_D->setText(QString::number(M.calculateD(Dp,Ds)));
    }
}
