#ifndef CREATEORDERFORM_H
#define CREATEORDERFORM_H

#include <QWidget>
#include "Models/Database.h"
#include "Models/DeliveryPointRepository.h"
#include "Models/ClientRepository.h"

class SmsConfirmDialog;

namespace Ui {
    class CreateOrderForm;
}

class CreateOrderForm : public QWidget
{
    Q_OBJECT
    Ui::CreateOrderForm *ui;

    Database& db;
    DeliveryPointRepository deliveryPointRepository;

    ClientRepository clientRepository;
    ClientModel parseClientModel() const;

    std::unique_ptr<SmsConfirmDialog> smsConfirmDialog;

signals:
    void orderDone();
public:
    explicit CreateOrderForm(Database& _db);
    ~CreateOrderForm();
    void showForm();
    void updateAttributesList();
private slots:
    void on_pushButton_close_clicked();

    void on_deliveryPoint_comboBox_currentIndexChanged(int index);
    void on_pushButton_confirm_clicked();
};

#endif // CREATEORDERFORM_H
