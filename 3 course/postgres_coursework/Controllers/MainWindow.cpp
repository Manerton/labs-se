#include "MainWindow.h"
#include "../Views/ui_MainWindow.h"
#include <cstdint>
#include <memory>
#include <QTableView>
#include <QMessageBox>
#include <QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      db(),
      authForm(make_unique<AuthForm>(db)),              // создаем форму для авторизации
      buyerForm(make_unique<BuyerForm>(this,db)),       // создаем форму для покупателя
      managerForm(make_unique<ManagerForm>(this,db)),   // создаем форму для менеджера
      operatorForm(make_unique<OperatorForm>(this,db))  // создаем форму для оператора
{
    ui->setupUi(this);
    ui->stackedWidget->addWidget(buyerForm.get());
    ui->stackedWidget->addWidget(managerForm.get());
    ui->stackedWidget->addWidget(operatorForm.get());

    // привязываем сигналы к слотам
    connect(authForm.get(), &AuthForm::authBuyerDone, this, &MainWindow::showBuyerForm);
    connect(authForm.get(), &AuthForm::authManagerDone, this, &MainWindow::showManagerForm);
    connect(authForm.get(), &AuthForm::authOperatorDone, this, &MainWindow::showOperatorForm);

    connect(buyerForm.get(), &BuyerForm::backToAuth, this, &MainWindow::backToAuth);
    connect(managerForm.get(), &ManagerForm::backToAuth, this, &MainWindow::backToAuth);
    connect(operatorForm.get(), &OperatorForm::backToAuth, this, &MainWindow::backToAuth);
    // показываем окно авторизации
    authForm->show();
}

void MainWindow::showBuyerForm()
{
    ui->stackedWidget->setCurrentWidget(buyerForm.get());
    this->show();
    buyerForm->initialRead();
}

void MainWindow::showManagerForm()
{
    ui->stackedWidget->setCurrentWidget(managerForm.get());
    this->show();
    managerForm->initialRead();
}

void MainWindow::showOperatorForm()
{
    ui->stackedWidget->setCurrentWidget(operatorForm.get());
    this->show();
    operatorForm->initialRead();
}

void MainWindow::backToAuth()
{
    this->close();
    authForm->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
