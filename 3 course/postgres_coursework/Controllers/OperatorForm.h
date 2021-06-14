#ifndef OPERATORFORM_H
#define OPERATORFORM_H

#include <QWidget>
#include <QMenu>
#include "Models/Database.h"
#include "Models/ManagerRepository.h"
#include "Models/CategoryRepository.h"
#include "Models/ManufacturerRepository.h"
#include "Models/DeliveryPointRepository.h"
#include "Models/ProductRepository.h"
#include "Models/ProductInWarehouseRepository.h"

namespace Ui {
    class OperatorForm;
}

class OperatorForm : public QWidget
{
    Q_OBJECT
    Ui::OperatorForm *ui;

    enum dbTable : int
    {
        category = 0,
        manager = 1,
        manufacturer = 2,
        deliveryPoint = 3,
        product = 4,
        productInWareHouse = 5
    };

    Database& db;
    CategoryRepository categoryRepository;
    CategoryModel parseCategoryModel() const;

    ManagerRepository managerRepository;
    ManagerModel parseManagerModel() const;

    ManufacturerRepository manufacturerRepository;
    ManufacturerModel parseManufacturerModel() const;

    DeliveryPointRepository deliveryPointRepository;
    DeliveryPointModel parseDeliveryPointModel() const;

    ProductRepository productRepository;
    ProductModel parseProductModel() const;
    std::unique_ptr<QMenu> specs_contextMenu;
    std::unique_ptr<QMenu> createSpecsContextMenu();

    ProductInWarehouseRepository productInWarehouseRepository;
    ProductInWarehouseModel parseProductInWarehouseModel() const;

    void updateAttributesList();
    void clearFields();
    void clearIdField();
    int getSelectedEntryId() const;
    void moveDataToInput_category(int row, QAbstractItemModel *model);
    void moveDataToInput_manager(int row, QAbstractItemModel *model);
    void moveDataToInput_manufacturer(int row, QAbstractItemModel *model);
    void moveDataToInput_deliveryPoint(int row, QAbstractItemModel *model);
    void moveDataToInput_product(int row, QAbstractItemModel *model);
    void moveDataToInput_productInWarehouse(int row, QAbstractItemModel *model);
    void clearFields_manager();
    void clearFields_category();
    void clearFields_manufacturer();
    void clearFields_deliveryPoint();
    void clearFields_product();
    void clearFields_productInWarehouse();
    int execRemoveMessageBox();
    void read(int tableIndex);
    void prepareUi();
private slots:
    void on_pushButton_create_clicked();
    void on_pushButton_update_clicked();
    void on_pushButton_remove_clicked();
    void on_pushButton_search_clicked();
    void on_tableView_activated(const QModelIndex &index);
    void on_pushButton_clearInput_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_extraFuncAdd_pushButton_clicked();
    void on_extraFuncRemove_pushButton_clicked();
    void specs_customMenuRequested(QPoint pos);
    void specs_createRecord();
    void specs_removeRecord();
    void on_pushButton_back_clicked();
signals:
    void backToAuth();
public:
    explicit OperatorForm(QWidget *parent, Database& _db);
    ~OperatorForm();
    void initialRead();
};

#endif // OPERATORFORM_H
