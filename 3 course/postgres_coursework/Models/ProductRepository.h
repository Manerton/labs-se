#ifndef PRODUCTREPOSITORY_H
#define PRODUCTREPOSITORY_H

#include <QString>
#include <QJsonObject>
#include "IRepository.h"

struct ProductModel
{
    int32_t id;
    int32_t id_manufacturer;
    int32_t id_category;
    QString name;
    float cost;
    int16_t warranty;
    QJsonObject specs;
};

class ProductRepository : public IRepository<ProductModel>
{
    Database& db;

public:
    ProductRepository(Database& _db)
        : db(_db)
    {}
    virtual void create(const ProductModel &data) override;
    virtual void read() const override;
    virtual void update(const ProductModel &data) override;
    virtual void remove(int id) override;
    void search(const ProductModel &data);
    std::map<int, QString> getAttributesList() const;
};

#endif // PRODUCTREPOSITORY_H
