#ifndef OPERATORFORM_H
#define OPERATORFORM_H

#include <QWidget>
#include "Models/Database.h"
#include "Models/ManagerRepository.h"
#include "Models/CategoryRepository.h"
#include "Models/ManufacturerRepository.h"

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
        manufacturer = 2
    };

    Database& db;
    CategoryRepository categoryRepository;
    CategoryModel parseCategoryModel() const;

    ManagerRepository managerRepository;
    ManagerModel parseManagerModel() const;

    ManufacturerRepository manufacturerRepository;
    ManufacturerModel parseManufacturerModel() const;

    void updateAttributesList();
    void clearFields();
    void clearIdField();
    int getSelectedEntryId() const;
    void moveDataToInput_category(int row, QAbstractItemModel *model);
    void moveDataToInput_manager(int row, QAbstractItemModel *model);
    void moveDataToInput_manufacturer(int row, QAbstractItemModel *model);
    void clearFields_manager();
    void clearFields_category();
    void clearFields_manufacturer();
    int execRemoveMessageBox();
    void read(int tableIndex);
private slots:
    void on_pushButton_create_clicked();
    void on_pushButton_update_clicked();
    void on_pushButton_remove_clicked();
    void on_pushButton_search_clicked();
    void on_tableView_activated(const QModelIndex &index);
    void on_pushButton_clearInput_clicked();
    void on_tabWidget_currentChanged(int index);

public:
    explicit OperatorForm(QWidget *parent, Database& _db);
    ~OperatorForm();
    void initialRead();
};

#endif // OPERATORFORM_H
