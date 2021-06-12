#include "OperatorForm.h"
#include "../Views/ui_OperatorForm.h"
#include <QMessageBox>
#include <QDebug>
#include <QMenu>
#include <QJsonDocument>
#include <QRegExpValidator>

OperatorForm::OperatorForm(QWidget *parent, Database &_db) :
    QWidget(parent),
    ui(new Ui::OperatorForm),
    db(_db),
    categoryRepository(db),
    managerRepository(db),
    manufacturerRepository(db),
    deliveryPointRepository(db),
    productRepository(db),
    specs_contextMenu(createSpecsContextMenu()),
    productInWarehouseRepository(db)
{
    ui->setupUi(this);
    this->prepareUi();
}

void OperatorForm::prepareUi()
{
    // отключаем, чтобы не сбивала с толку пользователя
    ui->tableView->setSortingEnabled(false);
    // привязываем модель из БД к таблице
    ui->tableView->setModel(db.getModel().get());
    // включаем горизонтальный заголовок для таблицы характеристик товара
    ui->specs_tableWidget->horizontalHeader()->setVisible(true);
    // связка для работы контекстного меню для таблицы характеристик товара
    connect(ui->specs_tableWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(specs_customMenuRequested(QPoint)));
    ui->productionDate_dateEdit->setMaximumDate(QDate::currentDate());
    ui->productionDate_dateEdit->setDate(ui->productionDate_dateEdit->minimumDate());
    ui->productionDate_dateEdit->setSpecialValueText("не выбрана");
    // только цифры для телефона
    ui->telephone_LineEdit->setValidator( new QRegExpValidator(QRegExp("[0-9]*"), this) );
}

void OperatorForm::specs_customMenuRequested(QPoint pos)
{
    specs_contextMenu->popup(ui->specs_tableWidget->viewport()->mapToGlobal(pos));
}

void OperatorForm::specs_createRecord()
{
    ui->specs_tableWidget->insertRow(ui->specs_tableWidget->rowCount());
}

void OperatorForm::specs_removeRecord()
{
    ui->specs_tableWidget->removeRow(ui->specs_tableWidget->currentRow());
}

void OperatorForm::read(int tableIndex)
{
    const int tableName = tableIndex;
    switch (tableName)
    {
        case dbTable::category:
            categoryRepository.read();
            break;
        case dbTable::manager:
            managerRepository.read();
            break;
        case dbTable::manufacturer:
            manufacturerRepository.read();
            break;
        case dbTable::deliveryPoint:
            deliveryPointRepository.read();
            break;
        case dbTable::product:
            productRepository.read();
            break;
        case dbTable::productInWareHouse:
            productInWarehouseRepository.read();
            break;
        default:
            qDebug() << tableName;
    }
}

void OperatorForm::initialRead()
{
    this->read(0);
    // прячем суррогатный ключ
    ui->tableView->hideColumn(0);
    // готовим все comboBox для внешних аттрибутов
    // (т.е помещаем в comboBox строку для пользователя
    // и там же прячем настоящий id внешнего ключа)
    this->updateAttributesList();
}

void OperatorForm::on_tabWidget_currentChanged(int index)
{
    this->read(index);
    this->updateAttributesList();

    if (ui->tabWidget->currentIndex() == dbTable::productInWareHouse)
    {
        ui->pushButton_update->setDisabled(true);
    }
    else
    {
        ui->pushButton_update->setDisabled(false);
    }
}

int OperatorForm::getSelectedEntryId() const
{
    const int tableName = ui->tabWidget->currentIndex();
    int value = 0;
    switch (tableName)
    {
        case dbTable::category:
            value = ui->idCategory_LineEdit->text().toInt()-1;
            break;
        case dbTable::manager:
            value = ui->idManager_LineEdit->text().toInt()-1;
            break;
        case dbTable::manufacturer:
            value = ui->idManufacturer_LineEdit->text().toInt()-1;
            break;
        case dbTable::deliveryPoint:
            value = ui->idDeliveryPoint_LineEdit->text().toInt()-1;
            break;
        case dbTable::product:
            value = ui->idProduct_LineEdit->text().toInt()-1;
            break;
        case dbTable::productInWareHouse:
            value = ui->idProductInWarehouse_LineEdit->text().toInt()-1;
            break;
        default:
            qDebug() << tableName;
    }
    return ui->tableView->model()->index(value,0).data().toInt();
}

