#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <memory>
#include <QTableView>
#include <QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), db("lombard")
{
    ui->setupUi(this);
    ui->tableView->setModel(db.getModel().get());
    // получим аттрибуты для ComboBox для клиента и сотрудника
    getAttributesList_client();
    getAttributesList_worker();
    // выведем для начала все
    on_pushButton_product_clicked();
    ui->tableView->hideColumn(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getAttributesList_client()
{
    // получаем от БД ключ и строку (ФИО)
    auto map = db.getAttributesList("Клиент");
    ui->comboBox_client->addItem("");
    for (auto &elem : map)
    {
        // добавляем строку, ключ запоминаем тут, чтобы потом получить через метод combobox->currentData()
        ui->comboBox_client->addItem(elem.second,elem.first);
    }
    // сортируем по алфавиту
    ui->comboBox_client->model()->sort(0);
}
void MainWindow::getAttributesList_worker()
{
    auto map = db.getAttributesList("Сотрудник");
    ui->comboBox_worker->addItem("");
    for (auto &elem : map)
    {
        ui->comboBox_worker->addItem(elem.second,elem.first);
    }
    ui->comboBox_worker->model()->sort(0);
}

void MainWindow::on_pushButton_product_clicked()
{
    ui->tableView->hideColumn(0);
    db.searchByProduct(ui->lineEdit_product->text());
}

void MainWindow::on_pushButton_client_clicked()
{
    ui->tableView->hideColumn(0);
    db.searchByClient(ui->comboBox_client->currentData().toInt());
}

void MainWindow::on_pushButton_worker_clicked()
{
    ui->tableView->hideColumn(0);
    db.searchByWorker(ui->comboBox_worker->currentData().toInt());
}

void MainWindow::on_comboBox_client_currentIndexChanged(int index)
{
    if (index != 0)
    {
        if (ui->comboBox_worker->currentIndex() != 0) ui->comboBox_worker->setCurrentIndex(0);
        ui->lineEdit_product->clear();
    }
}

void MainWindow::on_comboBox_worker_currentIndexChanged(int index)
{
    if (index != 0)
    {
        if (ui->comboBox_client->currentIndex() != 0) ui->comboBox_client->setCurrentIndex(0);
        ui->lineEdit_product->clear();
    }
}

void MainWindow::on_lineEdit_product_textChanged(const QString &arg1)
{
    if (arg1.length() == 1)
    {
        if (ui->comboBox_worker->currentIndex() != 0) ui->comboBox_worker->setCurrentIndex(0);
        if (ui->comboBox_client->currentIndex() != 0) ui->comboBox_client->setCurrentIndex(0);
    }
}

void MainWindow::on_pushButton_date_clicked()
{
    ui->tableView->hideColumn(0);
    db.searchByDate(ui->dateEdit_date->date().toString(Qt::DateFormat::ISODate));
}

void MainWindow::on_dateEdit_date_userDateChanged(const QDate &date)
{
    if (ui->comboBox_worker->currentIndex() != 0) ui->comboBox_worker->setCurrentIndex(0);
    if (ui->comboBox_client->currentIndex() != 0) ui->comboBox_client->setCurrentIndex(0);
    ui->lineEdit_product->clear();
}

void MainWindow::on_pushButton_toSold_clicked()
{
    ui->tableView->hideColumn(0);
    db.showExpired(ui->dateEdit_start->date().toString(Qt::DateFormat::ISODate),ui->dateEdit_end->date().toString(Qt::DateFormat::ISODate));
}

void MainWindow::on_pushButton_profit_clicked()
{
    ui->tableView->showColumn(0);
    db.showProfit(ui->dateEdit_start->date().toString(Qt::DateFormat::ISODate),ui->dateEdit_end->date().toString(Qt::DateFormat::ISODate));
}
