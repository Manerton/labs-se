#include "OrderDetailsForm.h"
#include "../Views/ui_OrderDetailsForm.h"

OrderDetailsForm::OrderDetailsForm(Database &_db) :
    QWidget(nullptr),
    ui(new Ui::OrderDetailsForm),
    db(_db),
    orderRepository(db),
    model(std::make_shared<QSqlQueryModel>())
{
    ui->setupUi(this);
    ui->tableView->setModel(model.get());
}

OrderDetailsForm::~OrderDetailsForm()
{
    delete ui;
}

void OrderDetailsForm::showDetails(int id)
{
    this->show();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    orderRepository.readOrderDetail(id, model);
    // скрываем суррогатный ключ
    ui->tableView->hideColumn(0);
}

void OrderDetailsForm::on_pushButton_close_clicked()
{
    this->close();
}

