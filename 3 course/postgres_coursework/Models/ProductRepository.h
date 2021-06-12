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
    using productNameWithPrice = QPair<QString, float>;

    Database& db;
    bool DoSearch(const ProductModel &data, const QString &_query);
public:
    ProductRepository(Database& _db)
        : db(_db)
    {}
    virtual bool create(const ProductModel &data) override;
    virtual bool read() const override;
    bool readForBuyer() const;
    virtual bool update(const ProductModel &data) override;
    virtual bool remove(int id) override;
    void search(const ProductModel &data);
    void searchForBuyer(const ProductModel &data);
    std::map<int, QString> getAttributesList() const;
    QJsonDocument getJsonSpecs(int id) const;
    int getProductAllCount(const int id) const;
    std::map<int, productNameWithPrice> getAttributesListWithPrices() const;
};

#endif // PRODUCTREPOSITORY_H
