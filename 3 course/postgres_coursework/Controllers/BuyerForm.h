#ifndef BUYERFORM_H
#define BUYERFORM_H

#include <QWidget>
#include "Models/Database.h"

namespace Ui {
    class BuyerForm;
}

class BuyerForm : public QWidget
{
    Q_OBJECT
    Ui::BuyerForm *ui;

    Database& db;
public:
    explicit BuyerForm(QWidget *parent, Database& _db);
    ~BuyerForm();
};

#endif // BUYERFORM_H
