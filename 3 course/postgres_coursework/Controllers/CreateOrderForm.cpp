#include "CreateOrderForm.h"
#include "../Views/ui_CreateOrderForm.h"
#include <QRegExpValidator>
#include "Controllers/SmsConfirmDialog.h"
#include <QDebug>

CreateOrderForm::CreateOrderForm(Database& _db, std::map<key, count>& _cart) :
    QWidget(nullptr),
    ui(new Ui::CreateOrderForm),
    db(_db),
    cart(_cart),
    deliveryPointRepository(db),
    clientRepository(db),
    orderRepository(db),
    smsConfirmDialog(std::make_unique<SmsConfirmDialog>())
{
    ui->setupUi(this);
    // только цифры для телефона
    ui->telephone_LineEdit->setValidator( new QRegExpValidator(QRegExp("[0-9]*"), this) );
}


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

OrderModel CreateOrderForm::parseOrderModel() const
{
    OrderModel data{};
    data.id_client = this->id_client;
    data.id_deliveryPoint = ui->deliveryPoint_comboBox->currentData().toInt();
    return data;
}

void CreateOrderForm::updateAttributesList()
{
    auto deliveryPoint_map = deliveryPointRepository.getAttributesList();
    ui->deliveryPoint_comboBox->clear();
    ui->deliveryPoint_comboBox->addItem("");
    for (const auto& elem : deliveryPoint_map) ui->deliveryPoint_comboBox->addItem(elem.second, elem.first);
}

void CreateOrderForm::showForm(double finalCost)
{
    this->show();
    ui->finalCost_LineEdit->setText(QString::number(finalCost));
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


bool CreateOrderForm::createOrder()
{
    bool orderResult = orderRepository.create(parseOrderModel());
    if (orderResult)
    {
        const int id_order = db.getFirstValue(0).toInt();
        for (const auto& elem : cart)
        {
            OrderItemModel item{};
            item.id_order = id_order;
            item.id_product = elem.first;
            item.count = elem.second;
            bool orderItemResult = orderRepository.createOrderItem(item);
            if (!orderItemResult) return false;
        }
        return true;
    }
    return false;
}

void CreateOrderForm::on_pushButton_confirm_clicked()
{
    db.transaction();
    bool clientResult = clientRepository.create(parseClientModel());

    // если не было ошибки в запросе, проверяем "СМС"
    if (clientResult)
    {
        smsConfirmDialog->clear();
        smsConfirmDialog->exec();
        if (smsConfirmDialog->isConfirmed())
        {
            this->id_client = db.getFirstValue(0).toInt();
            if (this->createOrder())
            {
                db.commit();
                emit orderDone();
                id_client = 0;
                this->close();
            }
            else db.rollback();
        }
        else db.rollback();
    }
    else db.rollback();
}

