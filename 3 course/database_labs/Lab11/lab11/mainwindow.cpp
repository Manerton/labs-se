#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <memory>
#include <QTableView>
#include <QtSql/QtSql>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), db("lab11")
{
    ui->setupUi(this);
    ui->tableView->setModel(db.getModel().get());
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_insert_clicked()
{
    db.insert(parseTableElem());
}

Database::tableElem MainWindow::parseTableElem()
{
    Database::tableElem newRow;
    newRow.ID = ui->lineEdit_ID->text().toUInt();
    newRow.eduYear = ui->lineEdit_eduYear->text().toUShort();
    newRow.groupID = ui->lineEdit_groupID->text().toUInt();
    newRow.hourCost = ui->lineEdit_hourCost->text().toDouble();
    newRow.hours = ui->lineEdit_hours->text().toUShort();
    newRow.lesson = ui->lineEdit_lesson->text();
    newRow.prepod = ui->lineEdit_prepod->text();
    return newRow;
}

void MainWindow::on_pushButton_update_clicked()
{
    db.update(parseTableElem());
}

void MainWindow::on_pushButton_erase_clicked()
{
    db.erase(parseTableElem());
}

void MainWindow::on_pushButton_search_clicked()
{
    db.search(parseTableElem());
}
