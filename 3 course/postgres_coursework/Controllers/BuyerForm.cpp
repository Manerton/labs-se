#include "BuyerForm.h"
#include "../Views/ui_BuyerForm.h"

BuyerForm::BuyerForm(QWidget *parent, Database &_db) :
    QWidget(parent),
    ui(new Ui::BuyerForm),
    db(_db)
{
    ui->setupUi(this);
}

BuyerForm::~BuyerForm()
{
    delete ui;
}