ManagerModel OperatorForm::parseManagerModel() const
{
    ManagerModel data;
    data.id = getSelectedEntryId();
    data.lastname = ui->lastName_LineEdit->text();
    data.firstname = ui->firstName_LineEdit->text();
    data.otchestvo = ui->otchestvo_LineEdit->text();
    data.telephone = ui->telephone_LineEdit->text();
    data.email = ui->email_LineEdit->text();
    data.password = ui->password_LineEdit->text();
    return data;
}

CategoryModel OperatorForm::parseCategoryModel() const
{
    CategoryModel data;
    data.id = getSelectedEntryId();
    data.name = ui->nameCategory_LineEdit->text();
    return data;
}

ManufacturerModel OperatorForm::parseManufacturerModel() const
{
    ManufacturerModel data;
    data.id = getSelectedEntryId();
    data.name = ui->nameManufacturer_LineEdit->text();
    data.country = ui->country_LineEdit->text();
    return data;
}

DeliveryPointModel OperatorForm::parseDeliveryPointModel() const
{
    DeliveryPointModel data;
    data.id = getSelectedEntryId();
    data.name = ui->nameDeliveryPoint_LineEdit->text();
    data.address = ui->address_LineEdit->text();
    data.area = ui->area_LineEdit->text().toFloat();

    const int N = ui->extraFunc_comboBox->count();
    for (int i = 0; i < N; ++i)
    {
        data.extraFunc.emplace_back(ui->extraFunc_comboBox->itemText(i));
    }

    return data;
}

ProductModel OperatorForm::parseProductModel() const
{
    ProductModel data;
    data.id = getSelectedEntryId();
    data.id_manufacturer = ui->manufacturer_comboBox->currentData().toInt();
    data.id_category = ui->category_comboBox->currentData().toInt();
    data.name = ui->nameProduct_LineEdit->text();
    data.cost = ui->cost_LineEdit->text().toFloat();
    data.warranty = ui->warranty_LineEdit->text().toShort();
    for (int i = 0; i < ui->specs_tableWidget->rowCount(); ++i)
    {
        QString valText = ui->specs_tableWidget->item(i,1)->text();

        bool okInt = false;
        int integer = valText.toInt(&okInt);

        bool okFloat = false;
        double real = valText.toDouble(&okFloat);

        bool okBoolTrue =   ( valText.toLower().indexOf("true") != -1 );
        bool okBoolFalse =  ( valText.toLower().indexOf("false") != -1 );

        if (okInt) {
            data.specs.insert(ui->specs_tableWidget->item(i,0)->text(), integer);
        }
        else if (okFloat) {
            data.specs.insert(ui->specs_tableWidget->item(i,0)->text(), real);
        }
        else if (okBoolTrue) {
            data.specs.insert(ui->specs_tableWidget->item(i,0)->text(), true);
        }
        else if (okBoolFalse) {
            data.specs.insert(ui->specs_tableWidget->item(i,0)->text(), false);
        }
        else {
            data.specs.insert(ui->specs_tableWidget->item(i,0)->text(), valText);
        }
    }

    return data;
}

std::unique_ptr<QMenu> OperatorForm::createSpecsContextMenu()
{
    // Создаем объект контекстного меню
    auto menu = std::make_unique<QMenu>(this);
    // Создаём действия для контекстного меню
    auto createDevice = new QAction("Добавить", this);
    auto deleteDevice = new QAction("Удалить", this);
    // Подключаем слоты для действий контекстного меню
    connect(createDevice, &QAction::triggered, this, &OperatorForm::specs_createRecord);
    connect(deleteDevice, &QAction::triggered, this, &OperatorForm::specs_removeRecord);
    /* Устанавливаем действия в меню */
    menu->addAction(createDevice);
    menu->addAction(deleteDevice);
    /* Вызываем контекстное меню */
    return menu;
}

