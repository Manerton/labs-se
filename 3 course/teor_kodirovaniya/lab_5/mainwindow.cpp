#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <set>
#include <string>
#include <fstream>
#include <QMessageBox>
#include "model.h"
#include "bigbinary.h"

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

void MainWindow::startWork()
{
    if (str.length() != 0)
    {
        const size_t k = str.length();
        auto alpha_std_str = str.toStdString();
        BigBinary alpha(alpha_std_str);
        Model m(alpha);
        ui->lineEdit_k->setText(QString::number(k));
        ui->lineEdit_r->setText(QString::number(m.getR()));
        ui->lineEdit_n->setText(QString::number(m.getN()));
        ui->lineEdit_gen_polynom->setText(QString::fromStdString(m.getGen_polynom().toString()));
        ui->lineEdit_kodirovanie_1->setText(QString::fromStdString(m.getKodirovanie1().toString()));
        ui->lineEdit_kodirovanie_2->setText(QString::fromStdString(m.getKodirovanie2().toString()));
        ui->lineEdit_B->setText(QString::fromStdString(m.getB().toString()));
        ui->lineEdit_B_with_error->setText(QString::fromStdString(m.getB_with_err().toString()));
        ui->lineEdit_shift_count->setText(QString::number(m.getShift_count()));
        ui->lineEdit_B_fixed->setText(QString::fromStdString(m.getB_fixed().toString()));
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
