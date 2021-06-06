#include "OperatorForm.h"
#include "ui_OperatorForm.h"
#include "QMessageBox"

OperatorForm::OperatorForm(QWidget *parent, Database &_db) :
    QWidget(parent),
    ui(new Ui::OperatorForm),
    db(_db),
    nagruzkaRepository(db)
{
    ui->setupUi(this);
    ui->tableView->setSortingEnabled(false);
    ui->tableView->setModel(db.getModel().get());
    //nagruzkaRepository.read();
    //updateAttributesList();
    ui->tableView->hideColumn(0);
}

void OperatorForm::updateAttributesList()
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

void OperatorForm::on_pushButton_create_clicked()
{
    //nagruzkaRepository.create(parse_NagruzkaModel());
    NagruzkaModel item(0, 1, 2, 25, "Физкультура", "Практика", 200);
    nagruzkaRepository.create(item);
    this->clearFields();
}

void OperatorForm::on_pushButton_update_clicked()
{
    //nagruzkaRepository.update(parse_NagruzkaModel());
    this->clearFields();
}

void OperatorForm::on_pushButton_remove_clicked()
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
        auto id = ui->tableView->model()->index(ui->lineEdit_id->text().toInt()-1,0).data().toInt();
        nagruzkaRepository.remove(id);
    }
    this->clearFields();
}
void OperatorForm::on_pushButton_search_clicked()
{
    //nagruzkaRepository.search(parse_NagruzkaModel());
    this->clearIdField();
}

void OperatorForm::on_tableView_activated(const QModelIndex &index)
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

void OperatorForm::clearIdField()
{
    ui->lineEdit_id->clear();
}

void OperatorForm::clearFields()
{
    this->clearIdField();
    ui->comboBox_lecturer->setCurrentIndex(0);
    ui->comboBox_group->setCurrentIndex(0);
    ui->lineEdit_hours->clear();
    ui->lineEdit_subject->clear();
    ui->lineEdit_type_subject->clear();
    ui->lineEdit_pay->clear();
}

OperatorForm::~OperatorForm()
{
    delete ui;
}
