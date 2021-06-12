#include "CartForm.h"
#include "../Views/ui_CartForm.h"
#include <QSpinBox>

CartForm::CartForm(Database &_db) :
    QWidget(nullptr),
    db(_db),
    productRepository(db),
    ui(new Ui::CartForm)

{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

void CartForm::showCart(const std::list<int>& cart)
{
    this->show();
    ui->tableWidget->setRowCount(0);
    auto product_map = productRepository.getAttributesList();
    for (const int id : cart)
    {
        int i = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(product_map[id]));

        auto SpinBox = new QSpinBox;
        // узнаем максимальное количество и устанавливаем его
        SpinBox->setMaximum(productRepository.getProductAllCount(id));
        SpinBox->setMinimum(1);
        SpinBox->setValue(1);
        ui->tableWidget->setCellWidget(i, 1, SpinBox);
    }
}

CartForm::~CartForm()
{
    delete ui;
}

void CartForm::on_pushButton_close_clicked()
{
    this->close();
}