ProductInWarehouseModel OperatorForm::parseProductInWarehouseModel() const
{
    ProductInWarehouseModel data;
    data.id = getSelectedEntryId();
    data.id_product = ui->product_comboBox->currentData().toInt();

    // если не выбрана дата
    if (ui->productionDate_dateEdit->text() == ui->productionDate_dateEdit->specialValueText())
    {
        data.production_date = QDate();
    }
    else // если выбрана
    {
        data.production_date = ui->productionDate_dateEdit->date();
    }

    data.count = ui->count_LineEdit->text().toShort();
    return data;
}

void OperatorForm::updateAttributesList()
{
    auto manufacturer_map = manufacturerRepository.getAttributesList();
    ui->manufacturer_comboBox->clear();
    ui->manufacturer_comboBox->addItem("");
    for (const auto& elem : manufacturer_map) ui->manufacturer_comboBox->addItem(elem.second, elem.first);

    auto category_map = categoryRepository.getAttributesList();
    ui->category_comboBox->clear();
    ui->category_comboBox->addItem("");
    for (const auto& elem : category_map) ui->category_comboBox->addItem(elem.second, elem.first);

    auto product_map = productRepository.getAttributesList();
    ui->product_comboBox->clear();
    ui->product_comboBox->addItem("");
    for (const auto& elem : product_map) ui->product_comboBox->addItem(elem.second, elem.first);
}

void OperatorForm::on_pushButton_create_clicked()
{

    const int tableName = ui->tabWidget->currentIndex();
    switch (tableName)
    {
        case dbTable::category:
            categoryRepository.create(parseCategoryModel());
            break;
        case dbTable::manager:
            managerRepository.create(parseManagerModel());
            break;
        case dbTable::manufacturer:
            manufacturerRepository.create(parseManufacturerModel());
            break;
        case dbTable::deliveryPoint:
            deliveryPointRepository.create(parseDeliveryPointModel());
            break;
        case dbTable::product:
            productRepository.create(parseProductModel());
            break;
        case dbTable::productInWareHouse:
            productInWarehouseRepository.create(parseProductInWarehouseModel());
            break;
        default:
            qDebug() << tableName;
    }
}

void OperatorForm::on_pushButton_update_clicked()
{
    const int tableName = ui->tabWidget->currentIndex();
    switch (tableName)
    {
        case dbTable::category:
            categoryRepository.update(parseCategoryModel());
            break;
        case dbTable::manager:
            managerRepository.update(parseManagerModel());
            break;
        case dbTable::manufacturer:
            manufacturerRepository.update(parseManufacturerModel());
            break;
        case dbTable::deliveryPoint:
            deliveryPointRepository.update(parseDeliveryPointModel());
            break;
        case dbTable::product:
            productRepository.update(parseProductModel());
            break;
        default:
            qDebug() << tableName;
    }
}

int OperatorForm::execRemoveMessageBox()
{
    QMessageBox messageBox(QMessageBox::Question,
                           "Вы уверены?",
                           "Вы точно хотите удалить эту запись?",
                           QMessageBox::Yes | QMessageBox::No,
                           this);
    messageBox.setButtonText(QMessageBox::Yes, "Да");
    messageBox.setButtonText(QMessageBox::No, "Нет");
    messageBox.exec();
    return messageBox.result();
}

void OperatorForm::on_pushButton_remove_clicked()
{
    auto choice = this->execRemoveMessageBox();
    if (choice == QMessageBox::Yes)
    {
        const int tableName = ui->tabWidget->currentIndex();
        switch (tableName)
        {
            case dbTable::category:
                categoryRepository.remove(getSelectedEntryId());
                break;
            case dbTable::manager:
                managerRepository.remove(getSelectedEntryId());
                break;
            case dbTable::manufacturer:
                manufacturerRepository.remove(getSelectedEntryId());
                break;
            case dbTable::deliveryPoint:
                deliveryPointRepository.remove(getSelectedEntryId());
                break;
            case dbTable::product:
                productRepository.remove(getSelectedEntryId());
                break;
            case dbTable::productInWareHouse:
                productInWarehouseRepository.remove(getSelectedEntryId());
                break;
            default:
                qDebug() << tableName;
        }
    }
    this->clearFields();
}
void OperatorForm::on_pushButton_search_clicked()
{
    const int tableName = ui->tabWidget->currentIndex();
    switch (tableName)
    {
        case dbTable::category:
            categoryRepository.search(parseCategoryModel());
            break;
        case dbTable::manager:
            managerRepository.search(parseManagerModel());
            break;
        case dbTable::manufacturer:
            manufacturerRepository.search(parseManufacturerModel());
            break;
        case dbTable::deliveryPoint:
            deliveryPointRepository.search(parseDeliveryPointModel());
            break;
        case dbTable::product:
            productRepository.search(parseProductModel());
            break;
        case dbTable::productInWareHouse:
            productInWarehouseRepository.search(parseProductInWarehouseModel());
            break;
        default:
            qDebug() << tableName;
    }
    this->clearIdField();
}

