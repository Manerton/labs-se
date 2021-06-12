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

    virtual bool update(const ProductInWarehouseModel&) override
    { Q_UNIMPLEMENTED(); return false; }
public:
    ProductInWarehouseRepository(Database& _db)
        : db(_db)
    {}
    virtual bool create(const ProductInWarehouseModel &data) override;
    virtual bool read() const override;
    virtual bool remove(int id) override;
    void search(const ProductInWarehouseModel &data);
};

#endif // PRODUCTINWAREHOUSEREPOSITORY_H
