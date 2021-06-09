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
    virtual void create(const DeliveryPointModel &data) override;
    virtual void read() const override;
    virtual void update(const DeliveryPointModel &data) override;
    virtual void remove(int id) override;
    void search(const DeliveryPointModel &data);
};

#endif // DELIVERYPOINTREPOSITORY_H
