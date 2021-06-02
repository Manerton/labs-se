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
      db("refresher_course"),
      nagruzkaRepository(db)
{
    ui->setupUi(this);
    ui->tableView->setSortingEnabled(false);
    ui->tableView->setModel(db.getModel().get());
    nagruzkaRepository.read();
    updateAttributesList();
    ui->tableView->hideColumn(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

NagruzkaRepository::NagruzkaModel MainWindow::parse_NagruzkaModel()
{
    NagruzkaRepository::NagruzkaModel data;
    data.id = ui->tableView->model()->index(ui->lineEdit_id->text().toInt()-1,0).data().toString();
    data.id_group = ui->comboBox_group->currentText();
    data.id_lecturer = ui->comboBox_lecturer->currentText();
    data.hours = ui->lineEdit_hours->text();
    data.subject = ui->lineEdit_subject->text();
    data.type_subject = ui->lineEdit_type_subject->text();
    data.pay = ui->lineEdit_pay->text();
    return data;
}

void MainWindow::updateAttributesList()
{
    auto map = db.getAttributesList("преподаватель");
    ui->comboBox_lecturer->clear();
    ui->comboBox_lecturer->addItem("");
    for (const auto& elem : map) ui->comboBox_lecturer->addItem(elem.second, elem.first);

    ui->comboBox_group->clear();
    ui->comboBox_group->addItem("");
    map = db.getAttributesList("группа");
    for (const auto &elem : map) ui->comboBox_group->addItem(elem.second, elem.first);
}

void MainWindow::clearFields()
{
    this->clearIdField();
    ui->comboBox_lecturer->setCurrentIndex(0);
    ui->comboBox_group->setCurrentIndex(0);
    ui->lineEdit_hours->clear();
    ui->lineEdit_subject->clear();
    ui->lineEdit_type_subject->clear();
    ui->lineEdit_pay->clear();
}

void MainWindow::clearIdField()
{
    ui->lineEdit_id->clear();
}

void MainWindow::on_pushButton_create_clicked()
{
    nagruzkaRepository.create(parse_NagruzkaModel());
    this->clearFields();
}

void MainWindow::on_pushButton_update_clicked()
{
    nagruzkaRepository.update(parse_NagruzkaModel());
    this->clearFields();
}

void MainWindow::on_pushButton_remove_clicked()
{
    QMessageBox messageBox(QMessageBox::Question,
                           "Вы уверены?",
                           "Вы точно хотите удалить эту запись?",
                           QMessageBox::Yes | QMessageBox::No,
                           this);
    messageBox.setButtonText(QMessageBox::Yes, "Да");
    messageBox.setButtonText(QMessageBox::No, "Нет");
    messageBox.exec();
    if (messageBox.result() == QMessageBox::Yes)
    {
        auto id = ui->tableView->model()->index(ui->lineEdit_id->text().toInt()-1,0).data().toString();
        nagruzkaRepository.remove(id);
    }
    this->clearFields();
}
void MainWindow::on_pushButton_search_clicked()
{
    nagruzkaRepository.search(parse_NagruzkaModel());
    this->clearIdField();
}

void MainWindow::on_tableView_activated(const QModelIndex &index)
{
    int row = index.row();
    auto model = ui->tableView->model();
    ui->lineEdit_id->setText(QString::number(row+1));
    int i = ui->comboBox_lecturer->findData(model->index(row,1).data().toString());
    if ( i != -1 ) {
        ui->comboBox_lecturer->setCurrentIndex(i);
    }
    int j = ui->comboBox_group->findData(model->index(row,2).data().toString());
    if ( j != -1 ) {
        ui->comboBox_group->setCurrentIndex(j);
    }
    ui->lineEdit_hours->setText(model->index(row,3).data().toString());
    ui->lineEdit_subject->setText(model->index(row,4).data().toString());
    ui->lineEdit_type_subject->setText(model->index(row,5).data().toString());
    ui->lineEdit_pay->setText(model->index(row,6).data().toString());
}

