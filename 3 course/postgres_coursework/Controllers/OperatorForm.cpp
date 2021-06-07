#include "OperatorForm.h"
#include "../Views/ui_OperatorForm.h"
#include "QMessageBox"
#include "QDebug"

OperatorForm::OperatorForm(QWidget *parent, Database &_db) :
    QWidget(parent),
    ui(new Ui::OperatorForm),
    db(_db),
    categoryRepository(db),
    managerRepository(db)
{
    ui->setupUi(this);
    ui->tableView->setSortingEnabled(false);
    ui->tableView->setModel(db.getModel().get());
    //updateAttributesList();
}

void OperatorForm::read(int tableIndex)
{
    const int tableName = tableIndex;
    switch (tableName)
    {
        case dbTable::category:
            categoryRepository.read();
            break;
        case dbTable::manager:
            managerRepository.read();
            break;
        default:
            qDebug() << tableName;
    }
}

void OperatorForm::initialRead()
{
    this->read(0);
    ui->tableView->hideColumn(0);
}

void OperatorForm::on_tabWidget_currentChanged(int index)
{
    this->read(index);
}

int OperatorForm::getSelectedEntryId() const
{
    const int tableName = ui->tabWidget->currentIndex();
    int value = 0;
    switch (tableName)
    {
        case dbTable::category:
            value = ui->idCategory_LineEdit->text().toInt()-1;
            break;
        case dbTable::manager:
            value = ui->idManager_LineEdit->text().toInt()-1;
            break;
        default:
            qDebug() << tableName;
    }
    return ui->tableView->model()->index(value,0).data().toInt();
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

CategoryModel OperatorForm::parseCategoryModel() const
{
    CategoryModel data;
    data.id = getSelectedEntryId();
    data.name = ui->nameCategory_LineEdit->text();
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

    const int tableName = ui->tabWidget->currentIndex();
    switch (tableName)
    {
        case dbTable::category:
            categoryRepository.create(parseCategoryModel());
            break;
        case dbTable::manager:
            managerRepository.create(parseManagerModel());
            break;
        default:
            qDebug() << tableName;
    }
}

void OperatorForm::on_pushButton_update_clicked()
{
    const int tableName = ui->tabWidget->currentIndex();
    switch (tableName)
    {
        case dbTable::category:
            categoryRepository.update(parseCategoryModel());
            break;
        case dbTable::manager:
            managerRepository.update(parseManagerModel());
            break;
        default:
            qDebug() << tableName;
    }
}

int OperatorForm::execRemoveMessageBox()
{
    QMessageBox messageBox(QMessageBox::Question,
                           "Вы уверены?",
                           "Вы точно хотите удалить эту запись?",
                           QMessageBox::Yes | QMessageBox::No,
                           this);
    messageBox.setButtonText(QMessageBox::Yes, "Да");
    messageBox.setButtonText(QMessageBox::No, "Нет");
    messageBox.exec();
    return messageBox.result();
}

void OperatorForm::on_pushButton_remove_clicked()
{
    auto choice = this->execRemoveMessageBox();
    if (choice == QMessageBox::Yes)
    {
        const int tableName = ui->tabWidget->currentIndex();
        switch (tableName)
        {
            case dbTable::category:
                categoryRepository.remove(getSelectedEntryId());
                break;
            case dbTable::manager:
                managerRepository.remove(getSelectedEntryId());
                break;
            default:
                qDebug() << tableName;
        }
    }
    this->clearFields();
}
void OperatorForm::on_pushButton_search_clicked()
{
    const int tableName = ui->tabWidget->currentIndex();
    switch (tableName)
    {
        case dbTable::category:
            categoryRepository.search(parseCategoryModel());
            break;
        case dbTable::manager:
            managerRepository.search(parseManagerModel());
            break;
        default:
            qDebug() << tableName;
    }
    this->clearIdField();
}

void OperatorForm::moveDataToInput_manager(int row, QAbstractItemModel* model)
{
    ui->idManager_LineEdit->setText(QString::number(row+1));
    ui->lastName_LineEdit->setText(model->index(row,1).data().toString());
    ui->firstName_LineEdit->setText(model->index(row,2).data().toString());
    ui->otchestvo_LineEdit->setText(model->index(row,3).data().toString());
    ui->telephone_LineEdit->setText(model->index(row,4).data().toString());
    ui->email_LineEdit->setText (model->index(row,5).data().toString());
    ui->password_LineEdit->setText(model->index(row,6).data().toString());
}

void OperatorForm::moveDataToInput_category(int row, QAbstractItemModel* model)
{
    ui->idCategory_LineEdit->setText(QString::number(row+1));
    ui->nameCategory_LineEdit->setText(model->index(row,1).data().toString());
}

void OperatorForm::on_tableView_activated(const QModelIndex &index)
{
    const int row = index.row();
    auto model = ui->tableView->model();

    const int tableName = ui->tabWidget->currentIndex();
    switch (tableName)
    {
        case dbTable::category:
            moveDataToInput_category(row, model);
            break;
        case dbTable::manager:
            moveDataToInput_manager(row, model);
            break;
        default:
            qDebug() << tableName;
    }

    /*int i = ui->comboBox_lecturer->findData(model->index(row,1).data().toString());
    if ( i != -1 ) {
        ui->comboBox_lecturer->setCurrentIndex(i);
    }*/
}

void OperatorForm::clearIdField()
{
    const int tableName = ui->tabWidget->currentIndex();
    switch (tableName)
    {
        case dbTable::category:
            ui->idCategory_LineEdit->clear();
            break;
        case dbTable::manager:
            ui->idManager_LineEdit->clear();
            break;
        default:
            qDebug() << tableName;
    }
}

void OperatorForm::clearFields_manager()
{
    ui->lastName_LineEdit->clear();
    ui->firstName_LineEdit->clear();
    ui->otchestvo_LineEdit->clear();
    ui->telephone_LineEdit->clear();
    ui->email_LineEdit->clear();
    ui->password_LineEdit->clear();
}

void OperatorForm::clearFields_category()
{
    ui->nameCategory_LineEdit->clear();
}

void OperatorForm::clearFields()
{
    this->clearIdField();

    const int tableName = ui->tabWidget->currentIndex();
    switch (tableName)
    {
        case dbTable::category:
            this->clearFields_category();
            break;
        case dbTable::manager:
            this->clearFields_manager();
            break;
        default:
            qDebug() << tableName;
    }
}

OperatorForm::~OperatorForm()
{
    delete ui;
}

void OperatorForm::on_pushButton_clearInput_clicked()
{
    this->clearFields();
}

