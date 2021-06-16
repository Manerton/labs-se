#include "SmsConfirmDialog.h"
#include "../Views/ui_SmsConfirmDialog.h"
#include <QMessageBox>

SmsConfirmDialog::SmsConfirmDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SmsConfirmDialog)
{
    ui->setupUi(this);
}

SmsConfirmDialog::~SmsConfirmDialog()
{
    delete ui;
}

void SmsConfirmDialog::on_buttonBox_accepted()
{
    confirmed = (ui->lineEdit->text() == "1111");
    if (!confirmed) QMessageBox::critical(nullptr, "Ошибка", "Вы ввели неправильный код!");

}

bool SmsConfirmDialog::isConfirmed() const
{
    return confirmed;
}

void SmsConfirmDialog::clear()
{
    confirmed = false;
    ui->lineEdit->clear();
}
