#include "OrderRepository.h"
#include "StringTools.h"

using namespace StringTools;

bool OrderRepository::create(const OrderModel &data)
{
    db.prepare("SELECT create_order(:id_client, :id_deliveryPoint)");
    db.bindValue(":id_client", convertIfNull(data.id_client));
    db.bindValue(":id_deliveryPoint", convertIfNull(data.id_deliveryPoint));

    return db.exec();
}

bool OrderRepository::read() const
{
    return db.execWithDisplay("SELECT * FROM заказ_v");
}

bool OrderRepository::readOrderDetail(int id, std::shared_ptr<QSqlQueryModel> model) const
{
    return db.execWithDisplayToOtherModel("SELECT * FROM детали_заказа_v WHERE id_заказ = " + QString::number(id), model);
}

bool OrderRepository::createOrderItem(const OrderItemModel &data)
{
    db.prepare("SELECT add_product_to_order(:id_order, :id_product, :count)");
    db.bindValue(":id_order", convertIfNull(data.id_order));
    db.bindValue(":id_product", convertIfNull(data.id_product));
    db.bindValue(":count", convertIfNull(data.count));

    return db.exec();
}

bool OrderRepository::remove(int id)
{
    bool result = false;
    if (id)
    {
        db.prepare("DELETE FROM заказ WHERE id_заказ = (:id)");
        db.bindValue(":id", id);

        result = db.exec();
        if (result) this->read();
    }
    return result;
}

bool OrderRepository::search(const SearchOrderModel &data)
{
    Tokens searchOptions;
    QString query = "SELECT * FROM заказ_v WHERE ";

    if (!data.clientSurname.isEmpty())
        searchOptions.emplace_back("\"Фамилия клиента\" ILIKE :surname");

    if (!data.clientTel.isEmpty())
        searchOptions.emplace_back("\"Телефон клиента\" ILIKE :tel");

    const size_t N = searchOptions.size();
    if (!searchOptions.empty())
    {
        for (size_t i = 0; i < N-1; ++i)
        {
            query += searchOptions[i] + " AND ";
        }
        query += searchOptions[N-1];

        db.prepare(query);

        db.bindValue(":surname", "%" + data.clientSurname + "%");
        db.bindValue(":tel", "%" + data.clientTel + "%");

        db.execWithDisplay();
        return true;
    }
    return false;
}

bool OrderRepository::issue(int id)
{
    bool result = false;
    if (id)
    {
        db.prepare("SELECT issue_order(:id)");
        db.bindValue(":id", id);

        result = db.exec();
        if (result) this->read();
    }
    return result;
}
