#ifndef MANUFACTURERREPOSITORY_H
#define MANUFACTURERREPOSITORY_H

#include <QString>
#include "IRepository.h"

struct ManufacturerModel
{
    int id;
    QString name;
    QString country;
};

class ManufacturerRepository : public IRepository<ManufacturerModel>
{
    Database& db;

    QString getSelectQuery() const;
public:
    ManufacturerRepository(Database& _db)
        : db(_db)
    {}
    virtual void create(const ManufacturerModel &data) override;
    virtual void read() const override;
    virtual void update(const ManufacturerModel &data) override;
    virtual void remove(int id) override;
    void search(const ManufacturerModel &data);
    std::map<int, QString> getAttributesList() const;
};

#endif // MANUFACTURERREPOSITORY_H
