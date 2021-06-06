#ifndef OPERATORFORM_H
#define OPERATORFORM_H

#include <QWidget>
#include "Models/Database.h"
#include "Models/ManagerRepository.h"

namespace Ui {
    class OperatorForm;
}

class OperatorForm : public QWidget
{
    Q_OBJECT
    Ui::OperatorForm *ui;

    Database& db;
    ManagerRepository managerRepository;
    ManagerModel parseManagerModel() const;
    void updateAttributesList();
    void clearFields();
    void clearIdField();
    int getSelectedEntryId() const;
private slots:
    void on_pushButton_create_clicked();
    void on_pushButton_update_clicked();
    void on_pushButton_remove_clicked();
    void on_pushButton_search_clicked();
    void on_tableView_activated(const QModelIndex &index);
    void on_pushButton_clearInput_clicked();

public:
    explicit OperatorForm(QWidget *parent, Database& _db);
    ~OperatorForm();
    void initialRead();
};

#endif // OPERATORFORM_H
