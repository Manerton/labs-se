#ifndef OPERATORFORM_H
#define OPERATORFORM_H

#include <QWidget>
#include "Models/Database.h"
#include "Models/NagruzkaRepository.h"

namespace Ui {
    class OperatorForm;
}

class OperatorForm : public QWidget
{
    Q_OBJECT
    Ui::OperatorForm *ui;

    Database& db;
    NagruzkaRepository nagruzkaRepository;
    void updateAttributesList();
    void clearFields();
    void clearIdField();

private slots:
    void on_pushButton_create_clicked();
    void on_pushButton_update_clicked();
    void on_pushButton_remove_clicked();
    void on_pushButton_search_clicked();
    void on_tableView_activated(const QModelIndex &index);
public:
    explicit OperatorForm(QWidget *parent, Database& _db);
    ~OperatorForm();

};

#endif // OPERATORFORM_H
