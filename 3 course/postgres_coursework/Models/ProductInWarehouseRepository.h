#ifndef PRODUCTINWAREHOUSEREPOSITORY_H
#define PRODUCTINWAREHOUSEREPOSITORY_H

#include <QString>
#include <QDate>
#include "IRepository.h"

struct ProductInWarehouseModel
{
    int32_t id;
    int32_t id_product;
    QDate production_date;
    int16_t count;
};

class ProductInWarehouseRepository : public IRepository<ProductInWarehouseModel>
{
    Database& db;

private:
    virtual void update(const ProductInWarehouseModel&) override
    {
        Q_UNIMPLEMENTED();
    }
public:
    ProductInWarehouseRepository(Database& _db)
        : db(_db)
    {}
    virtual void create(const ProductInWarehouseModel &data) override;
    virtual void read() const override;
    virtual void remove(int id) override;
    void search(const ProductInWarehouseModel &data);
};

#endif // PRODUCTINWAREHOUSEREPOSITORY_H
