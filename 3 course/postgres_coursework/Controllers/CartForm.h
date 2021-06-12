#ifndef CARTFORM_H
#define CARTFORM_H

#include <QWidget>
#include "Models/Database.h"
#include "Models/ProductRepository.h"
#include "Models/ProductInWarehouseRepository.h"

namespace Ui {
    class CartForm;
}

class CartForm : public QWidget
{
    Q_OBJECT

    Database& db;
    ProductRepository productRepository;
public:
    explicit CartForm(Database& _db);
    ~CartForm();

    void showCart(const std::list<int>& cart);
private slots:
    void on_pushButton_close_clicked();

private:
    Ui::CartForm *ui;
};

#endif // CARTFORM_H
