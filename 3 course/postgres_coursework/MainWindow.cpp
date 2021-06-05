#include "MainWindow.h"
#include "./ui_mainwindow.h"
#include <cstdint>
#include <memory>
#include <QTableView>
#include <QMessageBox>
#include <QDebug>

/* для сортировки в таблице
    #include <QSortFilterProxyModel>
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel();
    proxyModel->setSourceModel( db.getModel().get() );
    ui->tableView->setModel( proxyModel );
*/

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      db(),
      authForm(make_unique<AuthForm>(db)),              // создаем форму для авторизации
      operatorForm(make_unique<OperatorForm>(this,db))  // создаем форму для оператора
{
    ui->setupUi(this);
    ui->stackedWidget->addWidget(operatorForm.get());

    // привязываем сигналы к слотам
    connect(authForm.get(), &AuthForm::authBuyerDone, this, &MainWindow::showBuyerForm);
    connect(authForm.get(), &AuthForm::authOperatorDone, this, &MainWindow::showOperatorForm);

    // показываем окно авторизации
    authForm->show();
}

void MainWindow::showOperatorForm()
{
    ui->stackedWidget->setCurrentWidget(operatorForm.get());
    this->show();
}

void MainWindow::showBuyerForm()
{
    this->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*Repository::NagruzkaModel MainWindow::parse_NagruzkaModel()
{
    Repository::NagruzkaModel data;
    data.id = ui->tableView->model()->index(ui->lineEdit_id->text().toInt()-1,0).data().toString();
    data.id_group = ui->comboBox_group->currentText();
    data.id_lecturer = ui->comboBox_lecturer->currentText();
    data.hours = ui->lineEdit_hours->text();
    data.subject = ui->lineEdit_subject->text();
    data.type_subject = ui->lineEdit_type_subject->text();
    data.pay = ui->lineEdit_pay->text();
    return data;
}*/

