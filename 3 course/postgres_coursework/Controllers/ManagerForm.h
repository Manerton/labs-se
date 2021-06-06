#ifndef MANAGERFORM_H
#define MANAGERFORM_H

#include <QWidget>
#include "Models/Database.h"

namespace Ui {
    class ManagerForm;
}

class ManagerForm : public QWidget
{
    Q_OBJECT
    Ui::ManagerForm *ui;

    Database& db;
public:
    explicit ManagerForm(QWidget *parent, Database& _db);
    ~ManagerForm();
};

#endif // MANAGERFORM_H
