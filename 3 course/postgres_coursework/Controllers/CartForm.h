#ifndef CARTFORM_H
#define CARTFORM_H

#include <QWidget>
#include "Models/Database.h"
#include "Models/ProductRepository.h"
#include "Models/ProductInWarehouseRepository.h"

class CreateOrderForm;

namespace Ui {
    class CartForm;
}

class CartForm : public QWidget
{
    using key = int;
    using count = int;

    Q_OBJECT
    Ui::CartForm *ui;

    Database& db;
    ProductRepository productRepository;
    std::map<key, count> current_cart;
    double finalCost = 0;

    std::unique_ptr<CreateOrderForm> createOrderForm;

    void clearCart();
    void createCart(const std::list<int>& cart);
    void handleOrderDone();

signals:
    void orderDone();
public:
    explicit CartForm(Database& _db);
    ~CartForm();

    void showCart(const std::list<int>& cart);
private slots:
    void on_pushButton_close_clicked();
    void on_pushButton_confirm_clicked();
};

#endif // CARTFORM_H
