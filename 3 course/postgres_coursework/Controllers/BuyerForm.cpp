#include "BuyerForm.h"
#include "../Views/ui_BuyerForm.h"
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QJsonDocument>
#include "Models/CustomProxyModel.h"
#include "Controllers/CartForm.h"

BuyerForm::BuyerForm(QWidget *parent, Database &_db) :
    QWidget(parent),
    ui(new Ui::BuyerForm),
    db(_db),
    proxyModel(std::make_unique<CustomProxyModel>()),
    productRepository(db),
    manufacturerRepository(db),
    categoryRepository(db),
    cartForm(std::make_unique<CartForm>(db))
{
    ui->setupUi(this);
    this->prepareUi();
}

void BuyerForm::prepareUi()
{
    // для сортировки в таблице используем прокси для модели
    proxyModel->setSourceModel( db.getModel().get() );
    // привязываем проксированную модель из БД к таблице
    ui->tableView->setModel( proxyModel.get() );
    // включаем горизонтальный заголовок для таблицы характеристик товара
    ui->specs_tableWidget->horizontalHeader()->setVisible(true);
}

BuyerForm::~BuyerForm()
{
    delete ui;
}

ProductModel BuyerForm::parseProductModel() const
{
    ProductModel data;
    data.id = getSelectedEntryId();
    data.id_manufacturer = ui->manufacturer_comboBox->currentData().toInt();
    data.id_category = ui->category_comboBox->currentData().toInt();
    data.name = ui->nameProduct_LineEdit->text();
    data.cost = ui->cost_LineEdit->text().toFloat();
    data.warranty = ui->warranty_LineEdit->text().toShort();
    return data;
}

void BuyerForm::updateAttributesList()
{
    auto manufacturer_map = manufacturerRepository.getAttributesList();
    ui->manufacturer_comboBox->clear();
    ui->manufacturer_comboBox->addItem("");
    for (const auto& elem : manufacturer_map) ui->manufacturer_comboBox->addItem(elem.second, elem.first);

    auto category_map = categoryRepository.getAttributesList();
    ui->category_comboBox->clear();
    ui->category_comboBox->addItem("");
    for (const auto& elem : category_map) ui->category_comboBox->addItem(elem.second, elem.first);
}

void BuyerForm::clearIdField()
{
    ui->idProduct_LineEdit->clear();
}

void BuyerForm::clearFields()
{
    this->clearIdField();
    this->clearFields_product();
}

int BuyerForm::getSelectedEntryId() const
{
    const int row = ui->idProduct_LineEdit->text().toInt()-1;
    return ui->tableView->model()->index(row,0).data().toInt();
}

void BuyerForm::moveDataToInput_product(int row, QAbstractItemModel *model)
{
    ui->idProduct_LineEdit->setText(QString::number(row+1));

    int j = ui->category_comboBox->findText(model->index(row,1).data().toString());
    if ( j != -1 ) {
        ui->category_comboBox->setCurrentIndex(j);
    }

    int i = ui->manufacturer_comboBox->findText(model->index(row,2).data().toString());
    if ( i != -1 ) {
        ui->manufacturer_comboBox->setCurrentIndex(i);
    }

    ui->nameProduct_LineEdit->setText(model->index(row,3).data().toString());
    ui->cost_LineEdit->setText(model->index(row,4).data().toString());
    ui->warranty_LineEdit->setText(model->index(row,5).data().toString());

    ui->specs_tableWidget->clearContents();
    ui->specs_tableWidget->setRowCount(0);
    QJsonDocument doc = productRepository.getJsonSpecs(getSelectedEntryId());
    if (doc.isObject())
    {
        QVariantMap jsonMap = doc.object().toVariantMap();
        auto it = jsonMap.constBegin();
        while (it != jsonMap.constEnd())
        {
            int i = ui->specs_tableWidget->rowCount();
            ui->specs_tableWidget->insertRow(i);
            ui->specs_tableWidget->setItem(i, 0, new QTableWidgetItem(it.key()));
            ui->specs_tableWidget->setItem(i, 1, new QTableWidgetItem(it.value().toString(), static_cast<int>(it.value().type())));
            ++it;
        }
    }
}

void BuyerForm::clearFields_product()
{
    ui->manufacturer_comboBox->setCurrentIndex(0);
    ui->category_comboBox->setCurrentIndex(0);
    ui->nameProduct_LineEdit->clear();
    ui->cost_LineEdit->clear();
    ui->warranty_LineEdit->clear();
    ui->specs_tableWidget->clearContents();
    ui->specs_tableWidget->setRowCount(0);
}

int BuyerForm::execRemoveMessageBox()
{
    QMessageBox messageBox(QMessageBox::Question,
                           "Вы уверены?",
                           "Вы точно хотите удалить этот товар из корзины?",
                           QMessageBox::Yes | QMessageBox::No,
                           this);
    messageBox.setButtonText(QMessageBox::Yes, "Да");
    messageBox.setButtonText(QMessageBox::No, "Нет");
    messageBox.exec();
    return messageBox.result();
}

void BuyerForm::read()
{
    productRepository.readForBuyer();
}

void BuyerForm::initialRead()
{
    this->read();
    // прячем суррогатный ключ
    ui->tableView->hideColumn(0);
    // готовим все comboBox для внешних аттрибутов
    // (т.е помещаем в comboBox строку для пользователя
    // и там же прячем настоящий id внешнего ключа)
    this->updateAttributesList();
}

void BuyerForm::on_pushButton_create_clicked()
{
    proxyModel->insertInCart(getSelectedEntryId());
}

void BuyerForm::on_pushButton_remove_clicked()
{
    proxyModel->removeFromCart(getSelectedEntryId());
}

void BuyerForm::on_pushButton_search_clicked()
{
    productRepository.searchForBuyer(parseProductModel());
}

void BuyerForm::on_pushButton_clearInput_clicked()
{
    this->clearFields();
}

void BuyerForm::on_pushButton_myOrders_clicked()
{

}

void BuyerForm::on_pushButton_cart_clicked()
{
    this->cartForm->showCart(proxyModel->getCart());
}

void BuyerForm::on_tableView_activated(const QModelIndex &index)
{
    const int row = index.row();
    auto model = ui->tableView->model();

    this->moveDataToInput_product(row, model);
}

void BuyerForm::on_tableView_clicked(const QModelIndex &index)
{
    on_tableView_activated(index);
}

