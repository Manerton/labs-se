#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "teoria.h"
#include "test.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // -- для возврата в главное меню из подсистем теории, демонстрации и теста -- //
    connect(ui->teoria, &Teoria::return_to_menu, this, &MainWindow::return_to_menu);
    connect(ui->test, &Test::return_to_menu, this, &MainWindow::return_to_menu);
    connect(ui->demo, &Demo::return_to_menu, this, &MainWindow::return_to_menu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_teoria_clicked()
{
    setMaximumWidth(16777215);
    setMaximumHeight(16777215);
    ui->stackedWidget->setCurrentWidget(ui->teoria);            // -- переключится на теорию -- //
}

void MainWindow::on_pushButton_test_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->test);
}


void MainWindow::return_to_menu()
{
    ui->stackedWidget->setCurrentWidget(ui->mainmenu);
    setMaximumWidth(800);
    setMaximumHeight(600);
}


void MainWindow::on_pushButton_demo_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->demo);
}
