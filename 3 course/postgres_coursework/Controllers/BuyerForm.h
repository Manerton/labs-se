#ifndef BUYERFORM_H
#define BUYERFORM_H

#include <memory>
#include <QWidget>
#include "Models/Database.h"
#include "Models/ProductRepository.h"
#include "Models/ManufacturerRepository.h"
#include "Models/CategoryRepository.h"

class CustomProxyModel;
class CartForm;

namespace Ui {
    class BuyerForm;
}

class BuyerForm : public QWidget
{
    Q_OBJECT
    Ui::BuyerForm *ui;

    Database& db;
    std::unique_ptr<CustomProxyModel> proxyModel;
    ProductRepository productRepository;
    ProductModel parseProductModel() const;

    ManufacturerRepository manufacturerRepository;
    CategoryRepository categoryRepository;

    std::unique_ptr<CartForm> cartForm;

    void updateAttributesList();
    void clearFields();
    void clearIdField();
    int getSelectedEntryId() const;
    void moveDataToInput_product(int row, QAbstractItemModel *model);
    void clearFields_product();
    int execRemoveMessageBox();
    void read();
    void prepareUi();
    void handleOrderDone();
private slots:
    void on_pushButton_create_clicked();
    void on_pushButton_remove_clicked();
    void on_pushButton_search_clicked();
    void on_pushButton_clearInput_clicked();
    void on_pushButton_myOrders_clicked();
    void on_pushButton_cart_clicked();
    void on_tableView_activated(const QModelIndex &index);
    void on_tableView_clicked(const QModelIndex &index);

public:
    explicit BuyerForm(QWidget *parent, Database& _db);
    ~BuyerForm();
    void initialRead();
};

#endif // BUYERFORM_H
