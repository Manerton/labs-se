#include "ManagerForm.h"
#include "../Views/ui_ManagerForm.h"
#include <QMessageBox>
#include "OrderDetailsForm.h"

ManagerForm::ManagerForm(QWidget *parent, Database &_db) :
    QWidget(parent),
    ui(new Ui::ManagerForm),
    db(_db),
    orderRepository(db),
    orderDetailsForm(std::make_unique<OrderDetailsForm>(db))
{
    ui->setupUi(this);
    this->prepareUi();
}

void ManagerForm::prepareUi()
{
    ui->tableView->setModel( db.getModel().get() );
    ui->tableView->setSortingEnabled(false);
}

void ManagerForm::on_tableView_activated(const QModelIndex &index)
{
    const int row = index.row();
    auto model = ui->tableView->model();

    this->moveDataToInput_order(row, model);
}

void ManagerForm::on_tableView_clicked(const QModelIndex &index)
{
    on_tableView_activated(index);
}

ManagerForm::~ManagerForm()
{
    delete ui;
}


SearchOrderModel ManagerForm::parseSearchOrderModel() const
{
    SearchOrderModel data;
    data.id = getSelectedEntryId();
    data.clientSurname = ui->surname_LineEdit->text();
    data.clientTel = ui->telephone_LineEdit->text();
    return data;
}

void ManagerForm::moveDataToInput_order(int row, QAbstractItemModel *model)
{
    ui->id_LineEdit->setText(QString::number(row+1));
    ui->surname_LineEdit->setText(model->index(row,2).data().toString());
    ui->telephone_LineEdit->setText(model->index(row,3).data().toString());
}


void ManagerForm::clearIdField()
{
    ui->id_LineEdit->clear();
}

void ManagerForm::clearFields()
{
    this->clearIdField();
    this->clearFields_order();
}

int ManagerForm::getSelectedEntryId() const
{
    const int row = ui->id_LineEdit->text().toInt()-1;
    return ui->tableView->model()->index(row,0).data().toInt();
}

void ManagerForm::clearFields_order()
{
    ui->surname_LineEdit->clear();
    ui->telephone_LineEdit->clear();
}

void ManagerForm::read()
{
    orderRepository.read();
}

void ManagerForm::initialRead()
{
    this->read();
    ui->tableView->hideColumn(0);
}

void ManagerForm::on_pushButton_details_clicked()
{
    orderDetailsForm->showDetails(getSelectedEntryId());
}


void ManagerForm::on_pushButton_search_clicked()
{
    if ( !orderRepository.search(parseSearchOrderModel()) )
    {
        this->read();
    }
}

void ManagerForm::on_pushButton_back_clicked()
{
    emit backToAuth();
}

int ManagerForm::execMessageBox(QString msg)
{
    QMessageBox messageBox(QMessageBox::Question,
                           "Вы уверены?",
                           msg,
                           QMessageBox::Yes | QMessageBox::No,
                           this);
    messageBox.setButtonText(QMessageBox::Yes, "Да");
    messageBox.setButtonText(QMessageBox::No, "Нет");
    messageBox.exec();
    return messageBox.result();
}

void ManagerForm::on_pushButton_issue_clicked()
{
    auto choice = this->execMessageBox("Вы точно хотите выдать этот заказ?");
    if (choice == QMessageBox::Yes)
    {
        orderRepository.issue(getSelectedEntryId());
        this->clearFields();
    }
}

void ManagerForm::on_pushButton_remove_clicked()
{
    auto choice = this->execMessageBox("Вы точно хотите отменить этот заказ?");
    if (choice == QMessageBox::Yes)
    {
        orderRepository.remove(getSelectedEntryId());
        this->clearFields();
    }
}


void ManagerForm::on_pushButton_clear_clicked()
{
    this->clearFields();
}