void OperatorForm::moveDataToInput_manager(int row, QAbstractItemModel* model)
{
    ui->idManager_LineEdit->setText(QString::number(row+1));
    ui->lastName_LineEdit->setText(model->index(row,1).data().toString());
    ui->firstName_LineEdit->setText(model->index(row,2).data().toString());
    ui->otchestvo_LineEdit->setText(model->index(row,3).data().toString());
    ui->telephone_LineEdit->setText(model->index(row,4).data().toString());
    ui->email_LineEdit->setText (model->index(row,5).data().toString());
    ui->password_LineEdit->setText(model->index(row,6).data().toString());
}

void OperatorForm::moveDataToInput_manufacturer(int row, QAbstractItemModel *model)
{
    ui->idManufacturer_LineEdit->setText(QString::number(row+1));
    ui->nameManufacturer_LineEdit->setText(model->index(row,1).data().toString());
    ui->country_LineEdit->setText(model->index(row,2).data().toString());
}

void OperatorForm::moveDataToInput_deliveryPoint(int row, QAbstractItemModel *model)
{
    ui->idDeliveryPoint_LineEdit->setText(QString::number(row+1));
    ui->nameDeliveryPoint_LineEdit->setText(model->index(row,1).data().toString());
    ui->address_LineEdit->setText(model->index(row,2).data().toString());
    ui->area_LineEdit->setText(model->index(row,3).data().toString());

    QString extraFuncStr = model->index(row,4).data().toString();
    ui->extraFunc_comboBox->clear();
    // помещаем массив значений в comboBox
    if (!extraFuncStr.isEmpty())
    {
        QStringList list = extraFuncStr.split(" | ");
        ui->extraFunc_comboBox->addItems(list);
    }
}

