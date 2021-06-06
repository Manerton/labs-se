#include "OperatorForm.h"
#include "../Views/ui_OperatorForm.h"
#include "QMessageBox"

OperatorForm::OperatorForm(QWidget *parent, Database &_db) :
    QWidget(parent),
    ui(new Ui::OperatorForm),
    db(_db),
    managerRepository(db)
{
    ui->setupUi(this);
    ui->tableView->setSortingEnabled(false);
    ui->tableView->setModel(db.getModel().get());
    //updateAttributesList();
}

void OperatorForm::initialRead()
{
    managerRepository.read();
    ui->tableView->hideColumn(0);
}

int OperatorForm::getSelectedEntryId() const
{
    return ui->tableView->model()->index(ui->lineEdit_id->text().toInt()-1,0).data().toInt();
}

ManagerModel OperatorForm::parseManagerModel() const
{
    ManagerModel data;
    data.id = getSelectedEntryId();
    data.lastname = ui->lastName_LineEdit->text();
    data.firstname = ui->firstName_LineEdit->text();
    data.otchestvo = ui->otchestvo_LineEdit->text();
    data.telephone = ui->telephone_LineEdit->text();
    data.email = ui->email_LineEdit->text();
    data.password = ui->password_LineEdit->text();
    return data;
}

void OperatorForm::updateAttributesList()
{
    /*auto map = db.getAttributesList("преподаватель");
    ui->comboBox_lecturer->clear();
    ui->comboBox_lecturer->addItem("");
    for (const auto& elem : map) ui->comboBox_lecturer->addItem(elem.second, elem.first);*/
}

void OperatorForm::on_pushButton_create_clicked()
{
    managerRepository.create(parseManagerModel());
}

void OperatorForm::on_pushButton_update_clicked()
{
    managerRepository.update(parseManagerModel());
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
        managerRepository.remove(getSelectedEntryId());
    }
    this->clearFields();
}
void OperatorForm::on_pushButton_search_clicked()
{
    managerRepository.search(parseManagerModel());
    this->clearIdField();
}

void OperatorForm::on_tableView_activated(const QModelIndex &index)
{
    int row = index.row();
    auto model = ui->tableView->model();
    ui->lineEdit_id->setText(QString::number(row+1));
    ui->lastName_LineEdit->setText(model->index(row,1).data().toString());
    ui->firstName_LineEdit->setText(model->index(row,2).data().toString());
    ui->otchestvo_LineEdit->setText(model->index(row,3).data().toString());
    ui->telephone_LineEdit->setText(model->index(row,4).data().toString());
    ui->email_LineEdit->setText (model->index(row,5).data().toString());
    ui->password_LineEdit->setText(model->index(row,6).data().toString());
    /*int i = ui->comboBox_lecturer->findData(model->index(row,1).data().toString());
    if ( i != -1 ) {
        ui->comboBox_lecturer->setCurrentIndex(i);
    }*/
}

void OperatorForm::clearIdField()
{
    ui->lineEdit_id->clear();
}

void OperatorForm::clearFields()
{
    this->clearIdField();
    ui->lastName_LineEdit->clear();
    ui->firstName_LineEdit->clear();
    ui->otchestvo_LineEdit->clear();
    ui->telephone_LineEdit->clear();
    ui->email_LineEdit->clear();
    ui->password_LineEdit->clear();
}

OperatorForm::~OperatorForm()
{
    delete ui;
}

void OperatorForm::on_pushButton_clearInput_clicked()
{
    this->clearFields();
}

