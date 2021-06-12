#include "CreateOrderForm.h"
#include "../Views/ui_CreateOrderForm.h"
#include <QRegExpValidator>
#include "Controllers/SmsConfirmDialog.h"
#include <QDebug>

ClientModel CreateOrderForm::parseClientModel() const
{
    ClientModel data;
    data.lastname = ui->lastName_LineEdit->text();
    data.firstname = ui->firstName_LineEdit->text();
    data.otchestvo = ui->otchestvo_LineEdit->text();
    data.telephone = ui->telephone_LineEdit->text();
    data.email = ui->email_LineEdit->text();
    return data;
}

CreateOrderForm::CreateOrderForm(Database& _db) :
    QWidget(nullptr),
    ui(new Ui::CreateOrderForm),
    db(_db),
    deliveryPointRepository(db),
    clientRepository(db),
    smsConfirmDialog(std::make_unique<SmsConfirmDialog>())
{
    ui->setupUi(this);
    // только цифры для телефона
    ui->telephone_LineEdit->setValidator( new QRegExpValidator(QRegExp("[0-9]*"), this) );
}

void CreateOrderForm::updateAttributesList()
{
    auto deliveryPoint_map = deliveryPointRepository.getAttributesList();
    ui->deliveryPoint_comboBox->clear();
    ui->deliveryPoint_comboBox->addItem("");
    for (const auto& elem : deliveryPoint_map) ui->deliveryPoint_comboBox->addItem(elem.second, elem.first);
}

void CreateOrderForm::showForm()
{
    this->show();
    if (ui->deliveryPoint_comboBox->count() == 0)
    {
        this->updateAttributesList();
    }
}

CreateOrderForm::~CreateOrderForm()
{
    delete ui;
}

void CreateOrderForm::on_pushButton_close_clicked()
{
    this->close();
}


void CreateOrderForm::on_deliveryPoint_comboBox_currentIndexChanged(int)
{
    const int id = ui->deliveryPoint_comboBox->currentData().toInt();
    ui->extraFunc_comboBox->clear();
    if (id)
    {
        QString extraFuncStr = deliveryPointRepository.getExtraFuncForId(id);
        // помещаем массив значений в comboBox
        if (!extraFuncStr.isEmpty())
        {
            QStringList list = extraFuncStr.split(" | ");
            ui->extraFunc_comboBox->addItems(list);
        }
    }
}


void CreateOrderForm::on_pushButton_confirm_clicked()
{
    db.transaction();
    bool result = clientRepository.create(parseClientModel());

    // если не было ошибки в запросе, проверяем "СМС"
    if (result)
    {
        smsConfirmDialog->clear();
        smsConfirmDialog->exec();
        if (smsConfirmDialog->isConfirmed())
        {
            db.commit();
            emit orderDone();
            this->close();
        }
        else db.rollback();
    }
    else db.rollback();
}