void OperatorForm::moveDataToInput_product(int row, QAbstractItemModel *model)
{
    ui->idProduct_LineEdit->setText(QString::number(row+1));

    int i = ui->manufacturer_comboBox->findText(model->index(row,1).data().toString());
    if ( i != -1 ) {
        ui->manufacturer_comboBox->setCurrentIndex(i);
    }

    int j = ui->category_comboBox->findText(model->index(row,2).data().toString());
    if ( j != -1 ) {
        ui->category_comboBox->setCurrentIndex(j);
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

void OperatorForm::moveDataToInput_productInWarehouse(int row, QAbstractItemModel *model)
{
    ui->idProductInWarehouse_LineEdit->setText(QString::number(row+1));

    int i = ui->product_comboBox->findText(model->index(row,1).data().toString());
    if ( i != -1 ) {
        ui->product_comboBox->setCurrentIndex(i);
    }

    ui->productionDate_dateEdit->setDate(model->index(row,3).data().toDate());
    ui->count_LineEdit->setText(model->index(row,4).data().toString());
}

void OperatorForm::moveDataToInput_category(int row, QAbstractItemModel* model)
{
    ui->idCategory_LineEdit->setText(QString::number(row+1));
    ui->nameCategory_LineEdit->setText(model->index(row,1).data().toString());
}

void OperatorForm::on_tableView_activated(const QModelIndex &index)
{
    const int row = index.row();
    auto model = ui->tableView->model();

    const int tableName = ui->tabWidget->currentIndex();
    switch (tableName)
    {
        case dbTable::category:
            moveDataToInput_category(row, model);
            break;
        case dbTable::manager:
            moveDataToInput_manager(row, model);
            break;
        case dbTable::manufacturer:
            moveDataToInput_manufacturer(row, model);
            break;
        case dbTable::deliveryPoint:
            moveDataToInput_deliveryPoint(row, model);
            break;
        case dbTable::product:
            moveDataToInput_product(row, model);
            break;
        case dbTable::productInWareHouse:
            moveDataToInput_productInWarehouse(row, model);
            break;
        default:
            qDebug() << tableName;
    }
}

void OperatorForm::clearIdField()
{
    const int tableName = ui->tabWidget->currentIndex();
    switch (tableName)
    {
        case dbTable::category:
            ui->idCategory_LineEdit->clear();
            break;
        case dbTable::manager:
            ui->idManager_LineEdit->clear();
            break;
        case dbTable::manufacturer:
            ui->idManufacturer_LineEdit->clear();
            break;
        case dbTable::deliveryPoint:
            ui->idDeliveryPoint_LineEdit->clear();
            break;
        case dbTable::product:
            ui->idProduct_LineEdit->clear();
            break;
        case dbTable::productInWareHouse:
            ui->idProductInWarehouse_LineEdit->clear();
            break;
        default:
            qDebug() << tableName;
    }
}

void OperatorForm::clearFields_manager()
{
    ui->lastName_LineEdit->clear();
    ui->firstName_LineEdit->clear();
    ui->otchestvo_LineEdit->clear();
    ui->telephone_LineEdit->clear();
    ui->email_LineEdit->clear();
    ui->password_LineEdit->clear();
}

void OperatorForm::clearFields_category()
{
    ui->nameCategory_LineEdit->clear();
}

void OperatorForm::clearFields_manufacturer()
{
    ui->nameManufacturer_LineEdit->clear();
    ui->country_LineEdit->clear();
}

void OperatorForm::clearFields_deliveryPoint()
{
    ui->nameDeliveryPoint_LineEdit->clear();
    ui->address_LineEdit->clear();
    ui->area_LineEdit->clear();
    ui->extraFunc_comboBox->clear();
}

void OperatorForm::clearFields_product()
{
    ui->manufacturer_comboBox->setCurrentIndex(0);
    ui->category_comboBox->setCurrentIndex(0);
    ui->nameProduct_LineEdit->clear();
    ui->cost_LineEdit->clear();
    ui->warranty_LineEdit->clear();
    ui->specs_tableWidget->clearContents();
    ui->specs_tableWidget->setRowCount(0);
}

void OperatorForm::clearFields_productInWarehouse()
{
    ui->product_comboBox->setCurrentIndex(0);
    ui->productionDate_dateEdit->setDate(ui->productionDate_dateEdit->minimumDate());
    ui->count_LineEdit->clear();
}

void OperatorForm::clearFields()
{
    this->clearIdField();

    const int tableName = ui->tabWidget->currentIndex();
    switch (tableName)
    {
        case dbTable::category:
            this->clearFields_category();
            break;
        case dbTable::manager:
            this->clearFields_manager();
            break;
        case dbTable::manufacturer:
            this->clearFields_manufacturer();
            break;
        case dbTable::deliveryPoint:
            this->clearFields_deliveryPoint();
            break;
        case dbTable::product:
            this->clearFields_product();
            break;
        case dbTable::productInWareHouse:
            this->clearFields_productInWarehouse();
            break;
        default:
            qDebug() << tableName;
    }
}

OperatorForm::~OperatorForm()
{
    delete ui;
}

void OperatorForm::on_pushButton_clearInput_clicked()
{
    this->clearFields();
}

void OperatorForm::on_extraFuncAdd_pushButton_clicked()
{
    if (!ui->extraFunc_comboBox->currentText().trimmed().isEmpty())
    {
        if (ui->extraFunc_comboBox->findText(ui->extraFunc_comboBox->currentText().trimmed()) == -1)
        {
            ui->extraFunc_comboBox->addItem(ui->extraFunc_comboBox->currentText().trimmed());
            ui->extraFunc_comboBox->clearEditText();
        }
    }
}

void OperatorForm::on_extraFuncRemove_pushButton_clicked()
{
    ui->extraFunc_comboBox->removeItem(ui->extraFunc_comboBox->currentIndex());
}
