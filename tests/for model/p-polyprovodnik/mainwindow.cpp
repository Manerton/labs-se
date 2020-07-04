#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <random>
#include <QDoubleValidator>

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    graphic = new PictureBox(); // определяю фрейм, в котором рисую
    ui->graphicLayout->addWidget(graphic);
    connect(this, &MainWindow::signalPauseAnimation, graphic, &PictureBox::pauseAnim);
    graphic->show_p_semi();
    ui->lineEdit->setValidator(new QIntValidator(0, 2, this));
    hideInputs();
    //graphic->pauseAnim();

//    QTimer *timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), graphic, SLOT(unpauseAnim()));
//    timer->start(2000);
//    timer->setSingleShot(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hideInputs()
{
    ui->lineEdit->hide();
    ui->lineEdit_2->hide();
}

void MainWindow::on_pushButton_2_clicked()
{
    emit signalPauseAnimation();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    hideInputs();
    QWidget *p = ui->tabWidget->currentWidget();
    if (p == ui->p_semi)
    {
        ui->graphicLayout->addWidget(graphic);
        graphic->show_p_semi();
    } else if (p == ui->n_semi)
    {
        ui->graphicLayout_2->addWidget(graphic);
        graphic->show_n_semi();
    } else if (p == ui->diod)
    {
        ui->graphicLayoutDiod->addWidget(graphic);
        graphic->show_diod();
    } else if (p == ui->halfbitsum)
    {
        ui->graphicLayout_halfbitsum->addWidget(graphic);
        graphic->show_halfbitsum();
        ui->lineEdit->show();
        ui->lineEdit_2->show();
    } else if (p == ui->twobitsum)
    {
        ui->graphicLayout_twobitsum->addWidget(graphic);
        graphic->show_twobitsum();
        ui->lineEdit->show();
        ui->lineEdit_2->show();
    }
}

void MainWindow::on_pushButton_clicked()
{
    graphic->changePolarity();
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if (arg1 == "1" || arg1 == "0")
    {
        if (ui->lineEdit_2->text() == "1" || ui->lineEdit_2->text() == "0")
        {
            bool x = arg1.toInt();
            bool y = ui->lineEdit_2->text().toInt();
            graphic->callHalfBitSum(x,y);
        }
    }
}

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    if (arg1 == "1" || arg1 == "0")
    {
        if (ui->lineEdit->text() == "1" || ui->lineEdit->text() == "0")
        {
            bool y = arg1.toInt();
            bool x = ui->lineEdit->text().toInt();
            graphic->callHalfBitSum(x,y);
        }
    }
}
