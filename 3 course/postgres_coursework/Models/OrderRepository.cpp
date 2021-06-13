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
