#include "CartForm.h"
#include "../Views/ui_CartForm.h"
#include <QSpinBox>
#include "Controllers/CreateOrderForm.h"

CartForm::CartForm(Database &_db) :
    QWidget(nullptr),
    ui(new Ui::CartForm),
    db(_db),
    productRepository(db),
    createOrderForm(std::make_unique<CreateOrderForm>(db, current_cart))

{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    connect(createOrderForm.get(), &CreateOrderForm::orderDone, this, &CartForm::handleOrderDone);
}

void CartForm::createCart(const std::list<int>& cart)
{
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

        current_cart[id] = 1; // количество товара в корзине по умолчанию 1
        finalCost += cost;

        // меняем стоимость
        connect(SpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
                [=](int newCount)
        {
            const float newPrice = cost * static_cast<float>(newCount);
            finalCost -= ui->tableWidget->item(row,1)->text().toDouble();
            finalCost += newPrice;

            ui->tableWidget->item(row, 1)->setText(QString::number(newPrice));
            current_cart[id] = newCount; // новое количество товара в корзине

        });

        ui->tableWidget->setCellWidget(row, 2, SpinBox);
    }
}

void CartForm::showCart(const std::list<int>& cart)
{
    this->show();
    createCart(cart);
}

void CartForm::clearCart()
{
    ui->tableWidget->setRowCount(0);
    current_cart.clear();
    finalCost = 0;
}

void CartForm::handleOrderDone()
{
    this->clearCart();
    emit orderDone();
    this->close();
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
        createOrderForm->showForm(finalCost);
}

