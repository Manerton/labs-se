#include "CartForm.h"
#include "../Views/ui_CartForm.h"
#include <QSpinBox>
#include "Controllers/CreateOrderForm.h"

CartForm::CartForm(Database &_db) :
    QWidget(nullptr),
    ui(new Ui::CartForm),
    db(_db),
    productRepository(db),
    createOrderForm(std::make_unique<CreateOrderForm>(db))

{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    connect(createOrderForm.get(), &CreateOrderForm::orderDone, this, &CartForm::close);
}

void CartForm::createCart(const std::list<int>& cart)
{
    current_cart = cart;
    this->clearCart();
    auto product_map = productRepository.getAttributesListWithPrices();
    for (const int id : cart)
    {
        const int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        const QString productName = product_map[id].first;
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(product_map[id].first));

        const float cost = product_map[id].second;
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(cost)));

        auto SpinBox = new QSpinBox;
        // узнаем максимальное количество и устанавливаем его
        SpinBox->setMaximum(productRepository.getProductAllCount(id));
        SpinBox->setMinimum(1);
        SpinBox->setValue(1);

        // меняем стоимость
        connect(SpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int newCount)
            {
                const float newPrice = cost * static_cast<float>(newCount);
                ui->tableWidget->item(row, 1)->setText(QString::number(newPrice));
            });

        ui->tableWidget->setCellWidget(row, 2, SpinBox);
    }
}

void CartForm::showCart(const std::list<int>& cart)
{
    this->show();
    if (cart != current_cart)
        createCart(cart);
}

void CartForm::clearCart()
{
    ui->tableWidget->setRowCount(0);
}

CartForm::~CartForm()
{
    delete ui;
}

void CartForm::on_pushButton_close_clicked()
{
    this->close();
}
void CartForm::on_pushButton_confirm_clicked()
{
    if (!current_cart.empty())
        createOrderForm->showForm();
}

