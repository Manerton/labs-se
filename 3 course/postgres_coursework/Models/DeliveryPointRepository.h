#ifndef DELIVERYPOINTREPOSITORY_H
#define DELIVERYPOINTREPOSITORY_H

#include <QString>
#include <vector>
#include "IRepository.h"

struct DeliveryPointModel
{
    int id;
    QString name;
    QString address;
    float area;
    std::vector<QString> extraFunc;
};

class DeliveryPointRepository : public IRepository<DeliveryPointModel>
{
    Database& db;

    QString getSelectQuery() const;
    QVariant getExtraFuncStr(const std::vector<QString> &arr) const;
public:
    DeliveryPointRepository(Database& _db)
        : db(_db)
    {}
    virtual bool create(const DeliveryPointModel &data) override;
    virtual bool read() const override;
    virtual bool update(const DeliveryPointModel &data) override;
    virtual bool remove(int id) override;
    void search(const DeliveryPointModel &data);
    std::map<int, QString> getAttributesList() const;
    QString getExtraFuncForId(int id) const;
};

#endif // DELIVERYPOINTREPOSITORY_H
