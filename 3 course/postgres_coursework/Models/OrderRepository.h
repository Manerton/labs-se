#ifndef ORDERREPOSITORY_H
#define ORDERREPOSITORY_H

#include <QString>
#include "IRepository.h"

struct OrderModel
{
    int id;
    int id_client;
    int id_deliveryPoint;
};

struct OrderItemModel
{
    int id_order;
    int id_product;
    int count;
};

struct SearchOrderModel
{
    int id;
    QString clientSurname;
    QString clientTel;
};

class OrderRepository : public IRepository<OrderModel>
{
    Database& db;

    virtual bool update(const OrderModel&) override
    { Q_UNIMPLEMENTED(); return false; }

public:
    OrderRepository(Database& _db)
        : db(_db)
    {}
    virtual bool create(const OrderModel &data) override;
    virtual bool read() const override;
    bool readOrderDetail(int id, std::shared_ptr<QSqlQueryModel> model) const;
    bool createOrderItem(const OrderItemModel &data);
    virtual bool remove(int id) override;
    bool search(const SearchOrderModel &data);
    bool issue(int id);
};

#endif // ORDERREPOSITORY_H
