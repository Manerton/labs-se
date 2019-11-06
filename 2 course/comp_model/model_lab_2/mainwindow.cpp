#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <algorithm>
#include <cmath>
#include "telo.h"
#include <QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), object(100,20,50)
{
    ui->setupUi(this);
    get_koef();
    update_temp();
    paintTimer = new QTimer(this); // создаю таймер
    connect(paintTimer, SIGNAL(timeout()), this, SLOT(action()));
}

void MainWindow::update_temp()
{
    ui->spinBox->setValue(time);
    for (int i = 0; i < 8; ++i)
    {
        QLineEdit *segment = static_cast<QLineEdit*>(ui->Layout->itemAt(i)->widget());
        segment->setText(QString::number(object.current_temperature[i],'g',3));
        int temp_color = int((object.current_temperature[i] - min_temp) * k_temp);
        segment->setStyleSheet("background-color: rgb(255, " + QString::number(255-temp_color) + ", " + QString::number(255-temp_color) + ");");
    }
}

void MainWindow::get_koef()
{
    max_temp = int(*max_element(object.current_temperature.begin(),object.current_temperature.end()));
    min_temp = int(*min_element(object.current_temperature.begin(),object.current_temperature.end()));
    k_temp = 255.0 / (max_temp - min_temp);
    qDebug() << k_temp;
}

void MainWindow::disable_buttons(bool var)
{
    ui->spinBox_env->setDisabled(var);
    ui->spinBox_right->setDisabled(var);
    ui->spinBox_left->setDisabled(var);
}

void MainWindow::action()
{
    --time;
    object.work();
    update_temp();
    if (time == 0) {paintTimer->stop(); disable_buttons(false);}
}

void MainWindow::on_pushButton_clicked()
{
    if (paintTimer->isActive()) {paintTimer->stop(); disable_buttons(false);}
    else if(time != 0)
    {
        paintTimer->start(500);
        disable_buttons(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    time = arg1;
}

void MainWindow::on_spinBox_left_valueChanged(int arg1)
{
    object.current_temperature[0] = arg1;
    get_koef();
    update_temp();
}

void MainWindow::on_spinBox_env_valueChanged(int arg1)
{
    for (size_t i = 1; i < 7; i++)
    {
        object.current_temperature[i] = arg1;
    }
    get_koef();
    update_temp();
}

void MainWindow::on_spinBox_right_valueChanged(int arg1)
{
    object.current_temperature[7] = arg1;
    get_koef();
    update_temp();
}
