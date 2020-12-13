#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <set>
#include <string>
#include <fstream>
#include <QMessageBox>
#include <sstream>
#include "bigbinary.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // загружаем таблицы
    load_table2_from_file("table2.txt");
    load_table3_from_file("table3.txt");
    ui->lineEdit_text->setText("10011");
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
        ui->lineEdit_s->setText(QString::number(m.getS()));
        ui->lineEdit_h->setText(QString::number(m.getH()));
        ui->lineEdit_i->setText(QString::number(m.getI()));
        ui->lineEdit_gen_polynom->setText(QString::fromStdString(m.getGen_polynom().toString()));
        ui->lineEdit_kodirovanie_1->setText(QString::fromStdString(m.getKodirovanie1().toString()));
        ui->lineEdit_kodirovanie_2->setText(QString::fromStdString(m.getKodirovanie2().toString()));
        ui->lineEdit_B->setText(QString::fromStdString(m.getB().toString()));
        ui->lineEdit_B_with_error->setText(QString::fromStdString(m.getB_with_err().toString()));
        ui->lineEdit_m->setText(QString::number(m.getM()));
        ui->lineEdit_shift_count->setText(QString::number(m.getShift_count()));
        ui->lineEdit_B_fixed->setText(QString::fromStdString(m.getB_fixed().toString()));
    }
}

void MainWindow::load_table2_from_file(const string filename)
{
    ifstream file(filename);
    if (file)
    {
        Model::Table2 tb;
        int temp;
        while (file >> temp)
        {
            int n, k, r, s;
            n = temp;
            file >> k >> r >> s;
            tb.push_back({n,k,r,s});
        }
        Model::setTable2(tb);
    } else QMessageBox::warning(this,"Ошибка!","Не удалось открыть файл " + QString::fromStdString(filename));
}

void MainWindow::load_table3_from_file(const string filename)
{
    ifstream file(filename);
    if (file)
    {
        Model::Table3 tb;
        std::string temp_line;
        // читаем строку
        while (getline(file,temp_line))
        {
            stringstream ss(temp_line);
            // читаем номер строки i из таблицы 3
            int i = 0;
            ss >> i;
            // пока пустая строка
            Model::table3_row newRow(9,0);
            // читаем все двоичные многочлены из этой строки
            vector<string> temp_arr;
            string temp;
            while (ss >> temp) temp_arr.push_back(temp);
            // начинаем добавлять эти многочлены в строку с конца,
            // чтобы в строке оставались пустые элементы (когда в temp_arr элементов < 10)
            size_t index = newRow.size()-1;
            for (auto it = temp_arr.rbegin(); it != temp_arr.rend(); ++it)
            {
                newRow[index] = BigBinary(*it);
                --index;
            }
            tb[i] = newRow;
        }
        Model::setTable3(tb);
    } else QMessageBox::warning(this,"Ошибка!","Не удалось открыть файл " + QString::fromStdString(filename));
}

void MainWindow::on_lineEdit_text_textChanged(const QString &arg1)
{
    if (arg1.length() > 0)
    {
        if(arg1.back() != '0' && arg1.back() != '1')
            ui->lineEdit_text->setText(arg1.left(arg1.length()-1));
    }
}
