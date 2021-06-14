#ifndef ORDERDETAILSFORM_H
#define ORDERDETAILSFORM_H

#include <QWidget>
#include "Models/Database.h"
#include "Models/OrderRepository.h"

namespace Ui {
    class OrderDetailsForm;
}

class OrderDetailsForm : public QWidget
{
    Q_OBJECT
    Ui::OrderDetailsForm *ui;

    Database& db;
    OrderRepository orderRepository;

    std::shared_ptr<QSqlQueryModel> model;

    void clear();
public:
    explicit OrderDetailsForm(Database& _db);
    ~OrderDetailsForm();
    void showDetails(int id);
private slots:
    void on_pushButton_close_clicked();
};

#endif // ORDERDETAILSFORM_H
