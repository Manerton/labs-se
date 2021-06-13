#ifndef CREATEORDERFORM_H
#define CREATEORDERFORM_H

#include <QWidget>
#include "Models/Database.h"
#include "Models/DeliveryPointRepository.h"
#include "Models/ClientRepository.h"
#include "Models/OrderRepository.h"

class SmsConfirmDialog;

namespace Ui {
    class CreateOrderForm;
}

class CreateOrderForm : public QWidget
{
    using key = int;
    using count = int;

    Q_OBJECT
    Ui::CreateOrderForm *ui;

    Database& db;
    std::map<key, count>& cart;
    DeliveryPointRepository deliveryPointRepository;

    ClientRepository clientRepository;
    ClientModel parseClientModel() const;
    int id_client = 0;

    OrderRepository orderRepository;
    OrderModel parseOrderModel() const;

    std::unique_ptr<SmsConfirmDialog> smsConfirmDialog;

    bool createOrder();
signals:
    void orderDone();
public:
    explicit CreateOrderForm(Database& _db, std::map<key, count>& _cart);
    ~CreateOrderForm();
    void showForm(double finalCost);
    void updateAttributesList();
private slots:
    void on_pushButton_close_clicked();

    void on_deliveryPoint_comboBox_currentIndexChanged(int index);
    void on_pushButton_confirm_clicked();
};

#endif // CREATEORDERFORM_H
