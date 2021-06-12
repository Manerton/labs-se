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
    Q_OBJECT
    Ui::CartForm *ui;

    Database& db;
    ProductRepository productRepository;
    std::list<int> current_cart;

    std::unique_ptr<CreateOrderForm> createOrderForm;

    void clearCart();
    void createCart(const std::list<int>& cart);
public:
    explicit CartForm(Database& _db);
    ~CartForm();

    void showCart(const std::list<int>& cart);
private slots:
    void on_pushButton_close_clicked();
    void on_pushButton_confirm_clicked();
};

#endif // CARTFORM_H
