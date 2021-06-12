#ifndef CREATEORDERFORM_H
#define CREATEORDERFORM_H

#include <QWidget>
#include "Models/Database.h"
#include "Models/DeliveryPointRepository.h"

namespace Ui {
    class CreateOrderForm;
}

class CreateOrderForm : public QWidget
{
    Q_OBJECT
    Ui::CreateOrderForm *ui;

    Database& db;
    DeliveryPointRepository deliveryPointRepository;

public:
    explicit CreateOrderForm(Database& _db);
    ~CreateOrderForm();
    void showForm();
    void updateAttributesList();
private slots:
    void on_pushButton_close_clicked();

    void on_deliveryPoint_comboBox_currentIndexChanged(int index);
};

#endif // CREATEORDERFORM_H
