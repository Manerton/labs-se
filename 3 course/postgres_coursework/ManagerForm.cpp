#include "ManagerForm.h"
#include "ui_ManagerForm.h"

ManagerForm::ManagerForm(QWidget *parent, Database &_db) :
    QWidget(parent),
    ui(new Ui::ManagerForm),
    db(_db)
{
    ui->setupUi(this);
}

ManagerForm::~ManagerForm()
{
    delete ui;
}
