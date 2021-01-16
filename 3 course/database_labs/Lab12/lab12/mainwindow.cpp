#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <memory>
#include <QTableView>
#include <QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), db("refresher_course")
{
    ui->setupUi(this);
    ui->tableView->setModel(db.getModel().get());
    db.select(db.get_dbTableName(Database::dbTableName::Nagruzka));
    updateAttributesList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

Database::nagruzka_tableElem MainWindow::parse_nagruzka_TableElem()
{
    Database::nagruzka_tableElem newRow;
    newRow.ID = ui->lineEdit_ID->text();
    newRow.groupID = ui->comboBox_group->currentText();
    newRow.prepodID = ui->comboBox_prepod->currentText();
    newRow.hours = ui->lineEdit_hours->text();
    newRow.lesson = ui->lineEdit_lesson->text();
    newRow.typeLesson = ui->lineEdit_typeLesson->text();
    newRow.pay = ui->lineEdit_pay->text();
    return newRow;
}

Database::group_tableElem MainWindow::parse_group_TableElem()
{
    Database::group_tableElem newRow;
    newRow.ID = ui->lineEdit_groupID->text();
    newRow.specialnost = ui->lineEdit_specialnost->text();
    newRow.otdelenie = ui->lineEdit_otdelenie->text();
    newRow.count = ui->lineEdit_count->text();
    return newRow;
}

Database::prepod_tableElem MainWindow::parse_prepod_TableElem()
{
    Database::prepod_tableElem newRow;
    newRow.ID = ui->lineEdit_prepodID->text();
    newRow.exp = ui->lineEdit_exp->text();
    newRow.familiya = ui->lineEdit_familiya->text();
    newRow.name = ui->lineEdit_name->text();
    newRow.otchestvo = ui->lineEdit_otchestvo->text();
    newRow.phone = ui->lineEdit_phone->text();
    return newRow;
}

void MainWindow::updateAttributesList()
{
    auto vec = db.getAttributesList("Преподаватель");
    ui->comboBox_prepod->clear();
    ui->comboBox_prepod->addItem("");
    for (uint64_t i : vec) ui->comboBox_prepod->addItem(QString::number(i));
    ui->comboBox_group->clear();
    ui->comboBox_group->addItem("");
    vec = db.getAttributesList("Группа");
    for (uint64_t i : vec) ui->comboBox_group->addItem(QString::number(i));
}

void MainWindow::on_pushButton_insert_clicked()
{
    using dbTN = Database::dbTableName;
    auto name = static_cast<Database::dbTableName>(ui->tabWidget->currentIndex());
    if (name == dbTN::Nagruzka)
    {
        db.insert(parse_nagruzka_TableElem());
    }
    else if (name == dbTN::Group)
    {
        db.insert(parse_group_TableElem());
    }
    else if (name == dbTN::Prepod)
    {
        db.insert(parse_prepod_TableElem());
    }
}

void MainWindow::on_pushButton_update_clicked()
{
    using dbTN = Database::dbTableName;
    auto name = static_cast<Database::dbTableName>(ui->tabWidget->currentIndex());
    if (name == dbTN::Nagruzka)
    {
        db.update(parse_nagruzka_TableElem());
    }
    else if (name == dbTN::Group)
    {
        db.update(parse_group_TableElem());
    }
    else if (name == dbTN::Prepod)
    {
       db.update(parse_prepod_TableElem());
    }
}

void MainWindow::on_pushButton_erase_clicked()
{
    using dbTN = Database::dbTableName;
    auto name = static_cast<Database::dbTableName>(ui->tabWidget->currentIndex());
    if (name == dbTN::Nagruzka)
    {
        db.erase(parse_nagruzka_TableElem());
    }
    else if (name == dbTN::Group)
    {
        db.erase(parse_group_TableElem());
    }
    else if (name == dbTN::Prepod)
    {
       db.erase(parse_prepod_TableElem());
    }
}
void MainWindow::on_pushButton_search_clicked()
{
    using dbTN = Database::dbTableName;
    auto name = static_cast<Database::dbTableName>(ui->tabWidget->currentIndex());
    if (name == dbTN::Nagruzka)
    {
        db.search(parse_nagruzka_TableElem());
    }
    else if (name == dbTN::Group)
    {
        db.search(parse_group_TableElem());
    }
    else if (name == dbTN::Prepod)
    {
       db.search(parse_prepod_TableElem());
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    db.select(db.get_dbTableName(static_cast<Database::dbTableName>(index)));
    updateAttributesList();
}
