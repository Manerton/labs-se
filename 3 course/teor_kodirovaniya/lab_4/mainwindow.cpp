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
    /// DEBUG
    ui->lineEdit_text->setText("1001");
    ui->pushButton_finishText->click();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_finishText_clicked()
{
    str = ui->lineEdit_text->text();
    startWork();
}

void MainWindow::fillTable(QTableWidget *table, const MainWindow::matrix& M)
{
    // очищаем старую таблицу (если была)
    table->setRowCount(0);
    const size_t rows = M.size();
    const size_t columns = M.at(0).size();
    table->setRowCount(rows);
    table->setColumnCount(columns);
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < columns; ++j)
        {
            table->setItem(i,j,new QTableWidgetItem(QString::number(M[i][j])));
        }
    }
}

string MainWindow::rowToStr(const Model::Row &row)
{
    std::string s(row.begin(),row.end());
    for (auto &c : s) c += '0';
    return s;
}

Model::Row MainWindow::strToRow(const string &s)
{
    Model::Row row(s.begin(),s.end());
    for (auto &c : row) c -= '0';
    return row;
}

void MainWindow::startWork()
{
    if (str.length() != 0)
    {
        const size_t k = str.length();
        auto alpha_std_str = str.toStdString();
        auto alpha = strToRow(alpha_std_str);
        Model m(alpha);
        ui->lineEdit_k->setText(QString::number(k));
        ui->lineEdit_r->setText(QString::number(m.getR()));
        ui->lineEdit_n->setText(QString::number(m.getN()));
        fillTable(ui->tableWidget_check_matr,*(m.getCheck_matr_ptr()));
        fillTable(ui->tableWidget_gen_matr,*(m.getGen_matr_ptr()));
        Model::Row beta(m.getB());
        QString beta_str = QString::fromStdString(rowToStr(beta));
        ui->lineEdit_B->setText(beta_str);
        Model::Row beta_with_err(m.getB_with_err());
        QString beta_with_err_str = QString::fromStdString(rowToStr(beta_with_err));
        ui->lineEdit_B_with_error->setText(beta_with_err_str);
        Model::Row S(m.getS());
        QString S_str = QString::fromStdString(rowToStr(S));
        ui->lineEdit_S->setText(S_str);
        ui->lineEdit_razryad_err->setText(QString::number(m.getError_razryad()));
    }
}

void MainWindow::on_lineEdit_text_textChanged(const QString &arg1)
{
    if (arg1.length() > 0)
    {
        if(arg1.back() != '0' && arg1.back() != '1')
            ui->lineEdit_text->setText(arg1.left(arg1.length()-1));
    }
}
