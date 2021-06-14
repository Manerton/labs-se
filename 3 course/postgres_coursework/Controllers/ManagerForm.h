#ifndef MANAGERFORM_H
#define MANAGERFORM_H

#include <QWidget>
#include "Models/Database.h"
#include "Models/OrderRepository.h"

class OrderDetailsForm;

namespace Ui {
    class ManagerForm;
}

class ManagerForm : public QWidget
{
    Q_OBJECT
    Ui::ManagerForm *ui;

    Database& db;

    OrderRepository orderRepository;
    SearchOrderModel parseSearchOrderModel() const;

    std::unique_ptr<OrderDetailsForm> orderDetailsForm;

    void moveDataToInput_order(int row, QAbstractItemModel *model);
    void clearFields();
    void clearIdField();
    int getSelectedEntryId() const;
    void clearFields_order();
    void read();
    void prepareUi();
    int execMessageBox(QString msg);
private slots:
    void on_tableView_activated(const QModelIndex &index);
    void on_tableView_clicked(const QModelIndex &index);
    void on_pushButton_details_clicked();
    void on_pushButton_search_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_remove_clicked();
    void on_pushButton_issue_clicked();
    void on_pushButton_clear_clicked();
signals:
    void backToAuth();
public:
    explicit ManagerForm(QWidget *parent, Database& _db);
    ~ManagerForm();
    void initialRead();
};

#endif // MANAGERFORM_H
