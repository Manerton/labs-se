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
    connect(ui->test, &Test::enableMaxButtonOnWindow, this, &MainWindow::enableMaxButtonOnWindow);
    connect(ui->test, &Test::disableMaxButtonOnWindow, this, &MainWindow::disableMaxButtonOnWindow);
    disableMaxButtonOnWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_teoria_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->teoria);            // -- переключится на теорию -- //
    enableMaxButtonOnWindow();
}

void MainWindow::on_pushButton_test_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->test);
}


void MainWindow::return_to_menu()
{
    ui->stackedWidget->setCurrentWidget(ui->mainmenu);
    disableMaxButtonOnWindow();
}


void MainWindow::on_pushButton_demo_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->demo);
    enableMaxButtonOnWindow();
}

void MainWindow::enableMaxButtonOnWindow()
{
    setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    show();
}

void MainWindow::disableMaxButtonOnWindow()
{
    setMaximumWidth(800);
    setMaximumHeight(600);
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    show();
}
